#include <stdio.h>
#include <stdint.h>

typedef struct {
    char* buffer;
    size_t bufferLength;
    ssize_t inputLength;
} InputBuffer;

InputBuffer* newInputBuffer();
void printPromt();
void readInput(InputBuffer* inputBuffer);
void closeInputBuffer(InputBuffer* inputBuffer);