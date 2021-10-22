#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define TASN1_OCTET_T  uint8_t
#define TASN1_NUMBER_T int16_t
#define TASN1_SIZE_T   uint16_t

/**
 * @brief Internal node structure that holds a value.
 */
struct tasn1_node;
#define tasn1_node_t struct tasn1_node

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
 * @brief State for TASN1 scanner.
 */
struct tasn1_iterator {
    tasn1_type_t         ct;
    const TASN1_OCTET_T *p;
    TASN1_SIZE_T         c;
};
#define tasn1_iterator_t struct tasn1_iterator

#ifdef __cplusplus
}
#endif

#endif // TYPES_H
