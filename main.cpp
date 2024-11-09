#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define FCLOSE(ptr_) \
    do { fclose(ptr_); ptr_ = NULL; } while (0)
#define FREE(ptr_) \
    do { free(ptr_); ptr_ = NULL; } while (0)
#define $ fprintf(stderr, "%s:%d in function: %s\n", __FILE__, __LINE__, __func__);

struct tNode
{
    int data;
    tNode* left;
    tNode* right;
};

const char* const kDumpFileName = "dump.gv";

tNode* treeCtor(int value);
void treeDtor(tNode* node);
tNode* createNode(void);
void dump(tNode* root);
void dumpTreeTraversal(tNode* node, FILE* dumpFile);
void dumpTreeTraversalWithArrows(tNode* node, FILE* dumpFile);

void addNode(tNode* node, int value);
void insert(tNode* node, int value);

int main()
{
    tNode* root = treeCtor(50);
    insert(root, 30);
    insert(root, 10);
    insert(root, 70);
    insert(root, 65);
    insert(root, 66);
    insert(root, 80);

    dump(root);

    treeDtor(root);
    return 0;
}

tNode* treeCtor(int value)
{
    tNode* root = createNode();

    root->data = value;

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
    tNode* newNode = createNode();

    newNode->data = value;

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

    dumpTreeTraversal(root, dumpFile);
    dumpTreeTraversalWithArrows(root, dumpFile);

    fprintf(dumpFile, "}\n");

    FCLOSE(dumpFile);
}

void dumpTreeTraversal(tNode* node, FILE* dumpFile)
{
    assert(dumpFile);
    if (!node) return;

    static size_t rank = 0;
    fprintf(dumpFile, "    node_%p [rank=%lu,label=\" {ptr: %p | data: %d | {left: %p | right: %p}} \"];\n",
            node, rank, node, node->data, node->left, node->right);
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
