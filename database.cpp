#include "database.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "akinator.h"
#include "debug.h"

void databaseSave(tNode* root)
{
    assert(root);

    FILE* database = fopen("database.txt", "w"); // FIXME const char const
    assert(database);

    fprintf(database, "{\n");
    databaseEntry(root, database);

    FCLOSE(database);
}

void databaseEntry(tNode* node, FILE* database)
{
    assert(database);
    assert(node);

    static int rank = 0;
    static int depthOfRightmostPath = 0;

    for (int i = 0; i < rank + 1; i++)
    {
        fprintf(database, "    ");
    }

    fprintf(database, "\"%s\"\n", node->data);

    if (node->left)
    {
        rank++;

        for (int i = 0; i < rank; i++)
        {
            fprintf(database, "    ");
        }

        fprintf(database, "{\n");

        depthOfRightmostPath = 0;
        databaseEntry(node->left, database);
    }
    if (node->right)
    {
        rank++;

        for (int i = 0; i < rank; i++)
        {
            fprintf(database, "    ");
        }
        fprintf(database, "{\n");

        depthOfRightmostPath = 0;
        databaseEntry(node->right, database);
    }
    depthOfRightmostPath++;
    for (int i = 0; i < rank; i++)
    {
        fprintf(database, "    ");
    }
    fprintf(database, "}\n");
    rank--;
}

void databaseFetch(tNode* root)
{
    assert(root);

    // FIXME database.txt вынести в константу
    FILE* database = fopen("database.txt", "r");
    assert(database);

    // FIXME вынести в функцию
    fseek(database, 0, SEEK_END);
    int64_t fileSize = ftello(database);
    fseek(database, 0, SEEK_SET);

    char* dataArray = (char*)calloc((size_t)fileSize, sizeof(char));
    assert(dataArray);

    fread(dataArray, sizeof(char), (size_t)fileSize, database);

    char* dataArrayClear = (char*)calloc((size_t)fileSize, sizeof(char));
    assert(dataArrayClear);

    bool inWord = false;
    int j = 0;
    for (int i = 0; i < fileSize; i++)
    {
        if (dataArray[i] == '\n' || (dataArray[i] == ' ' && !inWord)) { continue; }
        dataArrayClear[j++] = dataArray[i];
        if (dataArray[i] == '"' && !inWord)     { inWord = true; }
        else if (dataArray[i] == '"' && inWord) { inWord = false; }
    }

    databaseRead(root, database, dataArrayClear, fileSize);

    FCLOSE(database);
    FREE(dataArray);
    FREE(dataArrayClear);
}

void databaseRead(tNode* node, FILE* database, char* dataArray, int64_t fileSize)
{
    assert(node);
    assert(database);

    char* ptr = NULL;
    static int status = 0;
    (status++) ? ptr = strtok(NULL,      "{\"}")
               : ptr = strtok(dataArray, "{\"}");

    strcpy(node->data, ptr);

    static int pos = 1;
    while (pos < fileSize)
    {
        if (dataArray[pos++] == '{')
        {
            node->left = createNode();
            node->right = createNode();
            databaseRead(node->left, database, dataArray, fileSize);
            databaseRead(node->right, database, dataArray, fileSize);
        }
        if (dataArray[pos - 1] == '}')
        {
            if (dataArray[pos] == '{') { pos++; }
            return;
        }
    }
}
