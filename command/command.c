#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../buffer/inputbuff.h"
#include "../structure/dbstructure.h"
#include "command.h"


// Simple tokenizer to split string into words
char** tokenizeInput(const char* input, int* tokenCount) {
    char* copy = strdup(input);
    int count = 0;
    char* tokens[64]; // max 64 tokens per command
    char* tok = strtok(copy, " \n");
    while (tok != NULL) {
        tokens[count++] = strdup(tok);
        tok = strtok(NULL, " \n");
    }
    char** result = malloc(sizeof(char*) * (count + 1));
    for (int i = 0; i < count; i++) result[i] = tokens[i];
    result[count] = NULL;
    *tokenCount = count;
    free(copy);
    return result;
}

MetaCommandResult doMetaCommand(InputBuffer* inputBuffer) {
    if (strcmp(inputBuffer->buffer, ".exit") == 0) {
        closeInputBuffer(inputBuffer);
        exit(EXIT_SUCCESS);
    }
    else {
        return META_COMMAND_UNRECOGNIZE_COMMAND;
    }
}

PrepareResult prepareStatement(InputBuffer* inputBuffer, Statement* statement) {
    int tokenCount = 0;
    PrepareResult prepareResult;
    char** tokens = tokenizeInput(inputBuffer->buffer, &tokenCount);
    if (strcmp(tokens[0], "insert") == 0) {
        if (tokenCount < 3) {
            printf("Syntax error. Use: insert <id> <username> <email>\n");
            return PREPARE_UNRECOGNIZED_STATEMENT; // failure
        }
        statement->type = STATEMENT_INSERT;
        statement->tableName = strdup(tokens[1]);
        statement->values = malloc(sizeof(char*) * (tokenCount - 1));
        for (int i = 2; i < tokenCount; i++) {
            statement->values[i-2] = strdup(tokens[i]);
        }
        statement->values[tokenCount-2] = NULL;        
        prepareResult = PREPARE_SUCCESS;
    }
    else if (strcmp(tokens[0], "select") == 0) {
        if (tokenCount != 2) { 
            printf("Error: SELECT requires table name\n"); return -1; 
        }
        statement->type = STATEMENT_SELECT;
        statement->tableName = strdup(tokens[1]);
        statement->values = NULL;
        prepareResult = PREPARE_SUCCESS;
    }
    else {
        statement->type = STATEMENT_UNKNOWN;
        statement->tableName = NULL;
        statement->values = NULL;
        prepareResult = PREPARE_UNRECOGNIZED_STATEMENT;
    }
    
    // free temporary tokens array
    for (int i = 0; i < tokenCount; i++) free(tokens[i]);
    free(tokens);
    return prepareResult;
}

Table* findTable(Database* database, const char* tableName) {
    for (int i = 0; i < database->numTables; i++) {
        if (strcmp(database->tables[i].name, tableName) == 0) {
            return&database->tables[i];
        }
    }
    return NULL;
}

void executeInsert(Database* database, const char* tableName, char** values) {
    Table* table = findTable(database, tableName);
    if (!table) {
        printf("Error: table '%s' not found.\n", tableName);
        return;
    }

    // Count values
    int value_count = 0;
    while (values[value_count] != NULL) value_count++;

    if (value_count != table->numColumns) {
        printf("Error: table '%s' expects %d values, got %d.\n",
               table->name, table->numColumns, value_count);
        return;
    }

    // Open file for appending
    FILE* f = fopen(table->name, "a");
    if (!f) {
        printf("Error: cannot open file for table '%s'\n", table->name);
        return;
    }

    // Write values separated by '|' for simplicity
    for (int i = 0; i < table->numColumns; i++) {
        fprintf(f, "%s", values[i]);
        if (i < table->numColumns - 1) fprintf(f, "|");
    }
    fprintf(f, "\n");

    fclose(f);
    printf("Inserted row into table '%s'.\n", table->name);
}

void executeSelect(Database* database, const char* tableName) {
    Table* table = findTable(database, tableName);
    if (!table) {
        printf("Error: table '%s' not found.\n", tableName);
        return;
    }

    FILE* f = fopen(table->name, "r");
    if (!f) {
        printf("No rows found in table '%s'.\n", table->name);
        return;
    }

    // Print column headers
    for (int i = 0; i < table->numColumns; i++) {
        printf("%s", table->columns[i].name);
        if (i < table->numColumns - 1) printf(" | ");
    }
    printf("\n");
    printf("--------------------------------------------------\n");

    // Read and print each row
    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        // Remove trailing newline
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';

        // Replace '|' with ' | ' for nicer formatting
        for (char* p = line; *p; p++) {
            if (*p == '|') *p = ' ';
        }

        printf("%s\n", line);
    }

    fclose(f);
}

void executeStatement(Statement* statement, Database* database) {
    switch (statement->type) {
        case STATEMENT_INSERT: {
            executeInsert(database, statement->tableName, statement->values);
            break;
        }
        case STATEMENT_SELECT: {
            executeSelect(database, statement->tableName);
            break;
        }
        case STATEMENT_UNKNOWN: {
            printf("Uknown statement");
            break;
        }
    }
}