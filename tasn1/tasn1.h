#ifndef TASN1_H
#define TASN1_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define TASN1_OCTET  uint8_t
#define TASN1_NUMBER int16_t

/**
 * @brief Internal node structure that holds a value.
 */
struct tasn1_node;
#define tasn1_node_t struct tasn1_node

/**
 * @brief Datatype of a asn1_node.
 */
enum tasn1_type { TASN1_MAP_T = 0, TASN1_ARRAY_T = 1, TASN1_OCTET_SEQUENCE_T = 2, TASN1_NUMBER_T = 3 };
#define tasn1_type_t enum tasn1_type

/**
 * @brief Create new asn1_node for octet sequence.
 * 
 * @param po Pointer to the octet sequence.
 * @param co Size of the octet sequence.
 * @param copy When true, the value is copied, otherwise only reference
 * @return tasn1_node_t* New asn1_node
 */
tasn1_node_t *tasn1_new_octet_sequence(const TASN1_OCTET *po, size_t co, bool copy);

/**
 * @brief Create new asn1_node for string.
 * 
 * @param s String to store. By referemce ,not copied!
 * @param copy When true, the value is copied, otherwise only reference
 * @return tasn1_node_t* New asn1_node
 */
#define tasn1_new_string(S, COPY) \
    tasn1_new_octet_sequence((TASN1_OCTET *)S, strlen(S) + 1, COPY)

/**
 * @brief Create new asn1_node for storing of array values
 * 
 * @return tasn1_node_t* New asn1_node 
 */
tasn1_node_t *tasn1_new_array();

/**
 * @brief Add value to an array
 * 
 * @param array The array to add this value to.
 * @param val Value to add
 * @return int Error code. 0 is OK
 */
int tasn1_add_array_value(tasn1_node_t *array, tasn1_node_t *val);

/**
 * @brief Create new asn1_node for storing of map items
 * 
 * @return tasn1_node_t* New asn1_node
 */
tasn1_node_t *tasn1_new_map();

/**
 * @brief Add item to a map
 * 
 * @param map The map to add this item to.
 * @param key Item key 
 * @param val Item value
 * @return int Error code. 0 is OK
 */
int tasn1_add_map_item(tasn1_node_t *map, tasn1_node_t *key, tasn1_node_t *val);

#define tasn1_add_map_string(MAP, KEY, COPY, VAL) \
    tasn1_add_map_item(MAP, tasn1_new_octet_sequence((const TASN1_OCTET *)KEY, strlen(KEY) + 1 , COPY), VAL)

/**
 * @brief Create new asn1_node for number.
 * 
 * @param n Number to store.
 * @return tasn1_node_t* New asn1_node
 */
tasn1_node_t *tasn1_new_number(TASN1_NUMBER n);

/**
 * @brief Create new asn1_node for boolean.
 * 
 * @param f Boolean to store.
 * @return tasn1_node_t* New asn1_node.
 */
#define tasn1_new_bool(F) \
    tasn1_new_number((F) ? 1 : 0)

/**
 * @brief Get number of octets required for serialization of this node.
 * 
 * @param node The node to query.
 * @return size_t Number of octets needed for serialization or negative error code.
 */
int tasn1_size(const tasn1_node_t *node);

/**
 * @brief Serialize node to a buffer. 
 * 
 * @param node Node to serialize.
 * @param po Pointer to buffer for serialization.
 * @param co Size of buffer for serialization.
 * @return Number of octets written or negative error number.
 */
int tasn1_serialize(const tasn1_node_t *node, TASN1_OCTET *po, size_t co);

/**
 * @brief Release all ressources allocated by a node, incl. all related nodes.
 * 
 * @param node The node to release.
 */
void tasn1_free(tasn1_node_t *node);

#ifdef __cplusplus
}
#endif

#endif // TASN1_H
