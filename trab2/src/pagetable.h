#ifndef PAGETABLE_H
#define PAGETABLE_H

struct pagetable {
    int lastAccess;
    char r;
    char w;
};
typedef struct pagetable PageTable;

PageTable *createPageTable(int pageSize);
int *createPageVector(int memorySize, int pageSize, int *nPageVector);
int getPageBits(int pageSize);

#endif
