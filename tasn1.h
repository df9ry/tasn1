#ifndef TASN1_H
#define TASN1_H

#include "tasn1/types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create new asn1_node for octet sequence.
 * 
 * @param po Pointer to the octet sequence.
 * @param co Size of the octet sequence.
 * @param copy When true, the value is copied, otherwise only reference
 * @return tasn1_node_t* New asn1_node
 */
tasn1_node_t *tasn1_new_octet_sequence(const TASN1_OCTET_T *po, TASN1_SIZE_T co, bool copy);

/**
 * @brief Create new asn1_node for string.
 * 
 * @param s String to store. By referemce ,not copied!
 * @param copy When true, the value is copied, otherwise only reference
 * @return tasn1_node_t* New asn1_node
 */
#define tasn1_new_string(S, COPY) \
    tasn1_new_octet_sequence((TASN1_OCTET_T *)S, strlen(S) + 1, COPY)

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
    tasn1_add_map_item(MAP, tasn1_new_octet_sequence((const TASN1_OCTET_T *)KEY, strlen(KEY) + 1 , COPY), VAL)

/**
 * @brief Create new asn1_node for number.
 * 
 * @param n Number to store.
 * @return tasn1_node_t* New asn1_node
 */
tasn1_node_t *tasn1_new_number(TASN1_NUMBER_T n);

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
int tasn1_serialize(const tasn1_node_t *node, TASN1_OCTET_T *po, TASN1_SIZE_T co);

/**
 * @brief Release all ressources allocated by a node, incl. all related nodes.
 * 
 * @param node The node to release.
 */
void tasn1_free(tasn1_node_t *node);

/**
 * @brief tasn1_get_type Get type of serialized node.
 * @param po Pointer into serialized stream.
 * @return Type of node.
 */
tasn1_type_t tasn1_get_type(const TASN1_OCTET_T *po);

/**
 * @brief tasn1_get_number Read number out of a serialized
 *        stream.
 * @param po Pointer into a serialized stream.
 * @param pn Pointer to a number vaialble to fill.
 * @return Error code - 0 is OK, ENOMEM when pn is NULL, EINVAL when
 *         po is not pointing to a number.
 */
int tasn1_get_number(const TASN1_OCTET_T *po, TASN1_NUMBER_T *pn);

/**
 * @brief tasn1_get_octetsequence Read octetsequence out of a serialized
 *        stream.
 * @param po Pointer into a serialized stream.
 * @param ppo Pointer to a buffer with the octets.
 * @param pso Pointer to size object where to store the length.
 * @return Error code - 0 is OK, ENOMEM when ppo or pso is NULL, EINVAL when
 *         po is not pointing to a octetsequence.
 */
int tasn1_get_octetsequence(const TASN1_OCTET_T *po,
                            const TASN1_OCTET_T **ppo, TASN1_SIZE_T *pso);

/**
 * @brief tasn1_get_string Read string out of a serialized stream.
 * @param po Pointer into a serialized stream.
 * @return Pointer to C-string or NULL, if no C-string there.
 */
const char *tasn1_get_string(const TASN1_OCTET_T *po);

#ifdef __cplusplus
}
#endif

#endif // TASN1_H
