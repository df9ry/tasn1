#ifndef TASN1_ARRAY_HPP
#define TASN1_ARRAY_HPP

#include "node.hpp"

namespace tasn1 {

class Array: public Node
{
public:
    Array();

    void add(Node &child);
};

} // end namespace tasn1 //

#endif // TASN1_ARRAY_HPP
