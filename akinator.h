#ifndef AKINATOR_H
#define AKINATOR_H

#include "structures.h"
#include <stdio.h>

const char* const kDumpFileName = "dump.gv";
const int kLengthOfTheResponceBuffer = 8;
const char* const kYes = "да";
const char* const kNo = "нет";

void treeDtor(tNode* node);
tNode* createNode(void);
void dump(tNode* root);
void dumpTreeTraversal(tNode* node, FILE* dumpFile);
void dumpTreeTraversalWithArrows(tNode* node, FILE* dumpFile);
void runAkinator(tNode* node);

#endif // AKINATOR_H
