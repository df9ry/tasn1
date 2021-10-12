
#include "tasn1.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

static void dump(TASN1_OCTET *pb, int cb) {
    printf("|");
    for (int i = 0; i < cb; ++i) {
        TASN1_OCTET o = pb[i];
        printf("%02x|", o);
    } // end for //
    printf("\n");
    printf("|");
    for (int i = 0; i < cb; ++i) {
        int c = pb[i];
        if (isprint(c))
            printf("%1c |", c);
        else
            printf(". |");
    } // end for //
    printf("\n");
}

int main() {
    int erc;

    printf("Running tests ...\n");

    tasn1_node_t *map1 = tasn1_new_map();
    assert(map1);

    erc = tasn1_add_map_string(map1, "KEY1", false, tasn1_new_string("VAL1", false));
    assert(erc == 0);

    erc = tasn1_add_map_string(map1, "KEY2", false, tasn1_new_bool(true));
    assert(erc == 0);

    int size1 = tasn1_size(map1);
    assert(size1 > 0);
    printf("Size = %i\n", size1);
    TASN1_OCTET buf[40];
    int size2 = tasn1_serialize(map1, buf, sizeof(buf));
    assert(size2 > 0);
    dump(buf, size2);
    assert(size2 == size1);

    tasn1_free(map1);

    printf("Success!\n");
    return EXIT_SUCCESS;
}
