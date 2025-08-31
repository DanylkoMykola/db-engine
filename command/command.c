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
    PrepareResult prepareResult = NULL;
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
    if (strcmp(tokens[0], "select") == 0) {
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


void executeStatement(Statement* statement) {
    switch (statement->type) {
        case STATEMENT_INSERT: {
             FILE* fp = fopen(FILENAME, "ab"); // append binary
            if (!fp) {
                printf("Could not open file for writing.\n");
                return;
            }
            fwrite(&(statement->rowToInsert), sizeof(Row), 1, fp);
            fclose(fp);
            printf("Inserted.\n");
            break;
        }
        case STATEMENT_SELECT: {
            FILE* fp = fopen(FILENAME, "rb");
            if (!fp) {
                printf("Could not open file for reading.\n");
                return;
            }
            Row row;
            while (fread(&row, sizeof(Row), 1, fp) == 1) {
                printf("%d %s %s\n", row.id, row.username, row.email);
            }
            fclose(fp);
            break;
        }
    }
}