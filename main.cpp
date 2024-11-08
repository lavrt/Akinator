#include <stdio.h>
#include <assert.h>

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

int main()
{
    tNode node_10 = { 10, 0, 0 };
    tNode node_66 = { 66, 0, 0 };
    tNode node_65 = { 65, 0, &node_66 };
    tNode node_80 = { 80, 0, 0 };
    tNode node_30 = { 30, &node_10, 0 };
    tNode node_70 = { 70, &node_65, &node_80 };
    tNode node_50 = { 50, &node_30, &node_70 };

    dump(&node_50);

    return 0;
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

    fprintf(dumpFile, "    node_%p [rank=%lu,label=\" {ptr: %p | data: %d | {left: %p | right: %p}} \"];\n", node, rank, node, node->data, node->left, node->right);

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
        if (flag)
        {
            fprintf(dumpFile, "-> node_%p ", node->left);
        }
        else
        {
            fprintf(dumpFile, "    node_%p -> node_%p ", node, node->left);
        }
        flag = 1;
        treeTraversalWithArrows(node->left, dumpFile);
    }
    if (node->right)
    {
        if (flag)
        {
            fprintf(dumpFile, "-> node_%p ", node->right);
        }
        else
        {
            fprintf(dumpFile, "    node_%p -> node_%p ", node, node->right);
        }
        flag = 1;
        treeTraversalWithArrows(node->right, dumpFile);
    }
    if (flag) { fprintf(dumpFile, ";\n"); }
    flag = 0;
}
