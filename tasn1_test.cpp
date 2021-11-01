
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
#include <tisslib.hpp>

using namespace std;
using namespace jsonx;
using namespace tasn1;

#ifdef DUMP
static void dump(TASN1_OCTET_T *pb, int cb) {
    const int l = 16;
    int i, k = 0;
    while (k < cb) {
        i = k;
        printf("\n%04i: |", k);
        for (int j = 0; ((i < cb) && (j < l)); ++i, ++j) {
            TASN1_OCTET_T o = pb[i];
            printf("%02x|", o);
        }
        i = k;
        printf("\n      |");
        for (int j = 0; ((i < cb) && (j < l)); ++i, ++j) {
            int c = pb[i];
            if (isprint(c))
                printf("%1c |", c);
            else
                printf(". |");
        } // end for //
        k += l;
    } // end while //
    printf("\n");
}
#endif

static tasn1::node_ptr_t ArrayOfByte(const uint8_t *pb, size_t cb) {
    auto *rg{new tasn1::Array()};
    while (cb-- > 0)
        rg->add(*pb++);
    return tasn1::node_ptr_t(rg);
}

static void handleCallDISC()
{
    uint8_t adr[2] = { 208, 1 };
    uint8_t mac[4] = { 0x40, 0x00, 0xf1, 0x6e };
    uint8_t groups[] = { 1, 3, 4 };

    vector<uint8_t> telegram{};
    telegram.push_back(0); // Master ID - Admin
    telegram.push_back(1); // Submaster ID - Call operator
    telegram.push_back(0b00000100); // Response, UI2

    tasn1::Map response;
    // request type
    response.add("requ", tiss::UI2_DISC);

    // List of services:
    auto *data{new tasn1::Array()};

    // User manager:
    auto *user_manager{new tasn1::Map()};
    user_manager->add("type", 1);
    data->add(tasn1::node_ptr_t(user_manager));

    // TCU:
    auto *tcu{new tasn1::Map()};
    tcu->add("type", 1);
    tcu->add("adr", ArrayOfByte(adr, sizeof(adr)));
    tcu->add("mac", ArrayOfByte(mac, sizeof(mac)));
    tcu->add("groups", ArrayOfByte(groups, sizeof(groups)));
    data->add(tasn1::node_ptr_t(tcu));

    response.add("data", tasn1::node_ptr_t(data));

    vector<TASN1_OCTET_T> payload;
    response.serialize(payload);
#ifdef DUMP
    cout << "payload:";
    dump(payload.data(), payload.size());
#endif
    json j = Node::toJson(payload.data(), payload.size());
#ifdef DUMP
    cout << "JSON> " << j << endl;
#endif

    telegram.insert(end(telegram), begin(payload), end(payload));
    //transmit(telegram.data(), telegram.size());
#ifdef DUMP
    cout << endl << "telegram:";
    dump(telegram.data(), telegram.size());
#endif
}

#define BUF_S 1024

static void c_static_tests() {
    int erc, n;
    TASN1_NUMBER_T number;
    TASN1_OCTET_T buf[BUF_S];
    TASN1_OCTET_T buf1[10];
    TASN1_OCTET_T buf2[40];

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

#ifdef DUMP
    printf("\n----> String:\n");
    dump(buf, n);
#endif

    astring = tasn1_get_string(buf);
    assert(astring);
    assert(strcmp(astring, test_string) == 0);

    //// Test arrays ////

    array_t s_array_1;
    tasn1_init_array(&s_array_1);

    n = tasn1_serialize(&s_array_1.node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_size(&s_array_1.node) == 1);
    assert(tasn1_get_type(buf) == TASN1_ARRAY);

    TASN1_ITERATOR(iter);

    assert(tasn1_iterator_set(&iter, buf) == 0);
    assert(tasn1_iterator_get(&iter) == NULL);

    const TASN1_OCTET_T *pb;
    array_t s_array_2;
    tasn1_init_array(&s_array_2);

    octet_sequence_t s_os;
    tasn1_init_string(&s_os, test_string);
    // Try it with one entry:
    assert(tasn1_add_array_value(&s_array_2, &s_os.node) == 0);
    n = tasn1_serialize(&s_array_2.node, buf, BUF_S);
    assert(n > 0);

#ifdef DUMP
    printf("\n----> One entry:\n");
    dump(buf, n);
#endif

    assert(tasn1_iterator_set(&iter, buf) == 0);
    pb = tasn1_iterator_get(&iter);
    assert(pb);
    astring = tasn1_get_string(pb);
    assert(astring);
    assert(strcmp(astring, test_string) == 0);
    pb = tasn1_iterator_get(&iter);
    assert(!pb);

    // Try it with three entries:
    tasn1_array_reset(&s_array_2);

    octet_sequence_t e1;
    tasn1_init_string(&e1, test_string);
    assert(tasn1_add_array_value(&s_array_2, &e1.node) == 0);

    array_t e2;
    tasn1_init_array(&e2);
    assert(tasn1_add_array_value(&s_array_2, &e2.node) == 0);

    map_t e3;
    tasn1_init_map(&e3);
    assert(tasn1_add_array_value(&s_array_2, &e3.node) == 0);

    n = tasn1_serialize(&s_array_2.node, buf, BUF_S);
    assert(n > 0);

#ifdef DUMP
    printf("\n----> Three entries:\n");
    dump(buf, n);
#endif

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
    erc = tasn1_map_add_item(&map1, &item1);
    assert(erc == 0);

    octet_sequence_t key2;
    tasn1_init_string(&key2, "KEY2");
    number_t val2;
    tasn1_init_number(&val2, true);
    item_t item2;
    tasn1_init_item(&item2, &key2.node, &val2.node);
    erc = tasn1_map_add_item(&map1, &item2);
    assert(erc == 0);

    int size1 = tasn1_size(&map1.node);
    assert(size1 > 0);
#ifdef DUMP
    printf("Size = %i\n", size1);
#endif

    erc = tasn1_serialize(&map1.node, buf1, sizeof(buf1));
    //printf("erc = %i: %s\n", erc, strerror(-erc));
    assert(erc == -12);

    int size2 = tasn1_serialize(&map1.node, buf2, sizeof(buf2));
    assert(size2 > 0);
#ifdef DUMP
    printf("\n----> Map KEY1:VAL1 KEY2:true:\n");
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

static void c_dynamic_tests(tasn1_allocator_t *alloc) {
    int erc, n;
    TASN1_NUMBER_T number;
    TASN1_OCTET_T buf[BUF_S];
    TASN1_OCTET_T buf1[10];
    TASN1_OCTET_T buf2[40];

    assert(tasn1_get_type(NULL) == TASN1_INVALID);

    //// Test numbers ////

    number_t *s_number = tasn1_new_number(alloc, SHRT_MIN);
    assert(s_number);
    assert(tasn1_size(&s_number->node) == 3);
    n = tasn1_serialize(&s_number->node, buf, BUF_S);
    assert(n == 3);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == SHRT_MIN);

    tasn1_reset_number(s_number, -1234);
    assert(tasn1_size(&s_number->node) == 3);
    n = tasn1_serialize(&s_number->node, buf, BUF_S);
    assert(n == 3);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == -1234);

    tasn1_reset_number(s_number, SCHAR_MIN);
    assert(tasn1_size(&s_number->node) == 2);
    n = tasn1_serialize(&s_number->node, buf, BUF_S);
    assert(n == 2);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == SCHAR_MIN);

    tasn1_reset_number(s_number, -56);
    assert(tasn1_size(&s_number->node) == 2);
    n = tasn1_serialize(&s_number->node, buf, BUF_S);
    assert(n == 2);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == -56);

    tasn1_reset_number(s_number, -16);
    assert(tasn1_size(&s_number->node) == 1);
    n = tasn1_serialize(&s_number->node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == -16);

    tasn1_reset_number(s_number, -9);
    assert(tasn1_size(&s_number->node) == 1);
    n = tasn1_serialize(&s_number->node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == -9);

    tasn1_reset_number(s_number, 0);
    assert(tasn1_size(&s_number->node) == 1);
    n = tasn1_serialize(&s_number->node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == 0);

    tasn1_reset_number(s_number, 11);
    assert(tasn1_size(&s_number->node) == 1);
    n = tasn1_serialize(&s_number->node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == 11);

    tasn1_reset_number(s_number, 15);
    assert(tasn1_size(&s_number->node) == 1);
    n = tasn1_serialize(&s_number->node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == 15);

    tasn1_reset_number(s_number, 99);
    assert(tasn1_size(&s_number->node) == 2);
    n = tasn1_serialize(&s_number->node, buf, BUF_S);
    assert(n == 2);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == 99);

    tasn1_reset_number(s_number, SCHAR_MAX);
    assert(tasn1_size(&s_number->node) == 2);
    n = tasn1_serialize(&s_number->node, buf, BUF_S);
    assert(n == 2);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == SCHAR_MAX);

    tasn1_reset_number(s_number, 5678);
    assert(tasn1_size(&s_number->node) == 3);
    n = tasn1_serialize(&s_number->node, buf, BUF_S);
    assert(n == 3);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == 5678);

    tasn1_reset_number(s_number, SHRT_MAX);
    assert(tasn1_size(&s_number->node) == 3);
    n = tasn1_serialize(&s_number->node, buf, BUF_S);
    assert(n == 3);
    assert(tasn1_get_type(buf) == TASN1_NUMBER);
    assert(tasn1_get_number(buf, &number) == 0);
    assert(number == SHRT_MAX);

    tasn1_del_number(alloc, s_number);

    //// Test octet sequence ////

    const TASN1_OCTET_T *po;
    TASN1_SIZE_T         co;

    octet_sequence_t *s_node = tasn1_new_octet_sequence(alloc, buf, 0, false);
    assert(tasn1_size(&s_node->node) == 1);
    n = tasn1_serialize(&s_node->node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_get_type(buf) == TASN1_OCTET_SEQUENCE);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 1);
    assert(co == 0);

    const TASN1_OCTET_T os_1[] { 0x01, 0x02, 0x03, 0x04 };
    tasn1_reset_octet_sequence(s_node, os_1, sizeof(os_1));
    assert(tasn1_size(&s_node->node) == sizeof(os_1) + 1);
    n = tasn1_serialize(&s_node->node, buf, BUF_S);
    assert(n == sizeof(os_1) + 1);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 1);
    assert(co == sizeof(os_1));
    assert(memcmp(os_1, po, co) == 0);

    const TASN1_OCTET_T os_2[] { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                                 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                                 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e        };
    tasn1_reset_octet_sequence(s_node, os_2, sizeof(os_2));
    assert(tasn1_size(&s_node->node) == sizeof(os_2) + 1);
    n = tasn1_serialize(&s_node->node, buf, BUF_S);
    assert(n == sizeof(os_2) + 1);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 1);
    assert(co == sizeof(os_2));
    assert(memcmp(os_2, po, co) == 0);

    const TASN1_OCTET_T os_3[] { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                                 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                                 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
    tasn1_reset_octet_sequence(s_node, os_3, sizeof(os_3));
    assert(tasn1_size(&s_node->node) == sizeof(os_3) + 2);
    n = tasn1_serialize(&s_node->node, buf, BUF_S);
    assert(n == sizeof(os_3) + 2);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 2);
    assert(co == sizeof(os_3));
    assert(memcmp(os_3, po, co) == 0);

    TASN1_OCTET_T os_4[BUF_S];
    for (size_t i = 0; i < 256; os_4[i] = i ^ 0x55, ++i);

    tasn1_reset_octet_sequence(s_node, os_4, 255);
    assert(tasn1_size(&s_node->node) == 255 + 2);
    n = tasn1_serialize(&s_node->node, buf, BUF_S);
    assert(n == 255 + 2);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 2);
    assert(co == 255);
    assert(memcmp(os_4, po, co) == 0);

    os_4[256] = 0xf3;

    tasn1_reset_octet_sequence(s_node, os_4, 256);
    assert(tasn1_size(&s_node->node) == 256 + 3);
    n = tasn1_serialize(&s_node->node, buf, BUF_S);
    assert(n == 256 + 3);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 3);
    assert(co == 256);
    assert(memcmp(os_4, po, co) == 0);

    for (size_t i = 0; i < 1004; os_4[i] = i & 0x55, ++i);

    tasn1_reset_octet_sequence(s_node, os_4, 1004);
    assert(tasn1_size(&s_node->node) == 1004 + 3);
    n = tasn1_serialize(&s_node->node, buf, BUF_S);
    assert(n == 1004 + 3);
    assert(tasn1_get_octetsequence(buf, &po, &co) == 0);
    assert(po == buf + 3);
    assert(co == 1004);
    assert(memcmp(os_4, po, co) == 0);

    const char *astring;

    tasn1_reset_string(s_node, "");
    assert(tasn1_size(&s_node->node) == 2);
    n = tasn1_serialize(&s_node->node, buf, BUF_S);
    assert(n == 2);
    astring = tasn1_get_string(buf);
    assert(astring);
    assert(strcmp(astring, "") == 0);

    const char *test_string = "The quick brown fox jumps over the lazy dog, 1234567890";

    tasn1_reset_string(s_node, test_string);
    assert(tasn1_size(&s_node->node) == (int)strlen(test_string) + 3);
    n = tasn1_serialize(&s_node->node, buf, BUF_S);
    assert(n == (int)strlen(test_string) + 3);

#ifdef DUMP
    printf("\n----> String:\n");
    dump(buf, n);
#endif

    astring = tasn1_get_string(buf);
    assert(astring);
    assert(strcmp(astring, test_string) == 0);

    tasn1_del_octet_sequence(alloc, s_node);

    //// Test arrays ////

    array_t *s_array_1 = tasn1_new_array(alloc);
    assert(s_array_1);

    n = tasn1_serialize(&s_array_1->node, buf, BUF_S);
    assert(n == 1);
    assert(tasn1_size(&s_array_1->node) == 1);
    assert(tasn1_get_type(buf) == TASN1_ARRAY);

    tasn1_del_array(alloc, s_array_1);

    tasn1_iterator_t *iter = tasn1_new_iterator(alloc);

    assert(tasn1_iterator_set(iter, buf) == 0);
    assert(tasn1_iterator_get(iter) == NULL);

    const TASN1_OCTET_T *pb;
    array_t *s_array_2 = tasn1_new_array(alloc);
    assert(s_array_2);

    octet_sequence_t *s_os = tasn1_new_string(alloc, test_string, true);
    // Try it with one entry:
    assert(tasn1_add_array_value(s_array_2, &s_os->node) == 0);
    n = tasn1_serialize(&s_array_2->node, buf, BUF_S);
    assert(n > 0);
    tasn1_del_array(alloc, s_array_2);

#ifdef DUMP
    printf("\n----> One entry:\n");
    dump(buf, n);
#endif

    assert(tasn1_iterator_set(iter, buf) == 0);
    pb = tasn1_iterator_get(iter);
    assert(pb);
    astring = tasn1_get_string(pb);
    assert(astring);
    assert(strcmp(astring, test_string) == 0);
    pb = tasn1_iterator_get(iter);
    assert(!pb);

    // Try it with three entries:
    array_t *s_array = tasn1_new_array(alloc);
    assert(s_array);

    octet_sequence_t *e1 = tasn1_new_string(alloc, test_string, true);
    assert(tasn1_add_array_value(s_array_2, &e1->node) == 0);

    array_t *e2 = tasn1_new_array(alloc);
    assert(tasn1_add_array_value(s_array_2, &e2->node) == 0);

    map_t *e3 = tasn1_new_map(alloc);
    assert(tasn1_add_array_value(s_array_2, &e3->node) == 0);

    n = tasn1_serialize(&s_array_2->node, buf, BUF_S);
    assert(n > 0);

    tasn1_del_array(alloc, s_array_2);

#ifdef DUMP
    printf("\n----> Three entries:\n");
    dump(buf, n);
#endif

    assert(tasn1_iterator_set(iter, buf) == 0);
    pb = tasn1_iterator_get(iter);
    assert(pb);
    astring = tasn1_get_string(pb);
    assert(astring);
    assert(strcmp(astring, test_string) == 0);
    pb = tasn1_iterator_get(iter);
    assert(pb);
    assert(tasn1_get_type(pb) == TASN1_ARRAY);
    pb = tasn1_iterator_get(iter);
    assert(pb);
    assert(tasn1_get_type(pb) == TASN1_MAP);
    pb = tasn1_iterator_get(iter);
    assert(!pb);

    //// Test maps ////

    map_t *map1 = tasn1_new_map(alloc);

    octet_sequence_t *key1 = tasn1_new_string(alloc, "KEY1", false);
    octet_sequence_t *val1 = tasn1_new_string(alloc, "VAL1", false);
    item_t *item1 = tasn1_new_item(alloc, &key1->node, &val1->node);
    erc = tasn1_map_add_item(map1, item1);
    assert(erc == 0);

    octet_sequence_t *key2 = tasn1_new_string(alloc, "KEY2", false);
    number_t *val2 = tasn1_new_number(alloc, true);
    item_t *item2 = tasn1_new_item(alloc, &key2->node, &val2->node);
    erc = tasn1_map_add_item(map1, item2);
    assert(erc == 0);

    int size1;
    size1 = tasn1_size(&map1->node);
    assert(size1 > 0);
#ifdef DUMP
    printf("Size = %i\n", size1);
#endif

    erc = tasn1_serialize(&map1->node, buf1, sizeof(buf1));
    //printf("erc = %i: %s\n", erc, strerror(-erc));
    assert(erc == -12);

    int size2 = tasn1_serialize(&map1->node, buf2, sizeof(buf2));
    assert(size2 > 0);
#ifdef DUMP
    printf("\n----> Map KEY1:VAL1 KEY2:true:\n");
    dump(buf2, size2);
#endif
    assert(size2 == size1);

    tasn1_del_map(alloc, map1);

    tasn1_iterator_t *child_iter = tasn1_new_iterator(alloc);

    assert(tasn1_get_type(buf2) == TASN1_MAP);
    assert(tasn1_iterator_set(iter, buf2) == 0);
    // Key 1:
    pb = tasn1_iterator_get(iter);
    assert(pb);
    assert(tasn1_get_type(pb) == TASN1_ARRAY);
    assert(tasn1_iterator_set(child_iter, pb) == 0);
    pb = tasn1_iterator_get(child_iter);
    assert(pb);
    assert(tasn1_get_type(pb) == TASN1_OCTET_SEQUENCE);
    astring = tasn1_get_string(pb);
    assert(astring);
    assert(strcmp(astring, "KEY1") == 0);
    // Value 1:
    pb = tasn1_iterator_get(child_iter);
    assert(pb);
    assert(tasn1_get_type(pb) == TASN1_OCTET_SEQUENCE);
    astring = tasn1_get_string(pb);
    assert(astring);
    assert(strcmp(astring, "VAL1") == 0);
    // EOF:
    assert(!tasn1_iterator_get(child_iter));

    // Key 2:
    pb = tasn1_iterator_get(iter);
    assert(pb);
    assert(tasn1_get_type(pb) == TASN1_ARRAY);
    assert(tasn1_iterator_set(child_iter, pb) == 0);
    pb = tasn1_iterator_get(child_iter);
    assert(pb);
    assert(tasn1_get_type(pb) == TASN1_OCTET_SEQUENCE);
    astring = tasn1_get_string(pb);
    assert(astring);
    assert(strcmp(astring, "KEY2") == 0);
    // Value 2:
    pb = tasn1_iterator_get(child_iter);
    assert(pb);
    assert(tasn1_get_type(pb) == TASN1_NUMBER);
    number = 7;
    assert(tasn1_get_number(pb, &number) == 0);
    assert(number == 1);
    // EOF:
    assert(!tasn1_iterator_get(child_iter));
    tasn1_del_iterator(alloc, child_iter);

    // EOF:
    assert(!tasn1_iterator_get(iter));
    tasn1_del_iterator(alloc, iter);
}

static void cpp_tests() {
    tasn1::vector_t buffer;

    {
        Array rg1;
        for (int i = -100; i <= 100; ++i) {
            rg1.add(i);
        }
        rg1.serialize(buffer);
#ifdef DUMP
        printf("\n----> for i = -100..100\n");
        dump(buffer.data(), buffer.size());
#endif
        json j = Node::toJson(buffer.data(), buffer.size());
#ifdef DUMP
        cout << "JSON> " << j << endl;
#endif
        json k = jsonx::jarray({
            -100,-99,-98,-97,-96,-95,-94,-93,-92,-91,-90,-89,-88,-87,-86,-85,-84,-83,-82,-81,
             -80,-79,-78,-77,-76,-75,-74,-73,-72,-71,-70,-69,-68,-67,-66,-65,-64,-63,-62,-61,
             -60,-59,-58,-57,-56,-55,-54,-53,-52,-51,-50,-49,-48,-47,-46,-45,-44,-43,-42,-41,
             -40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-25,-24,-23,-22,-21,
             -20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10, -9, -8, -7, -6, -5, -4, -3, -2, -1,
               0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
              20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
              40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
              60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
              80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
             100});
        assert(k == j);
    }

    {
        Array rg1;
        rg1.add("VAL1");
        rg1.add("VAL2");
        rg1.add("VAL3");
        rg1.serialize(buffer);
#ifdef DUMP
        printf("\n----> Array VAL1 VAL2 VAL3:\n");
        dump(buffer.data(), buffer.size());
#endif
        json j = Node::toJson(buffer.data(), buffer.size());
#ifdef DUMP
        cout << "JSON> " << j << endl;
#endif
        json k = jsonx::jarray({"VAL1","VAL2","VAL3"});
        assert(j == k);
    }

    {
        Map map1;
        map1.add("KEY1", "VAL1");
        map1.add("KEY2", true);
        map1.add("KEY3", 1234);
        map1.serialize(buffer);
#ifdef DUMP
        printf("\n----> Map KEY1:VAL1 KEY2:true KEY3:1234:\n");
        dump(buffer.data(), buffer.size());
#endif
        json j = Node::toJson(buffer.data(), buffer.size());
#ifdef DUMP
        cout << "JSON> " << j << endl;
#endif
        json k = jobject({
                     jitem("KEY1","VAL1"),
                     jitem("KEY2",static_cast<TASN1_NUMBER_T>(1)),
                     jitem("KEY3",static_cast<TASN1_NUMBER_T>(1234))
                 });
        assert(j == k);
    }

    {
        json x9 = jarray({
            static_cast<TASN1_NUMBER_T>(false),
            static_cast<TASN1_NUMBER_T>(1),
            jobject({
                jitem("First",  static_cast<TASN1_NUMBER_T>(99)),
                jitem("Second", "Blub"),
                jitem("Third",  static_cast<TASN1_NUMBER_T>(true))
            }),
            "Bla"
        });
        auto n1 = Node::fromJson(x9);
        n1->serialize(buffer);
#ifdef DUMP
        printf("\n----> Map false, 1, { First:99, Second:Blub, Third:true } Bla:\n");
        dump(buffer.data(), buffer.size());
#endif
        json j = Node::toJson(buffer.data(), buffer.size());
#ifdef DUMP
        cout << "JSON> " << j << endl;
#endif
        assert(j == x9);
    }
}

int main() {
    printf("\nRunning C static tests ...\n");
    c_static_tests();

    printf("\nRunning C dynamic tests with buffer allocator ...\n");
    vector<uint8_t> buffer(1024);
    set_buffer_allocator_buffer(buffer.data(), buffer.size());
    c_dynamic_tests(buffer_allocator());

    printf("\nRunning C dynamic tests with system heap ...\n");
    tasn1_allocator_t alloc1 = { .f_alloc = malloc, .f_free = free };
    c_dynamic_tests(&alloc1);

    printf("\nRunning C++ tests ...\n");
    cpp_tests();

    printf("\nRunning Special test for android ...\n");
    handleCallDISC();
    printf("\nSuccess!\n");
    return EXIT_SUCCESS;
}
