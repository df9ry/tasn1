#include "tasn1/node.hpp"
#include "tasn1/map.hpp"
#include "tasn1/array.hpp"
#include "tasn1/number.hpp"
#include "tasn1/octetsequence.hpp"
#include "tasn1.h"

#include <functional>

using namespace std;
using namespace jsonx;

namespace tasn1 {

Node Node::fromJson(const jsonx::json &j) {
    switch (j.getType()) {
    case json::UNDEFINED_T :
        return OctetSequence(NULL, 0);
    case json::NULL_T :
        return Number(static_cast<TASN1_NUMBER_T>(0));
    case json::BOOL_T :
        return Number(j.toBool());
    case json::SIGNED_T :
        return Number(static_cast<TASN1_NUMBER_T>(j.toSigned()));
    case json::UNSIGNED_T :
        return Number(static_cast<TASN1_NUMBER_T>(j.toSigned()));
    case json::REAL_T : {
        union {
            json_real_t r;
            TASN1_OCTET_T o[sizeof(json_real_t)];
        } u;
        u.r = j.toReal();
        return OctetSequence(u.o, static_cast<TASN1_NUMBER_T>(sizeof(json_real_t)));
    }
    case json::STRING_T :
        return OctetSequence(j.toString());
    case json::ARRAY_T :
    {
        tasn1::Array ta;
        const json_array_t &ja{j.toArrayRef()};
        for_each (ja.begin(), ja.end(), [&ta] (const json &j1) {
            tasn1::Node n{fromJson(j1)};
            ta.add(n);
        }); // end for_each //
        return move(ta);
    }
    case json::OBJECT_T : {
        tasn1::Map tm;
        const json_object_t &jo{j.toObject()};
        for_each (jo.begin(), jo.end(), [&tm] (const json_object_value_t &pair) {
            tasn1::OctetSequence key(pair.first);
            tasn1::Node val(fromJson(pair.second));
            tm.add(key, val);
        }); // end for_each //
        return move(tm);
    }
    default:
        return OctetSequence(NULL, 0);
    } // end switch //
}

Node::Node(Node &&other) {
    node = other.node;
    other.node = nullptr;
    contained = other.contained;
    other.contained = true; // Avoid unneccesary deletion of nullprt
}

Node::~Node() {
    node = nullptr;
    contained = false;
}

void Node::serialize(vector_t &buffer) {
    int n{::tasn1_size(node)};
    if (n < 0)
        throw std::runtime_error("SYS error " + std::to_string(n));
    buffer.resize(n);
    int m{::tasn1_serialize(node, buffer.data(), n)};
    if (m < 0)
        throw std::runtime_error("IO error " + std::to_string(n));
    if (m != n)
        throw std::runtime_error("Size inconsistency " + std::to_string(n) + " <-> " + std::to_string(m));
}

} // end namespace tasn1 //
