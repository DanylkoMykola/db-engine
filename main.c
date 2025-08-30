#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


#define USERNAME_SIZE 32
#define EMAIL_SIZE 255
#define FILENAME "my.db"

// ---------------------------
// Row structure
// ---------------------------
typedef struct {
    uint32_t id;
    char username[USERNAME_SIZE];
    char email[EMAIL_SIZE];
} Row;
//
// Input buffer
//

typedef struct {
    char* buffer;
    size_t bufferLength;
    ssize_t inputLength;
} InputBuffer;

InputBuffer* newInputBuffer() {
    InputBuffer* inputBuffer = (InputBuffer*) malloc(sizeof(InputBuffer));
    inputBuffer -> buffer = NULL;
    inputBuffer -> bufferLength = 0;
    inputBuffer -> inputLength = 0;
    return inputBuffer;
}

void printPromt() {
    printf("db > ");
}

void readInput(InputBuffer* inputBuffer) {
    ssize_t bytesRead = getline(&(inputBuffer -> buffer), &(inputBuffer -> bufferLength), stdin);

    if (bytesRead <= 0) {
        printf("Error reading input!\n");
        exit(EXIT_FAILURE);
    }

    //Ignore traling new line
    inputBuffer -> inputLength = bytesRead -1;
    inputBuffer -> buffer[bytesRead -1] = 0;
}

void closeInputBuffer(InputBuffer* inputBuffer) {
    free(inputBuffer -> buffer);
    free(inputBuffer);
}


//
// Command Handling
//

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZE_COMMAND
} MetaCommandResult;

MetaCommandResult doMetaCommand(InputBuffer* inputBuffer) {
    if (strcmp(inputBuffer -> buffer, ".exit") == 0) {
        closeInputBuffer(inputBuffer);
        exit(EXIT_SUCCESS);
    }
    else {
        return META_COMMAND_UNRECOGNIZE_COMMAND;
    }
}


typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StetmentType;

typedef struct {
    StetmentType type;
    Row rowToInsert; // used only for INSERT
} Statement;

PrepareResult prepareStatement(InputBuffer* inputBuffer, Statement* statement) {
    if (strncmp(inputBuffer -> buffer, "insert", 6) == 0) {
        statement -> type = STATEMENT_INSERT;
         // simple parsing: "insert id username email"
        int id = 0;
        char username[USERNAME_SIZE];
        char email[EMAIL_SIZE];
        int n.exit = sscanf(inputBuffer->buffer, "insert %d %31s %254s", &id, username, email);
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

int main(int argc, char* argv[]) {
    InputBuffer* inputBuffer = newInputBuffer();
    while (1) {
        printPromt();
        readInput(inputBuffer);

        if (inputBuffer -> buffer[0] == '.') {
            switch (doMetaCommand(inputBuffer)) {
            case META_COMMAND_SUCCESS:
                continue;
                break;
            case META_COMMAND_UNRECOGNIZE_COMMAND:
                printf("Unrecognize command '%s'\n", inputBuffer -> buffer);
                continue;
                break;
            }
        }
    
        
        Statement statement;
        switch (prepareStatement(inputBuffer, &statement)) {
            case PREPARE_SUCCESS:
                break;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized keyword at start of '%s'.\n", inputBuffer->buffer);
                continue;
            }

        executeStatement(&statement);
        printf("Executed.\n");
    }
}