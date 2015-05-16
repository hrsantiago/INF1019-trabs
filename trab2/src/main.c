#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "log.h"
#include "pagetable.h"

int main(int argc, const char *argv[])
{
    setbuf(stdout, NULL);

    if(argc != 5) {
        printf("You must enter 4 parameters:\n"
               "Algorithm: LRU / NRU / SEG\n"
               "A file containing addresses\n"
               "Page size (KB)\n"
               "Physical memory (KB)\n"
               "Eg.\nsim-virtual LRU arquivo.log 16 128\n");
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

    int pageBits = log2(pageSize << 10);

    PageTable *pageTable = createPageTable(pageSize);
    if(!pageTable)
        return -1;


    int maxPages = memorySize / pageSize;
    printf("Maximum pages at same time: %d\n", maxPages);

    int *referencedPages = (int*)malloc(maxPages * sizeof(int));
    int nReferencedPages = 0;

    int time;
    for(time = 0; time < nLogEntries; ++time) {
        // process 1 log entry per time unit

        LogEntry logEntry = logEntries[time];
        int pageIndex = logEntry.address >> pageBits;

        pageTable[pageIndex].lastAccess = time;
        if(logEntry.access == 'R')
            pageTable[pageIndex].r = 1;
        else
            pageTable[pageIndex].w = 1;

        int found = 0;
        int i;
        for(i = 0; i < maxPages; ++i) {
            if(referencedPages[i] == pageIndex) {
                found = 1;
                break;
            }
        }

        // page is not on physical memory
        if(!found) {

            // simply ref it
            if(nReferencedPages < maxPages) {
                referencedPages[nReferencedPages++] = pageIndex;
            }
            else {
                ++pageFaults;

                // TODO: use algorithm to choose a page index

                int replaceIndex = 0; // using 0 for now..

                if(pageTable[referencedPages[replaceIndex]].w)
                    ++writtenPages;

                referencedPages[replaceIndex] = pageIndex;
            }
        }
    }

    printf("Page faults: %d\n", pageFaults);
    printf("Written pages: %d\n", writtenPages);

    return 0;
}
