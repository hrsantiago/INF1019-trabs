#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "log.h"
#include "pagetable.h"
#include "debug.h"

int g_debug = 0;

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

    const char* algorithm = argv[1];
    const char *filename = argv[2];
    int pageSize = atol(argv[3]);
    int memorySize = atol(argv[4]);

    printf("Running simulator...\n");
    printf("Input file: %s\n", filename);
    printf("Physical memory size: %d KB\n", memorySize);
    printf("Page size: %d KB\n", pageSize);
    printf("Page replacement algorithm: %s\n", algorithm);

    int nLogEntries;
    LogEntry *logEntries = loadLog(filename, &nLogEntries);
    if(!logEntries)
        return -1;

    int pageFaults = 0;
    int writtenPages = 0;
    int pageBits = getPageBits(pageSize);

    PageTable *pageTable = createPageTable(pageSize);
    if(!pageTable)
        return -1;

    // vector that contains pages on physical memory
    int nPageVector;
    int *pageVector = createPageVector(memorySize, pageSize, &nPageVector);
    if(!pageVector)
        return -1;

    int time;
    for(time = 0; time < nLogEntries; ++time) {
        // process 1 log entry per time unit

        LogEntry logEntry = logEntries[time];
        int pageIndex = logEntry.address >> pageBits;

        int found = 0;
        int i;

        // check if it's on physical memory
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

                // TODO: use algorithm to choose a page index
                int replaceIndex = 0; // using 0 for now..

                // write it back to disk
                PageTable *lastPage = &pageTable[pageVector[replaceIndex]];
                if(lastPage->w)
                    ++writtenPages;
                lastPage->r = 0;
                lastPage->w = 0;

                pageVector[replaceIndex] = pageIndex;
            }
        }

        // page is on disk, update it
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
