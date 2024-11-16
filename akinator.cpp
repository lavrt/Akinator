#include "akinator.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"

void treeDtor(tNode* node)
{
    assert(node);

    if (node->left ) treeDtor(node->left );
    if (node->right) treeDtor(node->right);

    FREE(node);
}

tNode* createNode(void)
{
    tNode* node = (tNode*)calloc(1, sizeof(tNode));
    assert(node);

    return node;
}

void runAkinator(tNode* node)
{
    assert(node);

    char responceBuffer[kLengthOfTheResponceBuffer] = {};

    if (node->left && node->right)
    {
        printf("%s\n", node->data);
        scanf("%s", responceBuffer);

        if (!strcasecmp(responceBuffer, kYes)) { runAkinator(node->left); }
        else if (!strcasecmp(responceBuffer, kNo)) { runAkinator(node->right); }
    }
    else if (!(node->left && node->right)) // FIXME вынести в функцию node->left && node->right
    {
        printf("Это %s. Верно?\n", node->data);
        scanf("%s", responceBuffer);

        if (!strcasecmp(responceBuffer, kYes))
        {
            printf("Отлично, я снова угадал!\n");
            return;
        }
        else if (!strcasecmp(responceBuffer, kNo))
        {
            char correctAnswer[kMaxDataSize] = {};
            char distinctiveFeature[kMaxDataSize] = {};

            printf("И кого же ты загадал?\n"); getchar();
            scanf("%[^\n]", correctAnswer);

            printf("Чем %s отличается от %s?\n%s... ", correctAnswer, node->data, correctAnswer); getchar();
            scanf("%[^\n]", distinctiveFeature);

            node->left = createNode();
            node->right = createNode();
            strcpy(node->right->data, node->data);
            strcpy(node->left->data, correctAnswer);
            strcpy(node->data, distinctiveFeature);

            if (strlen(node->data) + 2 <= kMaxDataSize)
            {
                node->data[strlen(distinctiveFeature)] = '?';
                node->data[strlen(distinctiveFeature) + 1] = '\0';
            }

            printf("Я запомнил, теперь ты меня не проведешь!\n");
        }
    }
    else assert(0);
}

void dump(tNode* root)
{
    assert(root);

    FILE* dumpFile = fopen(kDumpFileName, "w");
    assert(dumpFile);

    fprintf(dumpFile, "digraph\n");
    fprintf(dumpFile, "{\n    ");
    fprintf(dumpFile, "rankdir = TB;\n    ");
    fprintf(dumpFile, "node [shape=record,style = rounded,color=\"#252A34\",penwidth = 2.5];\n    ");
    fprintf(dumpFile, "bgcolor = \"#E7ECEF\";\n\n");

    dumpTreeTraversal(root, dumpFile);
    dumpTreeTraversalWithArrows(root, dumpFile);

    fprintf(dumpFile, "}\n");

    FCLOSE(dumpFile);

    system("dot dump.gv -Tpng -o dump.png");
}

void dumpTreeTraversal(tNode* node, FILE* dumpFile)
{
    assert(dumpFile);
    if (!node) return;

    static size_t rank = 0;
    fprintf(dumpFile, "    node_%p [rank=%lu,label=\"%s\"];\n",
            node, rank, node->data);
    if (node->left)
    {
        rank++;
        dumpTreeTraversal(node->left, dumpFile);
    }
    if (node->right)
    {
        rank++;
        dumpTreeTraversal(node->right, dumpFile);
    }
    rank--;
}

void dumpTreeTraversalWithArrows(tNode* node, FILE* dumpFile)
{
    assert(dumpFile);
    if (!node) return;

    static int flag = 0;
    if (node->left)
    {
        (flag++) ? fprintf(dumpFile, "-> node_%p ", node->left)
                 : fprintf(dumpFile, "    node_%p -> node_%p ", node, node->left);
        dumpTreeTraversalWithArrows(node->left, dumpFile);
    }
    if (node->right)
    {
        (flag++) ? fprintf(dumpFile, "-> node_%p ", node->right)
                 : fprintf(dumpFile, "    node_%p -> node_%p ", node, node->right);
        dumpTreeTraversalWithArrows(node->right, dumpFile);
    }
    if (flag) { fprintf(dumpFile, ";\n"); }
    flag = 0;
}
