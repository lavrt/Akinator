#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

void databaseSave(tNode* root);
void databaseEntry(tNode* node, FILE* database);
void databaseFetch(tNode* root);
void databaseRead(tNode* node, FILE* database, char* dataArray, int64_t fileSize);

#endif // DATABASE_H
