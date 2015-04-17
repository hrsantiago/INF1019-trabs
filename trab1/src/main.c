#include <stdio.h>
#include <string.h>
#include "testasync.h"
#include "testsync.h"

int main(int argc, const char *argv[])
{
    if(argc < 2) {
        printf("You must choose a test to run. Choose either sync or async.\n");
        return 0;
    }

    setbuf(stdout, NULL);

    if(strcmp(argv[1], "sync") == 0)
        runSyncTest();
    else if(strcmp(argv[1], "async") == 0)
        runAsyncTest();

    return 0;
}
