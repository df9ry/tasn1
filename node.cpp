#include "tasn1/node.hpp"
#include "tasn1/tasn1.h"

namespace tasn1 {

Node::~Node() {
    if (!contained) {
        ::tasn1_free(node);
    }
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
