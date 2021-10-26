#include "tasn1/array.hpp"
#include "tasn1.h"

namespace tasn1 {

using namespace std;

Array::Array(): Node() {
    tasn1_init_array(&it);
    setNode(&it.node);
}

void Array::add(node_ptr_t child) {
    child->setContained();
    tasn1_add_array_value(node, child->getNode());
    children.push_back(child);
}

} // end namespace tasn1 //
