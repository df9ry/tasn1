#include "tasn1/array.hpp"
#include "tasn1/tasn1.h"

namespace tasn1 {

Array::Array(): Node(::tasn1_new_array()) {}

void Array::add(Node &child) {
    child.setContained();
    ::tasn1_add_array_value(node, child.getNode());
}

} // end namespace tasn1 //
