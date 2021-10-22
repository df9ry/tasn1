
#include "tasn1.h"
#include "tasn1/map.hpp"
#include "tasn1/array.hpp"
#include "tasn1/octetsequence.hpp"
#include "tasn1/number.hpp"

#undef DUMP

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>

using namespace std;
using namespace jsonx;
using namespace tasn1;

#ifdef DUMP
static void dump(TASN1_OCTET_T *pb, int cb) {
    printf("|");
    for (int i = 0; i < cb; ++i) {
        TASN1_OCTET_T o = pb[i];
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
#endif

#define BUF_S 1024

static void c_tests() {
    int erc, n;
    TASN1_NUMBER_T number;

    tasn1_node_t *node = NULL;
    TASN1_OCTET_T buf[BUF_S];

    assert(tasn1_get_type(NULL) == TASN1_INVALID);

    //// Test numbers ////

    node = tasn1_new_number(SHRT_MIN);
    assert(node);
    assert(tasn1_size(node) == 3);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 3);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    tasn1_free(node);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == SHRT_MIN);

    node = tasn1_new_number(-1234);
    assert(node);
    assert(tasn1_size(node) == 3);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 3);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    tasn1_free(node);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == -1234);

    node = tasn1_new_number(SCHAR_MIN);
    assert(node);
    assert(tasn1_size(node) == 2);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 2);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    tasn1_free(node);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == SCHAR_MIN);

    node = tasn1_new_number(-56);
    assert(node);
    assert(tasn1_size(node) == 2);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 2);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    tasn1_free(node);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == -56);

    node = tasn1_new_number(-16);
    assert(node);
    assert(tasn1_size(node) == 1);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    tasn1_free(node);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == -16);

    node = tasn1_new_number(-9);
    assert(node);
    assert(tasn1_size(node) == 1);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    tasn1_free(node);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == -9);

    node = tasn1_new_number(0);
    assert(node);
    assert(tasn1_size(node) == 1);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    tasn1_free(node);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == 0);

    node = tasn1_new_number(11);
    assert(node);
    assert(tasn1_size(node) == 1);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    tasn1_free(node);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == 11);

    node = tasn1_new_number(15);
    assert(node);
    assert(tasn1_size(node) == 1);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    tasn1_free(node);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == 15);

    node = tasn1_new_number(99);
    assert(node);
    assert(tasn1_size(node) == 2);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 2);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    tasn1_free(node);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == 99);

    node = tasn1_new_number(SCHAR_MAX);
    assert(node);
    assert(tasn1_size(node) == 2);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 2);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    tasn1_free(node);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == SCHAR_MAX);

    node = tasn1_new_number(5678);
    assert(node);
    assert(tasn1_size(node) == 3);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 3);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    tasn1_free(node);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == 5678);

    node = tasn1_new_number(SHRT_MAX);
    assert(node);
    assert(tasn1_size(node) == 3);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 3);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    tasn1_free(node);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == SHRT_MAX);

    //// Test octet sequence ////

    node = tasn1_new_octet_sequence(NULL, 0, false);
    assert(!node);

    node = tasn1_new_octet_sequence(NULL, 0, true);
    assert(!node);

    const TASN1_OCTET_T *po;
    TASN1_SIZE_T         co;

    node = tasn1_new_octet_sequence(buf, 0, false);
    assert(tasn1_size(node) == 1);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_OCTET_SEQUENCE);
    tasn1_free(node);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 1);
    assert(co == 0);

    node = tasn1_new_octet_sequence(buf, 0, true);
    assert(node);
    assert(tasn1_size(node) == 1);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_OCTET_SEQUENCE);
    tasn1_free(node);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 1);
    assert(co == 0);

    const TASN1_OCTET_T os_1[] { 0x01, 0x02, 0x03, 0x04 };
    node = tasn1_new_octet_sequence(os_1, sizeof(os_1), false);
    assert(node);
    assert(tasn1_size(node) == sizeof(os_1) + 1);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == sizeof(os_1) + 1);
    tasn1_free(node);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 1);
    assert(co == sizeof(os_1));
    assert(memcmp(os_1, po, co) == 0);

    node = tasn1_new_octet_sequence(os_1, sizeof(os_1), true);
    assert(node);
    assert(tasn1_size(node) == sizeof(os_1) + 1);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == sizeof(os_1) + 1);
    tasn1_free(node);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 1);
    assert(co == sizeof(os_1));
    assert(memcmp(os_1, po, co) == 0);

    const TASN1_OCTET_T os_2[] { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                                 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                                 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e        };
    node = tasn1_new_octet_sequence(os_2, sizeof(os_2), false);
    assert(node);
    assert(tasn1_size(node) == sizeof(os_2) + 1);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == sizeof(os_2) + 1);
    tasn1_free(node);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 1);
    assert(co == sizeof(os_2));
    assert(memcmp(os_2, po, co) == 0);

    const TASN1_OCTET_T os_3[] { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                                 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                                 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
    node = tasn1_new_octet_sequence(os_3, sizeof(os_3), false);
    assert(node);
    assert(tasn1_size(node) == sizeof(os_3) + 2);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == sizeof(os_3) + 2);
    tasn1_free(node);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 2);
    assert(co == sizeof(os_3));
    assert(memcmp(os_3, po, co) == 0);

    TASN1_OCTET_T os_4[BUF_S];
    for (size_t i = 0; i < 256; os_4[i] = i ^ 0x55, ++i);

    node = tasn1_new_octet_sequence(os_4, 255, false);
    assert(node);
    assert(tasn1_size(node) == 255 + 2);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 255 + 2);
    tasn1_free(node);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 2);
    assert(co == 255);
    assert(memcmp(os_4, po, co) == 0);

    os_4[256] = 0xf3;

    node = tasn1_new_octet_sequence(os_4, 256, false);
    assert(node);
    assert(tasn1_size(node) == 256 + 3);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 256 + 3);
    tasn1_free(node);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 3);
    assert(co == 256);
    assert(memcmp(os_4, po, co) == 0);

    for (size_t i = 0; i < 1004; os_4[i] = i & 0x55, ++i);

    node = tasn1_new_octet_sequence(os_4, 1004, false);
    assert(node);
    assert(tasn1_size(node) == 1004 + 3);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 1004 + 3);
    tasn1_free(node);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 3);
    assert(co == 1004);
    assert(memcmp(os_4, po, co) == 0);

    const char *astring;

    node = tasn1_new_string("", false);
    assert(node);
    assert(tasn1_size(node) == 2);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 2);
    tasn1_free(node);
    astring = tasn1_get_string(buf);
    assert(astring);
    assert(strcmp(astring, "") == 0);

    const char *test_string = "The quick brown fox jumps over the lazy dog, 1234567890";

    node = tasn1_new_string(test_string, true);
    assert(node);
    assert(tasn1_size(node) == (int)strlen(test_string) + 3);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == (int)strlen(test_string) + 3);
    tasn1_free(node);
    astring = tasn1_get_string(buf);
    assert(astring);
    assert(strcmp(astring, test_string) == 0);

    //// Test arrays ////

    node = tasn1_new_array();
    assert(node);
    n = tasn1_serialize(node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_size(node) == 1);
    tasn1_free(node);
    assert(tasn1_get_type(buf) == TASN1_ARRAY);

    TASN1_ITERATOR(iter);

    assert(tasn1_iterator_set(&iter, buf) == 0);
    assert(tasn1_iterator_get(&iter) == NULL);

    const TASN1_OCTET_T *pb;
    tasn1_node_t *container;

    // Try it with one entry:
    container = tasn1_new_array();
    assert(container);
    node = tasn1_new_string(test_string, true);
    assert(node);
    assert(tasn1_add_array_value(container, node) == 0);
    n = tasn1_serialize(container, buf, BUF_S);
    assert(n > 0);
    tasn1_free(container);
    assert(tasn1_iterator_set(&iter, buf) == 0);
    pb = tasn1_iterator_get(&iter);
    assert(pb);
    astring = tasn1_get_string(pb);
    assert(astring);
    assert(strcmp(astring, test_string) == 0);
    pb = tasn1_iterator_get(&iter);
    assert(!pb);

    // Try it with three entries:
    container = tasn1_new_array();
    assert(container);
    node = tasn1_new_string(test_string, false);
    assert(node);
    assert(tasn1_add_array_value(container, node) == 0);
    node = tasn1_new_array();
    assert(node);
    assert(tasn1_add_array_value(container, node) == 0);
    node = tasn1_new_map();
    assert(node);
    assert(tasn1_add_array_value(container, node) == 0);
    n = tasn1_serialize(container, buf, BUF_S);
    assert(n > 0);
    tasn1_free(container);
    assert(tasn1_iterator_set(&iter, buf) == 0);
    pb = tasn1_iterator_get(&iter);
    assert(pb);
    astring = tasn1_get_string(pb);
    assert(astring);
    assert(strcmp(astring, test_string) == 0);
    pb = tasn1_iterator_get(&iter);
    assert(pb);
    assert(tasn1_get_type(pb) == TASN1_ARRAY);
    pb = tasn1_iterator_get(&iter);
    assert(pb);
    assert(tasn1_get_type(pb) == TASN1_MAP);
    pb = tasn1_iterator_get(&iter);
    assert(!pb);

    //// Test maps ////

    tasn1_node_t *map1 = tasn1_new_map();
    assert(map1);

    erc = tasn1_add_map_string(map1, "KEY1", false, tasn1_new_string("VAL1", false));
    assert(erc == 0);

    erc = tasn1_add_map_string(map1, "KEY2", false, tasn1_new_bool(true));
    assert(erc == 0);

    int size1 = tasn1_size(map1);
    assert(size1 > 0);
#ifdef DUMP
    printf("Size = %i\n", size1);
#endif

    TASN1_OCTET_T buf1[10];
    erc = tasn1_serialize(map1, buf1, sizeof(buf1));
#ifdef DUMP
    printf("erc = %i: %s\n", erc, strerror(-erc));
#endif
    assert(erc < 0);

    TASN1_OCTET_T buf2[40];
    int size2 = tasn1_serialize(map1, buf2, sizeof(buf2));
    assert(size2 > 0);
#ifdef DUMP
    dump(buf2, size2);
#endif
    assert(size2 == size1);

    tasn1_free(map1);

    TASN1_ITERATOR(child_iter);

    assert(tasn1_get_type(buf2) == TASN1_MAP);
    assert(tasn1_iterator_set(&iter, buf2) == 0);
    // Key 1:
    pb = tasn1_iterator_get(&iter);
    assert(pb);
    assert(tasn1_get_type(pb) == TASN1_ARRAY);
    assert(tasn1_iterator_set(&child_iter, pb) == 0);
    pb = tasn1_iterator_get(&child_iter);
    assert(pb);
    assert(tasn1_get_type(pb) == TASN1_OCTET_SEQUENCE);
    astring = tasn1_get_string(pb);
    assert(astring);
    assert(strcmp(astring, "KEY1") == 0);
    // Value 1:
    pb = tasn1_iterator_get(&child_iter);
    assert(pb);
    assert(tasn1_get_type(pb) == TASN1_OCTET_SEQUENCE);
    astring = tasn1_get_string(pb);
    assert(astring);
    assert(strcmp(astring, "VAL1") == 0);
    // EOF:
    assert(!tasn1_iterator_get(&child_iter));

    // Key 2:
    pb = tasn1_iterator_get(&iter);
    assert(pb);
    assert(tasn1_get_type(pb) == TASN1_ARRAY);
    assert(tasn1_iterator_set(&child_iter, pb) == 0);
    pb = tasn1_iterator_get(&child_iter);
    assert(pb);
    assert(tasn1_get_type(pb) == TASN1_OCTET_SEQUENCE);
    astring = tasn1_get_string(pb);
    assert(astring);
    assert(strcmp(astring, "KEY2") == 0);
    // Value 2:
    pb = tasn1_iterator_get(&child_iter);
    assert(pb);
    assert(tasn1_get_type(pb) == TASN1_NUMBER);
    number = 7;
    assert(tasn1_get_number(pb, &number) == 0);
    assert(number == 1);
    // EOF:
    assert(!tasn1_iterator_get(&child_iter));

    // EOF:
    assert(!tasn1_iterator_get(&iter));
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

#ifdef DUMP
    dump(buffer.data(), buffer.size());
#endif

    json x9 = jarray({
        false,
        1,
        jobject({
            jitem("First",99),
            jitem("Second","Blub"),
            jitem("Third", true)
        }),
        "Bla"
    });

    Node n1 = Node::fromJson(x9);
    n1.serialize(buffer);

#ifdef DUMP
    dump(buffer.data(), buffer.size());
#endif
}

int main() {
    printf("Running C tests ...\n");
    c_tests();
    printf("Running C++ tests ...\n");
    cpp_tests();
    printf("Success!\n");
    return EXIT_SUCCESS;
}
