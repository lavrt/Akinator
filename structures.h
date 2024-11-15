#ifndef STRUCTURES_H
#define STRUCTURES_H

const int kMaxDataSize = 64;

#define FCLOSE(ptr_) \
    do { fclose(ptr_); ptr_ = NULL; } while (0)
#define FREE(ptr_) \
    do { free(ptr_); ptr_ = NULL; } while (0)

struct tNode
{
    char data[kMaxDataSize];
    tNode* left;
    tNode* right;
};

#endif // STRUCTURES_H
