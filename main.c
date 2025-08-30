#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
} Statement;

PrepareResult prepareStatement(InputBuffer* inputBuffer, Statement* statement) {
    if (strncmp(inputBuffer -> buffer, "insert", 6) == 0) {
        statement -> type = STATEMENT_INSERT;
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
        case STATEMENT_INSERT:
            printf("This is where we would do an insert.\n");
            break;
        case STATEMENT_SELECT:
            printf("This is where we would do a select.\n");
            break;
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