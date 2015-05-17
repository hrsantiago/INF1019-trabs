#ifndef DEBUG_H
#define DEBUG_H

extern int g_debug;

#define printfd(...) if(g_debug) printf(__VA_ARGS__)

#endif
