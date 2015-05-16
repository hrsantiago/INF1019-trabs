#include <stdio.h>
#include <stdlib.h>

#include "log.h"

LogEntry *loadLog(const char* filename, int *nEntries)
{
    FILE *fp = fopen(filename, "r");
    if(!fp) {
        printf("Could not load log file %s\n", filename);
        return NULL;
    }

    // number of entries knowing that each line has the same amount of characters
    int c;
    int characterCount = 0;
    while((c = fgetc(fp)) != EOF) {
        ++characterCount;
        if(c == '\n')
            break;
    }

    fseek(fp, 0, SEEK_END);
    int lines = ftell(fp) / characterCount;
    printf("Number of lines on log: %d\n", lines);

    LogEntry *logEntries = (LogEntry*)malloc(lines * sizeof(LogEntry));
    if(!logEntries) {
        printf("Could not allocate entries buffer.\n");
        return NULL;
    }

    fseek(fp, 0, SEEK_SET);

    int i;
    for(i = 0; i < lines; ++i) {
        if(fscanf(fp, "%x %c ", &logEntries[i].address, &logEntries[i].access) != 2) {
            printf("Error loading log file. Please ensure that each line has the same amount of characters.\n");
            break;
        }
    }

    fclose(fp);
    *nEntries = lines;
    return logEntries;
}
