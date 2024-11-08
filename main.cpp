#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define FCLOSE(ptr_) \
    do { fclose(ptr_); ptr_ = NULL; } while (0)
#define $ fprintf(stderr, "%s:%d in function: %s\n", __FILE__, __LINE__, __func__);

struct tNode
{
    int data;
    tNode* left;
    tNode* right;
};

const char* const kDumpFileName = "dump.gv";

void dump(tNode* root);
void treeTraversal(tNode* node, FILE* dumpFile);
void treeTraversalWithArrows(tNode* node, FILE* dumpFile);

void addNode(tNode* node, int value);
void insert(tNode* node, int value);

int main()
{
    tNode root = {50}; // FIXME а если не инициализирован?
    insert(&root, 30);
    insert(&root, 10);
    insert(&root, 70);
    insert(&root, 65);
    insert(&root, 66);
    insert(&root, 80);

    dump(&root);

    return 0;
}

void insert(tNode* node, int value)
{
    if (value < node->data)
    {
        (node->left) ? insert(node->left, value)
                     : addNode(node, value);
    }
    else
    {
        (node->right) ? insert(node->right, value)
                      : addNode(node, value);
    }
}

void addNode(tNode* node, int value)
{
    tNode* newNode = (tNode*)calloc(1, sizeof(tNode));
    assert(newNode);

    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;

    (value < node->data) ? node->left = newNode
                         : node->right = newNode;
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

    treeTraversal(root, dumpFile);
    treeTraversalWithArrows(root, dumpFile);

    fprintf(dumpFile, "}\n");

    FCLOSE(dumpFile);
}

void treeTraversal(tNode* node, FILE* dumpFile)
{
    assert(dumpFile);
    if (!node) return;

    static size_t rank = 0;
    fprintf(dumpFile, "    node_%p [rank=%lu,label=\" {ptr: %p | data: %d | {left: %p | right: %p}} \"];\n",
            node, rank, node, node->data, node->left, node->right);
    if (node->left)
    {
        rank++;
        treeTraversal(node->left, dumpFile);
    }
    if (node->right)
    {
        rank++;
        treeTraversal(node->right, dumpFile);
    }
    rank--;
}

void treeTraversalWithArrows(tNode* node, FILE* dumpFile)
{
    if (!node) return;

    static int flag = 0;
    if (node->left)
    {
        (flag++) ? fprintf(dumpFile, "-> node_%p ", node->left)
                 : fprintf(dumpFile, "    node_%p -> node_%p ", node, node->left);
        treeTraversalWithArrows(node->left, dumpFile);
    }
    if (node->right)
    {
        (flag++) ? fprintf(dumpFile, "-> node_%p ", node->right)
                 : fprintf(dumpFile, "    node_%p -> node_%p ", node, node->right);
        treeTraversalWithArrows(node->right, dumpFile);
    }
    if (flag) { fprintf(dumpFile, ";\n"); }
    flag = 0;
}
