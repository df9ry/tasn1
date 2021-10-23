#include "tasn1/array.hpp"
#include "tasn1.h"

namespace tasn1 {

Array::Array(): Node(&it.node) {
    tasn1_init_array(&it);
}

Array::~Array() {
    tasn1_array_reset(&it);
}

void Array::add(Node &child) {
    child.setContained();
    tasn1_add_array_value(node, child.getNode());
}

} // end namespace tasn1 //
