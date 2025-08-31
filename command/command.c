#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../buffer/inputbuff.h"
#include "../structure/rowstruct.h"
#include "command.h"



MetaCommandResult doMetaCommand(InputBuffer* inputBuffer) {
    if (strcmp(inputBuffer -> buffer, ".exit") == 0) {
        closeInputBuffer(inputBuffer);
        exit(EXIT_SUCCESS);
    }
    else {
        return META_COMMAND_UNRECOGNIZE_COMMAND;
    }
}

PrepareResult prepareStatement(InputBuffer* inputBuffer, Statement* statement) {
    if (strncmp(inputBuffer -> buffer, "insert", 6) == 0) {
        statement -> type = STATEMENT_INSERT;
         // simple parsing: "insert id username email"
        int id = 0;
        char username[USERNAME_SIZE];
        char email[EMAIL_SIZE];
        int n = sscanf(inputBuffer->buffer, "insert %d %31s %254s", &id, username, email);
        if (n < 3) {
            printf("Syntax error. Use: insert <id> <username> <email>\n");
            return PREPARE_UNRECOGNIZED_STATEMENT; // failure
        }
        statement->rowToInsert.id = (uint32_t)id;
        strncpy(statement->rowToInsert.username, username, USERNAME_SIZE);
        strncpy(statement->rowToInsert.email, email, EMAIL_SIZE);
        return PREPARE_SUCCESS;
    }
    if (strcmp(inputBuffer -> buffer, "select") == 0) {
        statement -> type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
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