#include <stdlib.h>
#include <string.h>

#include "dbstructure.h"

Database* createDatabase() {
    Database* database = malloc(sizeof(Database));
    database -> tables = NULL;
    database -> numTables = 0;
    return database;
}

void freeDatabase(Database* database) {
    if (!database) {
        for (int i = 0; i < database -> numTables; i++) {
            free(&database -> tables[i]);
        }    
    }
    free(database -> tables);
    free(database);
}

Table* createTable(const char* name, int numColumns, Column* columns) {
    Table* table = malloc(sizeof(Table));
    table -> name = strdup(name);
    table -> numColumns = numColumns;
    table -> columns = malloc(sizeof(Column) * numColumns);

    for (int i = 0; i < numColumns; i++) {
        table -> columns[i].name = strdup(columns[i].name);
        table -> columns[i].type = columns[i].type;
    }
    return table;
}

void freeTable(Table* table) {
    if (table) {
        free(table -> name);
        for (int i = 0; i < table -> numColumns; i++) {
            free(table -> columns[i].name);
        }
        free(table -> columns);
    }
} 

