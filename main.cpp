#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define FCLOSE(ptr_) \
    do { fclose(ptr_); ptr_ = NULL; } while (0)
#define FREE(ptr_) \
    do { free(ptr_); ptr_ = NULL; } while (0)
#define $ fprintf(stderr, "%s:%d in function: %s\n", __FILE__, __LINE__, __func__);

const char* const kDumpFileName = "dump.gv";
const int kMaxDataSize = 64;
const int kLengthOfTheResponceBuffer = 8;

struct tNode
{
    char data[kMaxDataSize];
    tNode* left;
    tNode* right;
};

tNode* treeCtor(char* string);
void treeDtor(tNode* node);
tNode* createNode(void);
void dump(tNode* root);
void dumpTreeTraversal(tNode* node, FILE* dumpFile);
void dumpTreeTraversalWithArrows(tNode* node, FILE* dumpFile);
void runAkinator(tNode* node);

int main()
{
    tNode* root = treeCtor("Is it animal?");
    root->left = createNode();
    strcpy(root->left->data, "Poltorahska");
    root->right = createNode();
    strcpy(root->right->data, "Znamenskaya");

    runAkinator(root);

    dump(root);

    treeDtor(root);

    return 0;
}

tNode* treeCtor(char* string)
{
    tNode* root = createNode();

    strcpy(root->data, string);

    return root;
}

void treeDtor(tNode* node)
{
    assert(node);

    if (node->left) { treeDtor(node->left); }
    if (node->right) { treeDtor(node->right); }

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

        if (!strcasecmp(responceBuffer, "yes")) { runAkinator(node->left); }
        else if (!strcasecmp(responceBuffer, "no")) { runAkinator(node->right); }
    }
    else if (!(node->left && node->right))
    {
        printf("It's %s. Right?\n", node->data);
        scanf("%s", responceBuffer);

        if (!strcasecmp(responceBuffer, "yes"))
        {
            printf("Great, I guessed it again!\n");
            return;
        }
        else if (!strcasecmp(responceBuffer, "no"))
        {
            char correctAnswer[kMaxDataSize] = {};
            char distinctiveFeature[kMaxDataSize] = {};

            printf("And who did you wish for?\n"); getchar();
            scanf("%[^\n]", correctAnswer);

            printf("How does %s differ from %s?\n%s is... ", correctAnswer, node->data, correctAnswer); getchar();
            scanf("%[^\n]", distinctiveFeature);

            node->left = createNode();
            node->right = createNode();
            strcpy(node->right->data, node->data);
            strcpy(node->left->data, correctAnswer);
            strcpy(node->data, distinctiveFeature);
            node->data[strlen(node->data)] = '?';

            printf("I remember, now you can't fool me!\n");
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
    fprintf(dumpFile, "    node_%p [rank=%lu,label=\" %s \"];\n",
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
