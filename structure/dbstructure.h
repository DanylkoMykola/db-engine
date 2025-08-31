#ifndef DBSTRUCTURE_H
#define DBSTRUCTURE_H

#include <stdint.h>

// Suported column types
typedef enum {
    COL_INT,
    COL_TEXT
} ColumnType;

// Column definition
typedef struct {
    char* name;
    ColumnType type;
} Column;

typedef struct {
    char* name;
    int numColumns;
    Column* columns;
} Table;

//Database holds multipel tables
typedef struct {
    int numTables;
    Table* tables;
} Database;

Database* createDatabase();
void freeDatabase(Database* db);

Table* createTable(const char* name, int numColumns, Column* columns);
void freeTable(Table* table);

#endif
