#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "pagetable.h"

PageTable *createPageTable(int pageSize)
{
    int pageBits = log2(pageSize << 10);
    int pageTableSize = pow(2, 32 - pageBits);
    printf("Page table entries: %d\n", pageTableSize);

    PageTable *pageTable = (PageTable*)malloc(pageTableSize * sizeof(PageTable));
    if(!pageTable) {
        printf("Not enough memory to simulate page table.\n");
        return NULL;
    }

    memset(pageTable, 0, pageTableSize * sizeof(PageTable));
    return pageTable;
}
