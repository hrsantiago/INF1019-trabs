// Henrique Rodrigues Santiago - 1120671
// Lucas Ribeiro Borges - 1311003

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "log.h"
#include "pagetable.h"
#include "debug.h"

int g_debug = 0;

int run(const char* algorithm, const char *filename, int pageSize, int memorySize)
{
    void (*updatePages)(PageTable*, int*, int);
    int (*getPageVectorRemoveIndex)(PageTable*, int*, int);

    if(strcmp(algorithm, "LRU") == 0) {
        updatePages = updatePagesLRU;
        getPageVectorRemoveIndex = getPageVectorRemoveIndexLRU;
    }
    else if(strcmp(algorithm, "NRU") == 0) {
        updatePages = updatePagesNRU;
        getPageVectorRemoveIndex = getPageVectorRemoveIndexNRU;
    }
    else if(strcmp(algorithm, "SEG") == 0) {
        updatePages = updatePagesSEG;
        getPageVectorRemoveIndex = getPageVectorRemoveIndexSEG;
    }
    else {
        printf("Unknown algorithm %s\n", algorithm);
        return -1;
    }

    int nLogEntries;
    LogEntry *logEntries = loadLog(filename, &nLogEntries);
    if(!logEntries)
        return -1;

    if(pageSize < 8 || pageSize > 32) {
        printf("Invalid page size %d. It must be between 8KB - 32KB\n", pageSize);
        return -1;
    }

    PageTable *pageTable = createPageTable(pageSize);
    if(!pageTable)
        return -1;

    if(memorySize < 128 || memorySize > 16384) {
        printf("Invalid physical memory size %dKB. It must be between 128KB - 16384KB\n", memorySize);
        return -1;
    }

    // vector that contains pages that are on physical memory
    int nPageVector;
    int *pageVector = createPageVector(memorySize, pageSize, &nPageVector);
    if(!pageVector)
        return -1;

    printf("Running simulator...\n");
    printf("Input file: %s\n", filename);
    printf("Physical memory size: %d KB\n", memorySize);
    printf("Page size: %d KB\n", pageSize);
    printf("Page replacement algorithm: %s\n", algorithm);

    int pageFaults = 0;
    int writtenPages = 0;
    int pageBits = getPageBits(pageSize);
    int time;
    for(time = 0; time < nLogEntries; ++time) {
        // process 1 log entry per time unit
        LogEntry logEntry = logEntries[time];
        int pageIndex = logEntry.address >> pageBits;

        // clock interrupt every 100 units of time
        if(time % 100 == 0)
            updatePages(pageTable, pageVector, nPageVector);

        // check if it's on physical memory
        int found = 0;
        int i;
        for(i = 0; i < nPageVector; ++i) {
            if(pageVector[i] == pageIndex) {
                found = 1;
                break;
            }
        }

        // page is not on physical memory
        if(!found) {

            // check if there's physical memory available to fit this page
            for(i = 0; i < nPageVector; ++i) {
                if(pageVector[i] == -1) {
                    pageVector[i] = pageIndex;
                    found = 1;
                    break;
                }
            }

            // mem is full. must remove someone and then add it
            if(!found) {
                ++pageFaults;

                int replaceIndex = getPageVectorRemoveIndex(pageTable, pageVector, nPageVector);

                // write it back to disk
                PageTable *lastPage = &pageTable[pageVector[replaceIndex]];
                if(lastPage->w)
                    ++writtenPages;
                lastPage->r = 0;
                lastPage->w = 0;

                pageVector[replaceIndex] = pageIndex;
            }
        }

        // page is on mem, update it
        pageTable[pageIndex].lastAccess = time;
        if(logEntry.access == 'R')
            pageTable[pageIndex].r = 1;
        else
            pageTable[pageIndex].w = 1;

    }

    printf("Page faults: %d\n", pageFaults);
    printf("Written pages: %d\n", writtenPages);
    return 0;
}

int main(int argc, const char *argv[])
{
    setbuf(stdout, NULL);

    if(argc < 5) {
        printf("You must enter at least 4 parameters:\n"
               "Algorithm: LRU / NRU / SEG\n"
               "A file containing addresses\n"
               "Page size (KB)\n"
               "Physical memory (KB)\n"
               "-D is optional\n"
               "Eg.\nsim-virtual LRU arquivo.log 16 128 -D\n");
    }
    else if(argc == 6 && strcmp(argv[5], "-D") == 0) {
        g_debug = 1;
    }

    int ret = run(argv[1], argv[2], atol(argv[3]), atol(argv[4]));
    if(ret != 0)
        return ret;

    // run tests
//    int pageSize;
//    for(pageSize = 8; pageSize <= 32; pageSize += 8) {
//        int ret = run(argv[1], argv[2], pageSize, atol(argv[4]));
//        if(ret != 0)
//            return ret;
//    }

    return 0;
}
