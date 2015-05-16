#ifndef LOG_H
#define LOG_H

struct logentry {
    unsigned int address;
    char access;
};
typedef struct logentry LogEntry;

LogEntry *loadLog(const char* filename, int *nEntries);

#endif
