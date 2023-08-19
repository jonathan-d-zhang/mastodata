#include <czmq.h>
#include <zsock.h>
#include <zstr.h>
#include <zmsg.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include "hll.h"

int main() {
    zsock_t *sub = zsock_new_sub("tcp://localhost:5556", "");
    char *string;
    long id;

    while (1) {
        string = zstr_recv(sub);
        id = atoll(string);
        printf("%ld\n", id);
    }

    zstr_free(&string);
    zsock_destroy(&sub);
    return 0;
}
