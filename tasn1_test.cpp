
#include "tasn1/tasn1.h"
#include "tasn1/map.hpp"
#include "tasn1/array.hpp"
#include "tasn1/octetsequence.hpp"
#include "tasn1/number.hpp"

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cctype>

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

static void c_tests() {
    int erc;

    tasn1_node_t *map1 = tasn1_new_map();
    assert(map1);

    erc = tasn1_add_map_string(map1, "KEY1", false, tasn1_new_string("VAL1", false));
    assert(erc == 0);

    erc = tasn1_add_map_string(map1, "KEY2", false, tasn1_new_bool(true));
    assert(erc == 0);

    int size1 = tasn1_size(map1);
    assert(size1 > 0);
    //printf("Size = %i\n", size1);

    TASN1_OCTET buf1[10];
    erc = tasn1_serialize(map1, buf1, sizeof(buf1));
    //printf("erc = %i: %s\n", erc, strerror(-erc));
    assert(erc < 0);

    TASN1_OCTET buf2[40];
    int size2 = tasn1_serialize(map1, buf2, sizeof(buf2));
    assert(size2 > 0);
    dump(buf2, size2);
    assert(size2 == size1);

    tasn1_free(map1);
}

static void cpp_tests() {
    tasn1::Map map1;
    tasn1::OctetSequence val1("VAL1");
    map1.add("KEY1", val1);

    tasn1::Number val2(true);
    map1.add("KEY2", val2);

    try {
        map1.add("KEY3", val1);
        assert(false);
    }  catch (const std::runtime_error &) {
        // Expected
    }

    tasn1::vector_t buffer;
    map1.serialize(buffer);

    dump(buffer.data(), buffer.size());
}

int main() {
    printf("Running C tests ...\n");
    c_tests();
    printf("Running C++ tests ...\n");
    cpp_tests();
    printf("Success!\n");
    return EXIT_SUCCESS;
}
