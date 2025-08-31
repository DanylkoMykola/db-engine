#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZE_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
    STATEMENT_UNKNOWN
} StetmentType;

typedef struct {
    StetmentType type;
    char* tableName;
    char** values;
} Statement;

MetaCommandResult doMetaCommand(InputBuffer* inputBuffer);
PrepareResult prepareStatement(InputBuffer* inputBuffer, Statement* statement);
void executeStatement(Statement* statement, Database* database);