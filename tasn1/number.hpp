#ifndef TASN1_NUMBER_HPP
#define TASN1_NUMBER_HPP

#include "node.hpp"

namespace tasn1 {

class Number: public Node
{
public:
    Number(int16_t n);
    Number(bool b);
};

} // end namespace tasn1 //

#endif // TASN1_NUMBER_HPP
