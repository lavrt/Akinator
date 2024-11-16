#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "structures.h"
#include "akinator.h"
#include "debug.h"

void gameMode(tNode* root);
void runDefinition(tNode* root);
tNode** wordSearch(tNode* node, char* word, tNode** path, size_t* pathLength);

int main()
{
    tNode* root = createNode();

    databaseFetch(root);

    gameMode(root);

    dump(root);

    databaseSave(root);

    treeDtor(root);

    return 0;
}

void gameMode(tNode* root)
{
    assert(root);

    printf("Выберите режим игры:\n"     );
    printf("1 - Отгадывать\n"           );
    printf("2 - Дать определение\n"     );
    printf("Введите номер режима игры: ");
    int mode = 0;
    scanf("%d", &mode);

    switch (mode)
    {
        case 1:
        {
            runAkinator(root);
        }
        break;
        case 2:
        {
            runDefinition(root);
        }
        break;
        default:
        {
            printf("Игрового режима с номером %d нет.\n", mode);
        }
    }
}

void runDefinition(tNode* root)
{
    assert(root);

    char word[64] = {};
    printf("Введите слово: "); getchar();
    scanf("%[^\n]", word);

    if (strchr(word, '?'))
    {
        printf("Я не могу дать определение этому слову.\n");
        return;
    }

    tNode** path = (tNode**)calloc(1, sizeof(tNode*));
    assert(path);

    size_t pathLength = 0;
    path = wordSearch(root, word, path, &pathLength);

    if (!path[0])
    {
        printf("Я не могу дать определение этому слову.\n");
        return;
    }

    for (int i = (int)pathLength - 1; i >= 0; i--)
    {
        if (i != (int)pathLength - 1 && path[i + 1] == path[i]->right)
        {
            printf("не ");
        }
        if (i == (int)pathLength - 1)
        {
            printf("%s: ", path[i]->data);
            continue;
        }
        if (!i)
        {
            printf("%s\b.\n", path[i]->data);
            continue;
        }
        printf("%s\b, ", path[i]->data);
    }

    FREE(path);
}

tNode** wordSearch(tNode* node, char* word, tNode** path1, size_t* pathLength)
{
    assert(node);
    assert(word);
    assert(path1);
    assert(pathLength);

    static tNode** path = path1;
    static bool successfulSearch = false;
    static size_t i = 0;
    path[i++] = node;
    if (!strcmp(node->data, word))
    {
        successfulSearch = true;
        *pathLength = i;
    }
    if (node->left && !successfulSearch)
    {
        path = (tNode**)realloc(path, (i + 1) * sizeof(tNode*));
        assert(path);
        wordSearch(node->left, word, path, pathLength);
    }
    if (node->right && !successfulSearch)
    {
        path = (tNode**)realloc(path, (i + 1) * sizeof(tNode*));
        assert(path);
        wordSearch(node->right, word, path, pathLength);
    }
    if (!successfulSearch)
    {
        path[--i] = 0;
    }
    return path;
}
