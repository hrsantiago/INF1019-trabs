#include <stdio.h>
#include <stdlib.h>
#include "messenger.h"

int main(int argc, char *argv)
{
    synch_t *sync = create_new_s();
    free(sync);
    printf("Hello!\n");
    return 0;
}
