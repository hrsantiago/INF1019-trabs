#include <stdio.h>
#include <stdlib.h>
#include "testasync.h"
#include "testsync.h"

int main(int argc, char *argv)
{
    setbuf(stdout, NULL);

    runAsyncTest();
    //runSyncTest();
    return 0;
}
