#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "structure/dbstructure.h"
#include "buffer/inputbuff.h"



int main() {
    Database* db = createDatabase();

    Column columns[2] = {
        {"id", COL_INT},
        {"name", COL_TEXT}
    };

    Table* users = createTable("users", 2, columns);
    printf("Created table: %s with %d columns\n", users->name, users->numColumns);

    for (int i = 0; i < users->numColumns; i++) {
        printf("  %s (%s)\n",
               users->columns[i].name,
               users->columns[i].type == COL_INT ? "INT" : "TEXT");
    }

    freeTable(users);
    freeDatabase(db);
    return 0;
}

/* int main(int argc, char* argv[]) {
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
} */