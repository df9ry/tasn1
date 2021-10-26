#ifndef TASN1_ARRAY_HPP
#define TASN1_ARRAY_HPP

#include "node.hpp"
#include "tasn1.h"

#include <list>
#include <memory>

namespace tasn1 {

class Array: public Node {
public:
    Array();

    void add(node_ptr_t child);

private:
    array_t it;
};

} // end namespace tasn1 //

#endif // TASN1_ARRAY_HPP
