#ifndef TASN1_H
#define TASN1_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tasn1/types.h"

#undef HAVE_HEAP // Don't use the heap. Good for very small devices.

/**
 * @brief Initialize octet sequence.
 *
 * @param it Pointer to an octet sequence struct.
 * @param po Pointer to the octet sequence.
 * @param co Size of the octet sequence.
 */
void tasn1_init_octet_sequence(octet_sequence_t *it, const TASN1_OCTET_T *po, TASN1_SIZE_T co);

/**
 * @brief Reset already initialized octet sequence with a new value.
 *
 * @param it Pointer to an octet sequence struct.
 * @param po Pointer to the octet sequence.
 * @param co Size of the octet sequence.
 */
void tasn1_reset_octet_sequence(octet_sequence_t *it, const TASN1_OCTET_T *po, TASN1_SIZE_T co);

#define tasn1_reset_string(O,S) \
    tasn1_reset_octet_sequence(O, (const TASN1_OCTET_T *)S, strlen(S) + 1)

#ifdef HAVE_HEAP
/**
 * @brief Create new asn1_node for octet sequence.
 * 
 * @param po Pointer to the octet sequence.
 * @param co Size of the octet sequence.
 * @param copy When true, the value is copied, otherwise only reference
 * @return tasn1_node_t* New asn1_node
 */
tasn1_node_t *tasn1_new_octet_sequence(const TASN1_OCTET_T *po, TASN1_SIZE_T co, bool copy);
#endif

#define tasn1_init_string(N,S) \
    tasn1_init_octet_sequence(N, (const TASN1_OCTET_T *)S, strlen(S) + 1);

#ifdef HAVE_HEAP
/**
 * @brief Create new asn1_node for string.
 * 
 * @param s String to store. By referemce ,not copied!
 * @param copy When true, the value is copied, otherwise only reference
 * @return tasn1_node_t* New asn1_node
 */
#define tasn1_new_string(S, COPY) \
    tasn1_new_octet_sequence((const TASN1_OCTET_T *)S, strlen(S) + 1, COPY)
#endif

/**
 * @brief tasn1_init_array Initialize an array.
 *
 * @param it The array structure to initialize.
 */
void tasn1_init_array(array_t *it);

#ifdef HAVE_HEAP
/**
 * @brief Create new asn1_node for storing of array values
 * 
 * @return tasn1_node_t* New asn1_node 
 */
tasn1_node_t *tasn1_new_array();
#endif

/**
 * @brief Add value to an array
 * 
 * @param array The array to add this value to.
 * @param val Value to add
 * @return int Error code. 0 is OK
 */
int tasn1_add_array_value(array_t *array, tasn1_node_t *val);

/**
 * @brief tasn1_array_reset Release all child elements.
 * @param it The array to reset.
 */
void tasn1_array_reset(array_t *it);

/**
 * @brief tasn1_init_map Initialize a map.
 * @param it
 */
void tasn1_init_map(map_t *it);

#ifdef HAVE_HEAP
/**
 * @brief Create new asn1_node for storing of map items
 * 
 * @return tasn1_node_t* New asn1_node
 */
tasn1_node_t *tasn1_new_map();
#endif

/**
 * @brief tasn1_map_reset Release all child elements.
 * @param it The map to reset.
 */
void tasn1_map_reset(map_t *it);

/**
 * @brief tasn1_init_item Initialize an item struct.
 * @param it Pointer to the struct to initiaize.
 * @param key Pointer to the key.
 * @param val Pointer to the value.
 */
void tasn1_init_item(item_t *it, tasn1_node_t *key, tasn1_node_t *val);

#ifdef HAVE_HEAP
/**
 * @brief new_item Create a new map item on the heap.
 * @param key Item key.
 * @param val Item value.
 * @return New item pointer or NULL, if out of memory.
 */
item_t *tasn1_new_item(tasn1_node_t *key, tasn1_node_t *val);
#define tasn1_new_string_item(MAP, KEY, COPY, VAL) \
    tasn1_new_item(MAP, tasn1_new_octet_sequence((const TASN1_OCTET_T *)KEY, strlen(KEY) + 1 , COPY), VAL)
#endif

#ifdef HAVE_HEAP
/**
 * @brief item_free Free an item.
 * @param it The item to free.
 */
void tasn1_item_free(item_t *it);
#endif

/**
 * @brief tasn1_reset_item Reset item with new values.
 * @param it. Item to reset.
 * @param key. New key or NULL to keep current key.
 * @param val. New value or NULL to keep current value.
 */
void tasn1_reset_item(item_t *it, tasn1_node_t *key, tasn1_node_t *val);

/**
 * @brief Add item to a map
 * 
 * @param map The map to add this item to.
 * @param item The item to add
 * @return int Error code. 0 is OK
 */
int tasn1_map_add_item(map_t *map, item_t *item);

/**
 * @brief tasn1_init_number Initialize a number structure.
 * @param it The number structure to initialize.
 */
void tasn1_init_number(number_t *it, TASN1_NUMBER_T n);

#ifdef HAVE_HEAP
/**
 * @brief Create new asn1_node for number.
 * 
 * @param n Number to store.
 * @return tasn1_node_t* New asn1_node
 */
tasn1_node_t *tasn1_new_number(TASN1_NUMBER_T n);
#endif

/**
 * @brief tasn1_reset_number. Reset number with a new value.
 * @param it. Number item to reset.
 * @param n. New value.
 */
void tasn1_reset_number(number_t *it, TASN1_NUMBER_T n);

/**
 * @brief tasn1_unlink Remove node from a list.
 * @param it The node to unlink.
 */
static inline void tasn1_unlink(tasn1_node_t *it)
{
    if (it)
        list_del_init(&it->list);
}

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

#ifdef HAVE_HEAP
/**
 * @brief Release all ressources allocated by a node, incl. all related nodes.
 * 
 * @param node The node to release.
 */
void tasn1_free(tasn1_node_t *node);
#endif

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

/**
 * @brief tasn1_iterator_set Initialize an existing iterator with a
 *        container.
 * @param iter Pointer to existing iterator.
 * @param po Pointer into a serialized stream.
 * @return 0, if OK, ENOMEN if iter is NULL, EINVAL if neither ARRAY
 *         nor MAP.
 */
int tasn1_iterator_set(tasn1_iterator_t *iter, const TASN1_OCTET_T *po);

#ifdef HAVE_HEAP
/**
 * @brief tasn1_new_iterator Create and initialize a new iterator.
 *        Note: This iterator must be freed with tasn1_iterator_free!
 * @return New iterator or NULL, if no memeory is available.
 */
tasn1_iterator_t *tasn1_new_iterator();
#endif

#ifdef HAVE_HEAP
/**
 * @brief tasn1_new_iterator_set Create a new iterator and set it
 *        to a seriaized container.
 *        Note: This iterator must be freed with tasn1_iterator_free!
 * @param po Pointer into a serialized stream.
 * @return New ready to use iterator or NULL, if either no memory is
 *        available or po doesn't point to a valid container object.
 */
tasn1_iterator_t *tasn1_new_iterator_set(const TASN1_OCTET_T *po);
#endif

#ifdef HAVE_HEAP
/**
 * @brief tasn1_iterator_free Free an dynamically allocated iterator.
 */
#define tasn1_iterator_free(iter) free(iter)
#endif

/**
 * @brief tasn1_iterator_get Get position to the current item in the
 *        stream and advance iterator.
 * @param iter Pointer to iterator.
 * @return Pointer to current node in stream or NULL if no item
 *         available (Not a container or end of list).
 */
const TASN1_OCTET_T *tasn1_iterator_get(tasn1_iterator_t *iter);

/**
 * @brief tasn1_get_value_size Get size in stream.
 * @param po Pointer to the stream
 * @return Number of octets occupied by this value or negative error
 *         number;
 */
int tasn1_get_total_size(const TASN1_OCTET_T *po);

/**
 * @brief tasn1_get_header_size Get size of header in stream.
 * @param po Pointer to the stream.
 * @return Number of octets ocupied by the header.
 */
TASN1_SIZE_T tasn1_get_header_size(const TASN1_OCTET_T *po);

/**
 * @brief tasn1_get_content_size Get size of content after the
 *        header in stream.
 * @param po Pointer to the stream.
 * @return Number of octets occupied by this value or negative error
 *         number;
 */
int tasn1_get_content_size(const TASN1_OCTET_T *po);

#ifdef __cplusplus
}
#endif

#endif // TASN1_H
