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

void updatePagesLRU(PageTable *pageTable, int *pageVector, int nPageVector);
int getPageVectorRemoveIndexLRU(PageTable *pageTable, int *pageVector, int nPageVector);

void updatePagesNRU(PageTable *pageTable, int *pageVector, int nPageVector);
int getPageVectorRemoveIndexNRU(PageTable *pageTable, int *pageVector, int nPageVector);

void updatePagesSEG(PageTable *pageTable, int *pageVector, int nPageVector);
int getPageVectorRemoveIndexSEG(PageTable *pageTable, int *pageVector, int nPageVector);

#endif
