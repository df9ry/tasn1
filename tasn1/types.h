#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <clist.h>

#define TASN1_OCTET_T  uint8_t
#define TASN1_NUMBER_T int16_t
#define TASN1_SIZE_T   uint16_t

/**
 * @brief Datatype of a asn1_node.
 */
enum tasn1_type {
    TASN1_INVALID        = -1,
    TASN1_MAP            =  0,
    TASN1_ARRAY          =  1,
    TASN1_OCTET_SEQUENCE =  2,
    TASN1_NUMBER         =  3
};
#define tasn1_type_t enum tasn1_type

/**
 * @brief Internal node structure that holds a value.
 */
struct tasn1_node {
    struct list_head list;
    tasn1_type_t type;
    bool on_heap;
};
#define tasn1_node_t struct tasn1_node

/**
 * @brief The octet_sequence struct
 */
struct octet_sequence {
    tasn1_node_t         node;
    const TASN1_OCTET_T *p_data;
    TASN1_SIZE_T         s_data;
    bool                 is_copy;
};
#define octet_sequence_t struct octet_sequence

/**
 * @brief The item struct
 */
struct item {
    struct list_head list;
    bool on_heap;
    tasn1_node_t *p_key;
    tasn1_node_t *p_val;
};
#define item_t struct item

/**
 * @brief The map struct
 */
struct map {
    tasn1_node_t node;
    struct list_head children;
};
#define map_t struct map

/**
 * @brief The array struct
 */
struct array {
    tasn1_node_t node;
    struct list_head children;
};
#define array_t struct array

/**
 * @brief The number struct
 */
struct number {
    tasn1_node_t node;
    TASN1_NUMBER_T val;
};
#define number_t struct number

/**
 * @brief State for TASN1 scanner.
 */
struct tasn1_iterator {
    tasn1_type_t         ct;
    const TASN1_OCTET_T *p;
    TASN1_SIZE_T         c;
};
#define tasn1_iterator_t struct tasn1_iterator
#define TASN1_ITERATOR_INIT(name) { .ct = TASN1_INVALID, .p = NULL, .c = 0 }
#define TASN1_ITERATOR(name) tasn1_iterator name = TASN1_ITERATOR_INIT(name)

#ifdef __cplusplus
}
#endif

#endif // TYPES_H
