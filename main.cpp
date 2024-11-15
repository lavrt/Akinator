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
void databaseSave(tNode* root);
int databaseEntry(tNode* node, FILE* database);
void databaseFetch(tNode* root);
void databaseRead(tNode* node, FILE* database, char* dataArray, int64_t fileSize);

int main()
{
    tNode* root = treeCtor("");
    databaseFetch(root);

    // runAkinator(root);

    dump(root);
    // databaseSave(root);

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
            if (strlen(node->data) + 2 <= kMaxDataSize)
            {
                node->data[strlen(correctAnswer)] = '?';
                node->data[strlen(correctAnswer) + 1] = '\0';
            }

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

void databaseSave(tNode* root)
{
    assert(root);

    FILE* database = fopen("database.txt", "w");
    assert(database);

    int depthOfRightmostPath = databaseEntry(root, database);

    for (int i = 0; i < depthOfRightmostPath - 1; i++)
    {
        for (int j = i; j < depthOfRightmostPath - 2; j++)
        {
            fprintf(database, "    ");
        }
        fprintf(database, "}\n");
    }

    FCLOSE(database);
}

int databaseEntry(tNode* node, FILE* database)
{
    assert(database);
    assert(node);

    static int rank = 0;
    static int depthOfRightmostPath = 0;

    for (int i = 0; i < rank    ; i++) { fprintf(database, "    "); } fprintf(database, "{\n");
    for (int i = 0; i < rank + 1; i++) { fprintf(database, "    "); }

    fprintf(database, "\"%s\"\n", node->data);

    for (int i = 0; i < rank; i++) { fprintf(database, "    "); } fprintf(database, "}\n");

    if (node->left)
    {
        rank++;
        depthOfRightmostPath = 0;
        databaseEntry(node->left, database);
    }
    if (node->right)
    {
        rank++;
        depthOfRightmostPath = 0;
        databaseEntry(node->right, database);
    }
    depthOfRightmostPath++;
    rank--;

    return depthOfRightmostPath;
}

void databaseFetch(tNode* root)
{
    assert(root);

    FILE* database = fopen("database.txt", "r");
    assert(database);

    fseek(database, 0, SEEK_END);
    int64_t fileSize = ftello(database);
    fseek(database, 0, SEEK_SET);

    char* dataArray = (char*)calloc(fileSize, sizeof(char));
    assert(dataArray);
    fread(dataArray, sizeof(char), fileSize, database);

    char* dataArrayClear = (char*)calloc(fileSize, sizeof(char));
    assert(dataArrayClear);

    bool inWord = false;
    int j = 0;
    for (int i = 0; i < fileSize; i++)
    {
        if (dataArray[i] == '\n' || (dataArray[i] == ' ' && !inWord)) { continue; }
        dataArrayClear[j++] = dataArray[i];
        if (dataArray[i] == '"' && !inWord)
        {
            inWord = true;
        }
        else if (dataArray[i] == '"' && inWord)
        {
            inWord = false;
        }
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

    char* ptr = 0;
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

// {"animal?"{"poltorashka"}{"studying?"{"student in session"}{"student"}}}

// {
//     "animal?"
//     {
//         "poltorashka"
//     }
//     {
//         "studying?"
//         {
//             "student in session"
//         }
//         {
//             "student"
//         }
//     }
// }



//
// {
//     "животное?"
//     {
//         "1.5ка"
//     }
//     {
//         "ведет матан?"
//         {
//             "знаменская"
//         }
//         {
//             "ведет физос?"
//             {
//                 "овчос"
//             }
//             {
//                 "комендант"
//             }
//         }
//     }
// }
