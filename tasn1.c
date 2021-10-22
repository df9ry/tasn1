#include "tasn1.h"

#include <errno.h>
#include <clist.h>
#include <limits.h>

tasn1_node_t {
    tasn1_type_t type;
    struct list_head list;
};

static int serialize_header(tasn1_type_t type, size_t size, TASN1_OCTET_T *po, size_t co) {
    if (size < 32) {
        if (co < 1)
            return -ENOMEM;
        if (po)
            *po = (type << 5) | size;
        return 1;
    } else if (size <= UCHAR_MAX) {
        if (co < 2)
            return -ENOMEM;
        if (po) {
            *po++ = 0x80 | (type << 5) | 0x01;
            *po = size;
        }
        return 2;
    } else if (size <= USHRT_MAX) {
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

struct octet_sequence {
    tasn1_node_t node_base;
    size_t size;
    bool is_copy;
    union {
        const TASN1_OCTET_T *p_data;
        TASN1_OCTET_T data[0];
    };
};
#define octet_sequence_t struct octet_sequence

tasn1_node_t *tasn1_new_octet_sequence(const TASN1_OCTET_T *po, TASN1_SIZE_T co, bool copy) {
    size_t sz = sizeof(octet_sequence_t) + (copy ? co : 0);
    octet_sequence_t *res = malloc(sz);
    if (!res)
        return NULL;
    res->node_base.type = TASN1_OCTET_SEQUENCE;
    INIT_LIST_HEAD(&res->node_base.list);
    if (co > USHRT_MAX - 3) 
        return NULL;
    res->size = co;
    res->is_copy = copy;
    if (copy) {
        memcpy(res->data, po, co);
    } else {
        res->p_data = po;
    }
    return (tasn1_node_t *)res;
}

static void octet_sequence_free(octet_sequence_t *it) {
    free(it);
}

static int serialize_octet_sequence(const octet_sequence_t *it, TASN1_OCTET_T *po, TASN1_SIZE_T co) {
    if (!it)
        return -EINVAL;
    int n = serialize_header(TASN1_OCTET_SEQUENCE, it->size, po, co);
    if (n < 0)
        return n;
    if (po)
        po += n;
    if ((int)co < n)
        return -ENOMEM;
    co -= n;
    if (co < it->size)
        return -ENOMEM;
    const TASN1_OCTET_T *src = (it->is_copy ? it->data : it->p_data);
    if (po)
        memcpy(po, src, it->size);
    if (co < it->size)
        return -ENOMEM;
    return n + it->size;
}

struct map {
    tasn1_node_t node_base;
    struct list_head children;
};
#define map_t struct map

tasn1_node_t *tasn1_new_map() {
    map_t *res = malloc(sizeof(map_t));
    if (!res)
        return NULL;
    res->node_base.type = TASN1_MAP;
    INIT_LIST_HEAD(&res->node_base.list);
    INIT_LIST_HEAD(&res->children);
    return (tasn1_node_t *)res;
}

struct item {
    struct list_head list;
    tasn1_node_t *p_key;
    tasn1_node_t *p_val;
};
#define item_t struct item

static item_t *new_item(tasn1_node_t *key, tasn1_node_t *val) {
    item_t *res = malloc(sizeof(item_t));
    if (!res)
        return NULL;
    INIT_LIST_HEAD(&res->list);
    res->p_key = key;
    res->p_val = val;
    return res;
}

static int serialize_item(const item_t *it, TASN1_OCTET_T *po, TASN1_SIZE_T co) {
    if (!it)
        return -ENOENT;
    int n = tasn1_serialize(it->p_key, po, co);
    if (n < 0)
        return n;
    if (po)
        po += n;
    if ((int)co < n)
        return -ENOMEM;
    co -= n;
    int m = tasn1_serialize(it->p_val, po, co);
    if (m < 0)
        return m;
    return n + m;
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
    int size = map_size_without_header(it);
    if (size < 0)
        return size;
    int n = serialize_header(TASN1_MAP, size, po, co);
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

    return n;
}

static void item_free(item_t *it) {
    if (it) {
        tasn1_free(it->p_key);
        tasn1_free(it->p_val);
        free(it);
    }
}

static void map_free(map_t *it) {
    if (it) {
        struct list_head *pos;
        struct list_head *n;
        item_t *current_item;

        list_for_each_safe(pos, n, &it->children) {
            current_item = list_entry(pos, item_t, list);
            item_free(current_item);
        }
        free(it);
    }
}

int tasn1_add_map_item(tasn1_node_t *map,  tasn1_node_t *key, tasn1_node_t *val) {
    if (!map)
        return -ENOENT;
    if (!(key && val))
        return -ENOENT;
    if (map->type != TASN1_MAP)
        return -EINVAL;
    list_add_tail(&new_item(key, val)->list, &((map_t *)map)->children);
    return 0;
}

struct array {
    tasn1_node_t node_base;
    struct list_head children;
};
#define array_t struct array

tasn1_node_t *tasn1_new_array() {
    array_t *res = malloc(sizeof(array_t));
    if (!res)
        return NULL;
    res->node_base.type = TASN1_ARRAY;
    INIT_LIST_HEAD(&res->node_base.list);
    INIT_LIST_HEAD(&res->children);
    return (tasn1_node_t *)res;
}

static void array_free(array_t *it) {
    if (it) {
        struct list_head *pos;
        struct list_head *n;
        tasn1_node_t *current_node;

        list_for_each_safe(pos, n, &it->children) {
            current_node = list_entry(pos, tasn1_node_t, list);
            tasn1_free(current_node);
        }
        free(it);
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
    int size = array_size_without_header(it);
    if (size < 0)
        return size;
    int n = serialize_header(TASN1_MAP, size, po, co);
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

    return n;
}

int tasn1_add_array_value(tasn1_node_t *array, tasn1_node_t *val) {
    if (!array)
        return -ENOMEM;
    if (!val)
        return -ENOENT;
    if (array->type != TASN1_ARRAY)
        return -EINVAL;
    list_add_tail(&val->list, &((array_t *)array)->children);
    return 0;
}

struct number {
    tasn1_node_t node_base;
    TASN1_NUMBER_T val;
};
#define number_t struct number

tasn1_node_t *tasn1_new_number(TASN1_NUMBER_T n) {
    number_t *res = malloc(sizeof(number_t));
    if (!res)
        return NULL;
    res->node_base.type = TASN1_NUMBER;
    INIT_LIST_HEAD(&res->node_base.list);
    res->val = n;
    return (tasn1_node_t *)res;
}

static void number_free(number_t *it) {
    free(it);
}

static int serialize_number(const number_t *number, TASN1_OCTET_T *po, TASN1_SIZE_T co) {
    TASN1_NUMBER_T val = number->val;
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
            return serialize_map((map_t *)node, po, co);
        case TASN1_ARRAY:
            return serialize_array((array_t *)node, po, co);
        case TASN1_OCTET_SEQUENCE:
            return serialize_octet_sequence((octet_sequence_t *)node, po, co);
        case TASN1_NUMBER:
            return serialize_number((number_t *)node, po, co);
        default:
            return -EINVAL;
    } // end switch //
}

int tasn1_size(const tasn1_node_t *node) {
    return tasn1_serialize(node, NULL, USHRT_MAX);
}

void tasn1_free(tasn1_node_t *node) {
    if (node) {
        switch (node->type) {
            case TASN1_MAP:
                map_free((map_t *)node);
                return;
            case TASN1_ARRAY:
                array_free((array_t *)node);
                return;
            case TASN1_OCTET_SEQUENCE:
                octet_sequence_free((octet_sequence_t *)node);
                return;
            case TASN1_NUMBER:
                number_free((number_t *)node);
                return;
            default:
                return;
        } // end switch //
    }
}
