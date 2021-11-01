#include "tasn1.h"

#include <errno.h>
#include <limits.h>
#include <assert.h>
#include <stdio.h>

TASN1_SIZE_T tasn1_get_header_size(const TASN1_OCTET_T *po)
{
    if (!po)
        return 0;
    if (!((*po) & 0x80)) // Short form
        return 1;
    return 1 + ((*po) & 0x1f); // 1 + number of length bytes.
}

int tasn1_get_content_size(const TASN1_OCTET_T *po)
{
    tasn1_type_t t = tasn1_get_type(po);
    if (t == TASN1_NUMBER)
        return 0;
    bool long_form = ((*po) & 0x80);
    if (!long_form) // Short form
        return (*po) & 0x1f;
    int length_bytes = (*po) & 0x1f;
    switch (length_bytes) {
    case 0: return 0;
    case 1: return (*(po + 1));
    case 2: return (*(po + 1)) * 256 + (*(po + 2));
    default: return -EINVAL; // We don't support more than two length bytes.
    } // end switch //
}

int tasn1_get_total_size(const TASN1_OCTET_T *po)
{
    int content_size = tasn1_get_content_size(po);
    if (content_size < 0)
        return content_size;
    TASN1_SIZE_T header_size = tasn1_get_header_size(po);
    if (content_size > (USHRT_MAX - header_size))
        return -ENOMEM;
    return header_size + content_size;
}

static int serialize_header(tasn1_type_t type, size_t size, TASN1_OCTET_T *po, size_t co) {
    if (size < 32) { // Use short form:
        if (co < 1)
            return -ENOMEM;
        if (po)
            *po = (type << 5) | size;
        return 1;
    } else if (size <= UCHAR_MAX) { // Use one length byte:
        if (co < 2)
            return -ENOMEM;
        if (po) {
            *po++ = 0x80 | (type << 5) | 0x01;
            *po = size;
        }
        return 2;
    } else if (size <= USHRT_MAX) { // Use two length bytes:
        if (co < 3)
            return -ENOMEM;
        if (po) {
            *po++ = 0x80 | (type << 5) | 0x02;
            *po++ = size / 0x100;
            *po = size & 0xff;
        }
        return 3;
    } else {
        return -ENOMEM;
    }
}

void tasn1_init_octet_sequence(octet_sequence_t *it, const TASN1_OCTET_T *po, TASN1_SIZE_T co)
{
#ifdef MEM_SAVE
    it->choke = OCTET_SEQUENCE_CHOKE;
    it->node.choke = NODE_CHOKE;
#endif
    it->node.type = TASN1_OCTET_SEQUENCE;
    it->node.on_heap = false;
    INIT_LIST_HEAD(&it->node.list);
    it->s_data = co;
    it->p_data = po;
}

void tasn1_reset_octet_sequence(octet_sequence_t *it, const TASN1_OCTET_T *po, TASN1_SIZE_T co)
{
    assert(it->node.type == TASN1_OCTET_SEQUENCE);
    assert(!it->is_copy); // Would result in memory leak!
    it->s_data = co;
    it->p_data = po;
}

octet_sequence_t *tasn1_new_octet_sequence(
        tasn1_allocator_t *allocator,
        const TASN1_OCTET_T *po,
        TASN1_SIZE_T co,
        bool copy)
{
    assert(allocator);
    if (!po)
        return NULL;
    octet_sequence_t *res = allocator->f_alloc(sizeof(octet_sequence_t));
    if (!res)
        return NULL;
    TASN1_OCTET_T *_po;
    if (copy) {
        _po = allocator->f_alloc(co);
        if (!_po) {
            allocator->f_free(res);
            return NULL;
        }
        memcpy(_po, po, co);
    } else {
        _po = (TASN1_OCTET_T *)po;
    }
    tasn1_init_octet_sequence(res, _po, co);
    res->is_copy = copy;
    res->node.on_heap = true;
    return res;
}

void tasn1_del_octet_sequence(tasn1_allocator_t *allocator, octet_sequence_t *it)
{
    assert(allocator);
    if (it) {
        if (it->is_copy) {
            allocator->f_free((TASN1_OCTET_T *)it->p_data);
            it->is_copy = false;
        }
        if (it->node.on_heap)
            allocator->f_free(it);
    }
}

static int serialize_octet_sequence(const octet_sequence_t *it, TASN1_OCTET_T *po, TASN1_SIZE_T co) {
    if (!it)
        return -EINVAL;
    int n = serialize_header(TASN1_OCTET_SEQUENCE, it->s_data, po, co);
    if (n < 0)
        return n;
    if (po)
        po += n;
    if ((int)co < n)
        return -ENOMEM;
    co -= n;
    if (co < it->s_data)
        return -ENOMEM;
    if (po)
        memcpy(po, it->p_data, it->s_data);
    return n + it->s_data;
}

void tasn1_init_map(map_t *it)
{
#ifdef MEM_SAVE
    it->choke = MAP_CHOKE;
    it->node.choke = NODE_CHOKE;
#endif
    it->node.type = TASN1_MAP;
    it->node.on_heap = false;
    INIT_LIST_HEAD(&it->node.list);
    INIT_LIST_HEAD(&it->children);
}

map_t *tasn1_new_map(tasn1_allocator_t *allocator)
{
    assert(allocator);
    map_t *res = allocator->f_alloc(sizeof(map_t));
    if (!res)
        return NULL;
    tasn1_init_map(res);
    res->node.on_heap = true;
    return res;
}

void tasn1_init_item(item_t *it, tasn1_node_t *key, tasn1_node_t *val)
{
#ifdef MEM_SAVE
    it->choke = ITEM_CHOKE;
#endif
    INIT_LIST_HEAD(&it->list);
    assert(list_empty(&key->list));
    it->p_key = key;
    assert(list_empty(&val->list));
    it->p_val = val;
    it->on_heap = false;
}

item_t *tasn1_new_item(tasn1_allocator_t *allocator,
                       tasn1_node_t *key, tasn1_node_t *val)
{
    assert(allocator);
    item_t *res = allocator->f_alloc(sizeof(item_t));
    if (!res)
        return NULL;
    tasn1_init_item(res, key, val);
    res->on_heap = true;
    return res;
}

void tasn1_del_map(tasn1_allocator_t *allocator, map_t *it)
{
    assert(allocator);
    if (it) {
#ifdef MEM_SAVE
        assert(it->choke == MAP_CHOKE);
#endif
        struct list_head *pos;
        struct list_head *n;

        list_for_each_safe(pos, n, &it->children) {
            item_t *current_item = list_entry(pos, item_t, list);
#ifdef MEM_SAVE
            assert(current_item->choke == ITEM_CHOKE);
#endif
            list_del_init(pos);
            if (current_item->on_heap)
                tasn1_del_item(allocator, current_item);
        }
        if (it->node.on_heap)
            allocator->f_free(it);
    }
}

void tasn1_reset_item(item_t *it, tasn1_node_t *key, tasn1_node_t *val)
{
    if (!it)
        return;
#ifdef MEM_SAVE
    assert(it->choke == ITEM_CHOKE);
#endif
    if (key) {
        assert(list_empty(&key->list));
        if (it->p_key)
            assert(!it->p_key->on_heap); // Would cause memory leak!
        it->p_key = key;
    }
    if (val) {
        assert(list_empty(&val->list));
        if (it->p_val)
            assert(!it->p_val->on_heap); // Would cause memory leak!
        it->p_val = val;
    }
}

void tasn1_del_item(tasn1_allocator_t *allocator, item_t *it)
{
    assert(allocator);
    if (it) {
#ifdef MEM_SAVE
        assert(it->choke == ITEM_CHOKE);
#endif
        assert(list_empty(&it->p_key->list));
        if (it->p_key->on_heap)
            tasn1_del(allocator, it->p_key);
        assert(list_empty(&it->p_val->list));
        if (it->p_val->on_heap)
            tasn1_del(allocator, it->p_val);
        if (it->on_heap)
            allocator->f_free(it);
    }
}

static int serialize_item(const item_t *it, TASN1_OCTET_T *po, TASN1_SIZE_T co) {
    if (!it)
        return -ENOENT;

    // Fake a list with two entries:
    int size_without_header = 0;
    {
        int m = tasn1_serialize(it->p_key, NULL, USHRT_MAX);
        if (m < 0)
            return m;
        if ((int)co < m)
            return -ENOMEM;
        size_without_header += m;
    }
    {
        int m = tasn1_serialize(it->p_val, NULL, USHRT_MAX);
        if (m < 0)
            return m;
        if ((int)co < m)
            return -ENOMEM;
        size_without_header += m;
    }
    if (size_without_header < 0)
        return size_without_header;
    int n = serialize_header(TASN1_ARRAY, size_without_header, po, co);
    if (n < 0)
        return n;
    if (po)
        po += n;
    if ((int)co < n)
        return -ENOMEM;
    co -= n;
    {
        int m = tasn1_serialize(it->p_key, po, co);
        if (m < 0)
            return m;
        if (po)
            po += m;
        if ((int)co < m)
            return -ENOMEM;
        co -= m;
        n += m;
    }
    {
        int m = tasn1_serialize(it->p_val, po, co);
        if (m < 0)
            return m;
        if ((int)co < m)
            return -ENOMEM;
        n += m;
    }
    return n;
}

static int map_size_without_header(const map_t *it) {
    if (!it)
        return -ENOENT;

    struct list_head *pos;
    item_t *current_item;
    int n = 0, m;
    list_for_each(pos, &it->children) {
        current_item = list_entry(pos, item_t, list);
        m = serialize_item(current_item, NULL, USHRT_MAX);
        if (m < 0)
            return m;
        n += m;
    }
    return n;
}

static int serialize_map(const map_t *it, TASN1_OCTET_T *po, TASN1_SIZE_T co) {
    if (!it)
        return -ENOENT;
    //if (po)
    //    fprintf(stderr, "serialize_map() {\n");
    int size_without_header = map_size_without_header(it);
    if (size_without_header < 0)
        return size_without_header;
    int n = serialize_header(TASN1_MAP, size_without_header, po, co);
    if (n < 0)
        return n;
    if (po)
        po += n;
    if ((int)co < n)
        return -ENOMEM;
    co -= n;

    struct list_head *pos;
    item_t *current_item;
    int m;
    list_for_each(pos, &it->children) {
        current_item = list_entry(pos, item_t, list);
        m = serialize_item(current_item, po, co);
        if (m < 0)
            return m;
        if (po)
            po += m;
        if ((int)co < m)
            return -ENOMEM;
        if ((int)co < m)
            return -ENOMEM;
        co -= m;
        n += m;
    }

    //if (po)
    //    fprintf(stderr, "serialize_map() }\n");
    return n;
}

int tasn1_map_add_item(map_t *map, item_t *item) {
    if ((!map) || (!item))
        return -ENOENT;
#ifdef MEM_SAVE
    assert(map->choke == MAP_CHOKE);
#endif
    if (!list_empty(&item->list))
        return -EINVAL;
    list_add_tail(&item->list, &map->children);
    return 0;
}

void tasn1_map_reset(map_t *it)
{
    if (!it)
        return;
#ifdef MEM_SAVE
    assert(it->choke == MAP_CHOKE);
#endif
    struct list_head *pos;
    struct list_head *n;
    list_for_each_safe(pos, n, &it->children) {
        item_t *current_item = list_entry(pos, item_t, list);
#ifdef MEM_SAVE
        assert(current_item->choke == ITEM_CHOKE);
#endif
        assert(!current_item->on_heap); // This would lead to corruption!
        list_del_init(pos);
    }
}

void tasn1_init_array(array_t *it)
{
#ifdef MEM_SAVE
    it->choke = ARRAY_CHOKE;
    it->node.choke = NODE_CHOKE;
#endif
    it->node.type = TASN1_ARRAY;
    it->node.on_heap = false;
    INIT_LIST_HEAD(&it->node.list);
    INIT_LIST_HEAD(&it->children);
}

array_t *tasn1_new_array(tasn1_allocator_t *allocator)
{
    assert(allocator);
    array_t *res = allocator->f_alloc(sizeof(array_t));
    if (!res)
        return NULL;
    tasn1_init_array(res);
    res->node.on_heap = true;
    return res;
}

void tasn1_del_array(tasn1_allocator_t *allocator, array_t *it)
{
    assert(allocator);
    if (it) {
        struct list_head *pos;
        struct list_head *n;

        list_for_each_safe(pos, n, &it->children) {
            tasn1_node_t *current_node = list_entry(pos, tasn1_node_t, list);
#ifdef MEM_SAVE
            assert(current_node->choke == NODE_CHOKE);
#endif
            list_del_init(pos);
            if (current_node->on_heap)
                tasn1_del(allocator, current_node);
        }
        if (it->node.on_heap)
            allocator->f_free(it);
    }
}

static int array_size_without_header(const array_t *it) {
    if (!it)
        return -ENOENT;

    struct list_head *pos;
    tasn1_node_t *current_node;

    int n = 0, m;
    list_for_each(pos, &it->children) {
        current_node = list_entry(pos, tasn1_node_t, list);
        m = tasn1_serialize(current_node, NULL, USHRT_MAX);
        if (m < 0)
            return m;
        n += m;
    }

    return n;
}

static int serialize_array(const array_t *it, TASN1_OCTET_T *po, TASN1_SIZE_T co) {
    if (!it)
        return -ENOENT;
    //if (po)
    //    fprintf(stderr, "serialize_array() [\n");
    int size_without_header = array_size_without_header(it);
    if (size_without_header < 0)
        return size_without_header;
    int n = serialize_header(TASN1_ARRAY, size_without_header, po, co);
    if (n < 0)
        return n;
    if (po)
        po += n;
    if ((int)co < n)
        return -ENOMEM;
    co -= n;
    struct list_head *pos;
    tasn1_node_t *current_node;

    list_for_each(pos, &it->children) {
        current_node = list_entry(pos, tasn1_node_t, list);
        int m = tasn1_serialize(current_node, po, co);
        if (m < 0)
            return m;
        if (po)
            po += m;
        if ((int)co < m)
            return -ENOMEM;
        co -= m;
        n += m;
    }

    //if (po)
    //    fprintf(stderr, "serialize_array() ]\n");
    return n;
}

int tasn1_add_array_value(array_t *array, tasn1_node_t *val) {
    if ((!array) || (!val))
        return -ENOENT;
    if (!list_empty(&val->list)) // val already in another list!
        return -EINVAL;
    list_add_tail(&val->list, &array->children);
    return 0;
}

void tasn1_array_reset(array_t *it)
{
    if (!it)
        return;
#ifdef MEM_SAVE
    assert(it->choke == ARRAY_CHOKE);
#endif
    struct list_head *pos;
    struct list_head *n;
    list_for_each_safe(pos, n, &it->children) {
        tasn1_node_t *current_node = list_entry(pos, tasn1_node_t, list);
#ifdef MEM_SAVE
        assert(current_node->choke == NODE_CHOKE);
#endif
        assert(!current_node->on_heap); // This would lead to corruption!
        list_del_init(pos);
    }
}

void tasn1_init_number(number_t *it, TASN1_NUMBER_T n)
{
#ifdef MEM_SAVE
    it->choke = OCTET_SEQUENCE_CHOKE;
    it->node.choke = ARRAY_CHOKE;
#endif
    it->node.type = TASN1_NUMBER;
    it->node.on_heap = false;
    INIT_LIST_HEAD(&it->node.list);
    it->val = n;
}

number_t *tasn1_new_number(tasn1_allocator_t *allocator, TASN1_NUMBER_T n)
{
    assert(allocator);
    number_t *res = allocator->f_alloc(sizeof(number_t));
    if (!res)
        return NULL;
    tasn1_init_number(res, n);
    res->node.on_heap = true;
    return res;
}

void tasn1_del_number(tasn1_allocator_t *allocator, number_t *it)
{
    assert(allocator);
    if (it->node.on_heap)
        allocator->f_free(it);
}

void tasn1_reset_number(number_t *it, TASN1_NUMBER_T n)
{
    if (!it)
        return;
    it->val = n;
}

static int serialize_number(const number_t *number, TASN1_OCTET_T *po, TASN1_SIZE_T co) {
    TASN1_NUMBER_T val = number->val;
    //if (po)
    //    fprintf(stderr, "serialize_number(%i)\n", val);
    if ((val >= -16) && (val < 16)) {
        if (co < 1)
            return -ENOMEM;
        if (po)
            *po = (TASN1_NUMBER << 5) | (val & 0x1f);
        return 1;
    } else if ((val >= SCHAR_MIN) && (val <= SCHAR_MAX)) {
        if (co < 2)
            return -ENOMEM;
        if (po) {
            *po++ = 0x80 | (TASN1_NUMBER << 5) | 0x01;
            *po = val;
        }
        return 2;
    } else {
        if (co < 3)
            return -ENOMEM;
        if (po) {
            *po++ = 0x80 | (TASN1_NUMBER << 5) | 0x02;
            *po++ = (val >> 8);
            *po = val & 0xff;
        }
        return 3;
    }
}

int tasn1_serialize(const tasn1_node_t *node, TASN1_OCTET_T *po, TASN1_SIZE_T co) {
    if (!node)
        return -ENOENT;
    switch (node->type) {
        case TASN1_MAP:
            return serialize_map(container_of(node, map_t, node), po, co);
        case TASN1_ARRAY:
            return serialize_array(container_of(node, array_t, node), po, co);
        case TASN1_OCTET_SEQUENCE:
            return serialize_octet_sequence(container_of(node, octet_sequence_t, node), po, co);
        case TASN1_NUMBER:
            return serialize_number(container_of(node ,number_t, node), po, co);
        default:
            return -EINVAL;
    } // end switch //
}

int tasn1_size(const tasn1_node_t *node) {
    return tasn1_serialize(node, NULL, USHRT_MAX);
}

void tasn1_del(tasn1_allocator_t *allocator, tasn1_node_t *node)
{
    assert(allocator);
    if (node) {
        switch (node->type) {
            case TASN1_MAP:
                tasn1_del_map(allocator, container_of(node, map_t, node));
                return;
            case TASN1_ARRAY:
                tasn1_del_array(allocator, container_of(node, array_t, node));
                return;
            case TASN1_OCTET_SEQUENCE:
                tasn1_del_octet_sequence(allocator, container_of(node, octet_sequence_t, node));
                return;
            case TASN1_NUMBER:
                tasn1_del_number(allocator, container_of(node, number_t, node));
                return;
            default:
                return;
        } // end switch //
    }
}

tasn1_type_t tasn1_get_type(const TASN1_OCTET_T *po)
{
    if (!po)
        return TASN1_INVALID;
    return (*po & 0x60) >> 5;
}

int tasn1_get_number(const TASN1_OCTET_T *po, TASN1_NUMBER_T *pn)
{
    if (!pn)
        return ENOMEM;
    if (!po)
        return EINVAL;
    if (tasn1_get_type(po) != TASN1_NUMBER)
        return EINVAL;
    uint16_t msb, lsb;
    if (*po & 0x80) {
        int size = *po & 0x1f;
        switch (size) {
        case 1:
            lsb = *(po + 1);
            msb = (lsb & 0x80) ? 0xff : 0x00;
            *pn = (msb << 8) | lsb;
            //fprintf(stderr, "tasn1_get_number(%i)\n", *pn);
            return 0;
        case 2:
            lsb = *(po + 2);
            msb = *(po + 1);
            *pn = (msb << 8) | lsb;
            //fprintf(stderr, "tasn1_get_number(%i)\n", *pn);
            return 0;
        default:
            *pn = -1;
            //fprintf(stderr, "tasn1_get_number() error\n");
            return EINVAL;
        } // end switch //
    }
    lsb = (*po) & 0x001f;
    msb = (lsb & 0x0010) ? 0xffe0 : 0x0000;
    (*pn) = msb | lsb;
    //fprintf(stderr, "tasn1_get_number(%i)\n", *pn);
    return 0;
}

int tasn1_get_octetsequence(const TASN1_OCTET_T *po,
                            const TASN1_OCTET_T **ppo, TASN1_SIZE_T *pso)
{
    if (!po)
        return EINVAL;
    if (tasn1_get_type(po) != TASN1_OCTET_SEQUENCE)
        return EINVAL;

    int size = tasn1_get_content_size(po);
    if ((size < 0) || (size > USHRT_MAX))
        return EINVAL;
    int size_size = tasn1_get_header_size(po);
    if ((size_size < 1) || (size_size > 3))
        return EINVAL;
    if (ppo)
        *ppo = po + size_size;
    if (pso)
        *pso = (TASN1_SIZE_T)size;
    return 0;
}

const char *tasn1_get_string(const TASN1_OCTET_T *po)
{
    const TASN1_OCTET_T *pc;
    TASN1_SIZE_T         sc;
    if (tasn1_get_octetsequence(po, &pc, &sc) != 0)
        return NULL;
    if (sc == 0)
        return NULL;
    // Check for terminating '\0'
    if (pc[sc - 1] != '\0')
        return NULL;
    return (const char *)pc;
}

static void iterator_init(tasn1_iterator_t *iter)
{
    assert(iter);
    iter->ct = TASN1_INVALID;
    iter->p = NULL;
    iter->c = 0;
}

int tasn1_iterator_set(tasn1_iterator_t *iter, const TASN1_OCTET_T *po)
{
    if (!iter)
        return ENOMEM;
    iter->ct = tasn1_get_type(po);
    if ((iter->ct != TASN1_ARRAY) && (iter->ct != TASN1_MAP))
        return EINVAL;
    int header_size = tasn1_get_header_size(po);
    if (header_size < 0)
        return EINVAL;
    int content_size = tasn1_get_content_size(po);
    if (content_size < 0)
        return EINVAL;
    iter->p = po + header_size; // Start at content
    iter->c = content_size; // Left content_size octets
    return 0;
}

tasn1_iterator_t *tasn1_new_iterator(tasn1_allocator_t *allocator)
{
    assert(allocator);
    tasn1_iterator_t *iter = allocator->f_alloc(sizeof(tasn1_iterator_t));
    if (!iter)
        return NULL;
    iterator_init(iter);
    return iter;
}

tasn1_iterator_t *tasn1_new_iterator_set(tasn1_allocator_t *allocator, const TASN1_OCTET_T *po)
{
    assert(allocator);
    tasn1_type_t type = tasn1_get_type(po);
    if ((type != TASN1_ARRAY) && (type != TASN1_MAP))
        return NULL;
    tasn1_iterator_t *iter = allocator->f_alloc(sizeof(tasn1_iterator_t));
    if (!iter)
        return NULL;
    if (tasn1_iterator_set(iter, po) != 0) {
        tasn1_del_iterator(allocator, iter);
        return NULL;
    }
    return iter;
}

const TASN1_OCTET_T *tasn1_iterator_get(tasn1_iterator_t *iter)
{
    if ((!iter) || (iter->ct == TASN1_INVALID) || (iter->p == NULL) || (iter->c == 0))
        return NULL;
    const TASN1_OCTET_T *result = iter->p;
    int content_size = tasn1_get_content_size(iter->p);
    if ((content_size < 0) || (content_size > iter->c)) {
        iterator_init(iter);
        return result;
    }
    int total_size = tasn1_get_total_size(iter->p);
    if ((total_size < 0) || (total_size >= iter->c)) {
        iterator_init(iter);
        return result;
    }
    iter->p += total_size;
    iter->c -= total_size;
    return result;
}

static uint8_t *my_buffer_pointer = NULL;
static size_t my_buffer_size = 0;

static void *my_buffer_alloc(size_t cb)
{
    if (cb > my_buffer_size)
        return NULL;
    void *res = my_buffer_pointer;
    my_buffer_pointer += cb;
    my_buffer_size -= cb;
    return res;
}

static void my_buffer_free(void *pb) {
    // This intentionally does nothing
}

static tasn1_allocator_t my_buffer_allocator = {
    .f_alloc = my_buffer_alloc,
    .f_free  = my_buffer_free
};

void set_buffer_allocator_buffer(uint8_t *pb, size_t cb)
{
    my_buffer_pointer = pb;
    my_buffer_size = cb;
}

tasn1_allocator_t *buffer_allocator(void)
{
    return &my_buffer_allocator;
}
