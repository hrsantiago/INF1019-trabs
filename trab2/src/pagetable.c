#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "debug.h"
#include "pagetable.h"

PageTable *createPageTable(int pageSize)
{
    int pageBits = getPageBits(pageSize);
    int pageTableSize = pow(2, 32 - pageBits);
    printfd("Page table entries: %d\n", pageTableSize);

    PageTable *pageTable = (PageTable*)malloc(pageTableSize * sizeof(PageTable));
    if(!pageTable) {
        printf("Not enough memory to simulate page table.\n");
        return NULL;
    }

    memset(pageTable, 0, pageTableSize * sizeof(PageTable));
    return pageTable;
}

int *createPageVector(int memorySize, int pageSize, int *nPageVector)
{
    *nPageVector = memorySize / pageSize;
    printfd("Maximum pages at same time: %d\n", *nPageVector);

    int *pageVector = (int*)malloc((*nPageVector) * sizeof(int));
    if(!pageVector) {
        printf("Not enough memory to simulate page vector.\n");
        return NULL;
    }
    int i;
    for(i = 0; i < *nPageVector; ++i)
        pageVector[i] = -1;
    return pageVector;
}

int getPageBits(int pageSize)
{
    return log2(pageSize << 10);
}

void updatePagesLRU(PageTable *pageTable, int *pageVector, int nPageVector)
{

}

int getPageVectorRemoveIndexLRU(PageTable *pageTable, int *pageVector, int nPageVector)
{
    int minNotAccessedPageVectorIndex = -1;
    int minNotAccessedPageTime;
    int minPageVectorIndex = -1;
    int minPageTime;

    int i;
    for(i = 0; i < nPageVector; ++i) {
        if(i == 0) {
            minPageVectorIndex = i;
            minPageTime = pageTable[pageVector[i]].lastAccess;
            if(pageTable[pageVector[i]].r == 0) {
                minNotAccessedPageVectorIndex = i;
                minNotAccessedPageTime = minPageTime;
            }
        }
        else if(pageTable[pageVector[i]].lastAccess < minPageTime) {
            minPageVectorIndex = i;
            minPageTime = pageTable[pageVector[i]].lastAccess;
            if(pageTable[pageVector[i]].r == 0) {
                minNotAccessedPageVectorIndex = i;
                minNotAccessedPageTime = minPageTime;
            }
        }
    }

    if(minNotAccessedPageVectorIndex != -1)
        return minNotAccessedPageVectorIndex;
    return minPageVectorIndex;
}

void updatePagesNRU(PageTable *pageTable, int *pageVector, int nPageVector)
{
    int i;
    for(i = 0; i < nPageVector; ++i) {
        int pageIndex = pageVector[i];
        if(pageIndex != -1) {
            pageTable[pageIndex].r = 0;
        }
    }
}

int getPageVectorRemoveIndexNRU(PageTable *pageTable, int *pageVector, int nPageVector)
{
    int x,y;
    for(y = 0; y < 4; ++y) {
        for(x = 0; x < nPageVector; ++x) {
            PageTable *page = &pageTable[pageVector[x]];
            if(y == 0 && page->r == 0 && page->w == 0)
                return x;
            if(y == 1 && page->r == 0 && page->w == 1)
                return x;
            if(y == 2 && page->r == 1 && page->w == 0)
                return x;
            if(y == 3 && page->r == 1 && page->w == 1)
                return x;
        }
    }
    return 0;
}

void updatePagesSEG(PageTable *pageTable, int *pageVector, int nPageVector)
{

}

int getPageVectorRemoveIndexSEG(PageTable *pageTable, int *pageVector, int nPageVector)
{
    // remove if R=0, and move it to the end as this is a queue
    do {
        int pageIndex = pageVector[0];

        int i;
        for(i = 0; i < nPageVector-1; ++i)
            pageVector[i] = pageVector[i+1];
        pageVector[nPageVector-1] = pageIndex;

        if(pageTable[pageIndex].r == 0)
            return nPageVector-1;
        else
            pageTable[pageIndex].r = 0;

    } while(1);
}
