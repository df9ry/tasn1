
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
#include <clist.h>

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
    TASN1_OCTET_T buf[BUF_S];

    assert(tasn1_get_type(NULL) == TASN1_INVALID);

    //// Test numbers ////

    number_t s_number;
    tasn1_init_number(&s_number, SHRT_MIN);
    assert(tasn1_size(&s_number.node) == 3);
    n = tasn1_serialize(&s_number.node, buf, BUF_S);
    assert(n == 3);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == SHRT_MIN);

    tasn1_reset_number(&s_number, -1234);
    assert(tasn1_size(&s_number.node) == 3);
    n = tasn1_serialize(&s_number.node, buf, BUF_S);
    assert(n == 3);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == -1234);

    tasn1_reset_number(&s_number, SCHAR_MIN);
    assert(tasn1_size(&s_number.node) == 2);
    n = tasn1_serialize(&s_number.node, buf, BUF_S);
    assert(n == 2);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == SCHAR_MIN);

    tasn1_reset_number(&s_number, -56);
    assert(tasn1_size(&s_number.node) == 2);
    n = tasn1_serialize(&s_number.node, buf, BUF_S);
    assert(n == 2);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == -56);

    tasn1_reset_number(&s_number, -16);
    assert(tasn1_size(&s_number.node) == 1);
    n = tasn1_serialize(&s_number.node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == -16);

    tasn1_reset_number(&s_number, -9);
    assert(tasn1_size(&s_number.node) == 1);
    n = tasn1_serialize(&s_number.node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == -9);

    tasn1_reset_number(&s_number, 0);
    assert(tasn1_size(&s_number.node) == 1);
    n = tasn1_serialize(&s_number.node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == 0);

    tasn1_reset_number(&s_number, 11);
    assert(tasn1_size(&s_number.node) == 1);
    n = tasn1_serialize(&s_number.node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == 11);

    tasn1_reset_number(&s_number, 15);
    assert(tasn1_size(&s_number.node) == 1);
    n = tasn1_serialize(&s_number.node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == 15);

    tasn1_reset_number(&s_number, 99);
    assert(tasn1_size(&s_number.node) == 2);
    n = tasn1_serialize(&s_number.node, buf, BUF_S);
    assert(n == 2);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == 99);

    tasn1_reset_number(&s_number, SCHAR_MAX);
    assert(tasn1_size(&s_number.node) == 2);
    n = tasn1_serialize(&s_number.node, buf, BUF_S);
    assert(n == 2);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == SCHAR_MAX);

    tasn1_reset_number(&s_number, 5678);
    assert(tasn1_size(&s_number.node) == 3);
    n = tasn1_serialize(&s_number.node, buf, BUF_S);
    assert(n == 3);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == 5678);

    tasn1_reset_number(&s_number, SHRT_MAX);
    assert(tasn1_size(&s_number.node) == 3);
    n = tasn1_serialize(&s_number.node, buf, BUF_S);
    assert(n == 3);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == SHRT_MAX);

    //// Test octet sequence ////

#ifdef HAVE_HEAP
    node = tasn1_new_octet_sequence(NULL, 0, false);
    assert(!node);
#endif

#ifdef HAVE_HEAP
    node = tasn1_new_octet_sequence(NULL, 0, true);
    assert(!node);
#endif

    const TASN1_OCTET_T *po;
    TASN1_SIZE_T         co;

    octet_sequence_t s_node;
    tasn1_init_octet_sequence(&s_node, buf, 0);
    assert(tasn1_size(&s_node.node) == 1);
    n = tasn1_serialize(&s_node.node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_OCTET_SEQUENCE);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 1);
    assert(co == 0);

    const TASN1_OCTET_T os_1[] { 0x01, 0x02, 0x03, 0x04 };
    tasn1_reset_octet_sequence(&s_node, os_1, sizeof(os_1));
    assert(tasn1_size(&s_node.node) == sizeof(os_1) + 1);
    n = tasn1_serialize(&s_node.node, buf, BUF_S);
    assert(n == sizeof(os_1) + 1);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 1);
    assert(co == sizeof(os_1));
    assert(memcmp(os_1, po, co) == 0);

#ifdef HAVE_HEAP
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
#endif

    const TASN1_OCTET_T os_2[] { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                                 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                                 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e        };
    tasn1_reset_octet_sequence(&s_node, os_2, sizeof(os_2));
    assert(tasn1_size(&s_node.node) == sizeof(os_2) + 1);
    n = tasn1_serialize(&s_node.node, buf, BUF_S);
    assert(n == sizeof(os_2) + 1);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 1);
    assert(co == sizeof(os_2));
    assert(memcmp(os_2, po, co) == 0);

    const TASN1_OCTET_T os_3[] { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                                 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                                 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
    tasn1_reset_octet_sequence(&s_node, os_3, sizeof(os_3));
    assert(tasn1_size(&s_node.node) == sizeof(os_3) + 2);
    n = tasn1_serialize(&s_node.node, buf, BUF_S);
    assert(n == sizeof(os_3) + 2);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 2);
    assert(co == sizeof(os_3));
    assert(memcmp(os_3, po, co) == 0);

    TASN1_OCTET_T os_4[BUF_S];
    for (size_t i = 0; i < 256; os_4[i] = i ^ 0x55, ++i);

    tasn1_reset_octet_sequence(&s_node, os_4, 255);
    assert(tasn1_size(&s_node.node) == 255 + 2);
    n = tasn1_serialize(&s_node.node, buf, BUF_S);
    assert(n == 255 + 2);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 2);
    assert(co == 255);
    assert(memcmp(os_4, po, co) == 0);

    os_4[256] = 0xf3;

    tasn1_reset_octet_sequence(&s_node, os_4, 256);
    assert(tasn1_size(&s_node.node) == 256 + 3);
    n = tasn1_serialize(&s_node.node, buf, BUF_S);
    assert(n == 256 + 3);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 3);
    assert(co == 256);
    assert(memcmp(os_4, po, co) == 0);

    for (size_t i = 0; i < 1004; os_4[i] = i & 0x55, ++i);

    tasn1_reset_octet_sequence(&s_node, os_4, 1004);
    assert(tasn1_size(&s_node.node) == 1004 + 3);
    n = tasn1_serialize(&s_node.node, buf, BUF_S);
    assert(n == 1004 + 3);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 3);
    assert(co == 1004);
    assert(memcmp(os_4, po, co) == 0);

    const char *astring;

    tasn1_reset_string(&s_node, "");
    assert(tasn1_size(&s_node.node) == 2);
    n = tasn1_serialize(&s_node.node, buf, BUF_S);
    assert(n == 2);
    astring = tasn1_get_string(buf);
    assert(astring);
    assert(strcmp(astring, "") == 0);

    const char *test_string = "The quick brown fox jumps over the lazy dog, 1234567890";

    tasn1_reset_string(&s_node, test_string);
    assert(tasn1_size(&s_node.node) == (int)strlen(test_string) + 3);
    n = tasn1_serialize(&s_node.node, buf, BUF_S);
    assert(n == (int)strlen(test_string) + 3);
    astring = tasn1_get_string(buf);
    assert(astring);
    assert(strcmp(astring, test_string) == 0);

    //// Test arrays ////

    array_t s_array;
    tasn1_init_array(&s_array);

    n = tasn1_serialize(&s_array.node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_size(&s_array.node) == 1);
    assert(tasn1_get_type(buf) == TASN1_ARRAY);

    TASN1_ITERATOR(iter);

    assert(tasn1_iterator_set(&iter, buf) == 0);
    assert(tasn1_iterator_get(&iter) == NULL);

    const TASN1_OCTET_T *pb;
    array_t container;
    tasn1_init_array(&container);

    octet_sequence_t s_os;
    tasn1_init_string(&s_os, test_string);
    // Try it with one entry:
    assert(tasn1_add_array_value(&container.node, &s_os.node) == 0);
    n = tasn1_serialize(&container.node, buf, BUF_S);
    assert(n > 0);
    assert(tasn1_iterator_set(&iter, buf) == 0);
    pb = tasn1_iterator_get(&iter);
    assert(pb);
    astring = tasn1_get_string(pb);
    assert(astring);
    assert(strcmp(astring, test_string) == 0);
    pb = tasn1_iterator_get(&iter);
    assert(!pb);

    // Try it with three entries:
    tasn1_array_reset(&container);

    octet_sequence_t e1;
    tasn1_init_string(&e1, test_string);
    assert(tasn1_add_array_value(&container.node, &e1.node) == 0);

    array_t e2;
    tasn1_init_array(&e2);
    assert(tasn1_add_array_value(&container.node, &e2.node) == 0);

    map_t e3;
    tasn1_init_map(&e3);
    assert(tasn1_add_array_value(&container.node, &e3.node) == 0);

    n = tasn1_serialize(&container.node, buf, BUF_S);
    assert(n > 0);

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

    map_t map1;
    tasn1_init_map(&map1);

    octet_sequence_t key1, val1;
    tasn1_init_string(&key1, "KEY1");
    tasn1_init_string(&val1, "VAL1");
    item_t item1;
    tasn1_init_item(&item1, &key1.node, &val1.node);
    erc = tasn1_map_add_item(&map1.node, &item1);
    assert(erc == 0);

    octet_sequence_t key2;
    tasn1_init_string(&key2, "KEY2");
    number_t val2;
    tasn1_init_number(&val2, true);
    item_t item2;
    tasn1_init_item(&item2, &key2.node, &val2.node);
    erc = tasn1_map_add_item(&map1.node, &item2);
    assert(erc == 0);

    int size1 = tasn1_size(&map1.node);
    assert(size1 > 0);
#ifdef DUMP
    printf("Size = %i\n", size1);
#endif

    TASN1_OCTET_T buf1[10];
    erc = tasn1_serialize(&map1.node, buf1, sizeof(buf1));
#ifdef DUMP
    printf("erc = %i: %s\n", erc, strerror(-erc));
#endif
    assert(erc < 0);

    TASN1_OCTET_T buf2[40];
    int size2 = tasn1_serialize(&map1.node, buf2, sizeof(buf2));
    assert(size2 > 0);
#ifdef DUMP
    dump(buf2, size2);
#endif
    assert(size2 == size1);

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
