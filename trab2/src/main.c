#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct logentry {
    unsigned int address;
    char access;
};
typedef struct logentry LogEntry;

LogEntry *loadLog(const char* filename)
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
            printf("Unkonwn error loading log file. Please ensure that each line has the same amount of characters.\n");
            break;
        }
    }

    fclose(fp);
    return logEntries;
}

int main(int argc, const char *argv[])
{
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

    LogEntry *logEntries = loadLog(filename);
    if(!logEntries)
        return -1;

    setbuf(stdout, NULL);

    return 0;
}
