#include "tasn1/node.hpp"
#include "tasn1/map.hpp"
#include "tasn1/array.hpp"
#include "tasn1/number.hpp"
#include "tasn1/octetsequence.hpp"
#include "tasn1.h"

#include <functional>
#include <cerrno>

using namespace std;
using namespace jsonx;

namespace tasn1 {

node_ptr_t Node::fromJson(const jsonx::json &j) {
    switch (j.getType()) {
    case json::UNDEFINED_T :
        return node_ptr_t(new OctetSequence(NULL, 0));
    case json::NULL_T :
        return node_ptr_t(new Number(static_cast<TASN1_NUMBER_T>(0)));
    case json::BOOL_T :
        return node_ptr_t(new Number(static_cast<TASN1_NUMBER_T>(j.toBool())));
    case json::SIGNED_T :
        return node_ptr_t(new Number(static_cast<TASN1_NUMBER_T>(j.toSigned())));
    case json::UNSIGNED_T :
        return node_ptr_t(new Number(static_cast<TASN1_NUMBER_T>(j.toSigned())));
    case json::REAL_T : {
        union {
            json_real_t r;
            TASN1_OCTET_T o[sizeof(json_real_t)];
        } u;
        u.r = j.toReal();
        return node_ptr_t(new OctetSequence(u.o, static_cast<TASN1_NUMBER_T>(sizeof(json_real_t))));
    }
    case json::STRING_T :
        return node_ptr_t(new OctetSequence(j.toString()));
    case json::ARRAY_T :
    {
        tasn1::Array *ta = new tasn1::Array();
        const json_array_t &ja{j.toArrayRef()};
        for_each (ja.begin(), ja.end(), [&ta] (const json &j1) {
            node_ptr_t n{fromJson(j1)};
            ta->add(n);
        }); // end for_each //
        return node_ptr_t(ta);
    }
    case json::OBJECT_T : {
        tasn1::Map *tm = new tasn1::Map();
        const json_object_t &jo{j.toObject()};
        for_each (jo.begin(), jo.end(), [&tm] (const json_object_value_t &pair) {
            node_ptr_t key = node_ptr_t(new tasn1::OctetSequence(pair.first));
            node_ptr_t val = fromJson(pair.second);
            tm->add(key, val);
        }); // end for_each //
        return node_ptr_t(tm);
    }
    default:
        return node_ptr_t(new OctetSequence(NULL, 0));
    } // end switch //
}

Node::~Node() {
}

void Node::serialize(vector_t &buffer) {
    int n{::tasn1_size(node)};
    if (n < 0)
        throw std::runtime_error("SYS error: " + string(strerror(-n)));
    buffer.resize(n);
    int m{::tasn1_serialize(node, buffer.data(), n)};
    if (m < 0)
        throw std::runtime_error("IO error: " + string(strerror(-n)));
    if (m != n)
        throw std::runtime_error("Size inconsistency " + std::to_string(n) + " <-> "
                                                       + std::to_string(m));
}

} // end namespace tasn1 //
