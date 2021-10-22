#ifndef TASN1_NUMBER_HPP
#define TASN1_NUMBER_HPP

#include "node.hpp"
#include "tasn1/types.h"

namespace tasn1 {

class Number: public Node
{
public:
    Number(TASN1_NUMBER_T n);
    Number(bool b);
};

} // end namespace tasn1 //

#endif // TASN1_NUMBER_HPP
