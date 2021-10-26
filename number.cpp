#include "tasn1/number.hpp"
#include "tasn1.h"

namespace tasn1 {

Number::Number(int16_t n): Node() {
    tasn1_init_number(&number, n);
    setNode(&number.node);
}

Number::Number(bool b): Node() {
    tasn1_init_number(&number, static_cast<TASN1_NUMBER_T>(b));
    setNode(&number.node);
};

} // end namespace tasn1 //
