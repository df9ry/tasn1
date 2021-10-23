#include "tasn1/number.hpp"
#include "tasn1.h"

namespace tasn1 {

Number::Number(int16_t n): Node(::tasn1_new_number(n)) {}
Number::Number(bool b): Node(::tasn1_new_number(static_cast<TASN1_NUMBER_T>(b))) {};

} // end namespace tasn1 //
