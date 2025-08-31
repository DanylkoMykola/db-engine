#include <stdio.h>
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