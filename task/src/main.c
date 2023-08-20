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
    char *filter = getenv("SUB_FILTER");
    char *proxy_address = getenv("PUB_ADDRESS");
    if (NULL == filter)
    {
        filter = "";
    }
    zsock_t *sub = zsock_new_sub(proxy_address, filter);

    char *string;

    while (1) {
        string = zstr_recv(sub);
        printf("%s\n", string);
    }

    zstr_free(&string);
    zsock_destroy(&sub);
    return 0;
}
