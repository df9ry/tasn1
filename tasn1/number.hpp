#ifndef TASN1_NUMBER_HPP
#define TASN1_NUMBER_HPP

#include "node.hpp"
#include "tasn1/types.h"

#include <cinttypes>

namespace tasn1 {

class Number: public Node
{
public:
    Number(TASN1_NUMBER_T n);
    Number(bool     b): Number(static_cast<TASN1_NUMBER_T>(b)) {};
    Number(int8_t   x): Number(static_cast<TASN1_NUMBER_T>(x)) {};
    Number(uint8_t  x): Number(static_cast<TASN1_NUMBER_T>(x)) {};
    Number(uint16_t x): Number(downcast(x)) {};
    Number(int32_t  x): Number(downcast(x)) {};
    Number(uint32_t x): Number(downcast(x)) {};

private:
    TASN1_NUMBER_T downcast(int32_t  x);
    TASN1_NUMBER_T downcast(uint32_t x);

    number_t number;
};

} // end namespace tasn1 //

#endif // TASN1_NUMBER_HPP
