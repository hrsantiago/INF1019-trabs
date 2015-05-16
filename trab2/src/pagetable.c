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
