#ifndef TASN1_ARRAY_HPP
#define TASN1_ARRAY_HPP

#include "node.hpp"
#include "tasn1.h"

namespace tasn1 {

class Array: public Node
{
public:
    Array();
    ~Array();

    void add(Node &child);

private:
    array_t it;
};

} // end namespace tasn1 //

#endif // TASN1_ARRAY_HPP
