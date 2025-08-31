#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "structure/rowstruct.h"
#include "buffer/inputbuff.h"
#include "command/command.h"

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