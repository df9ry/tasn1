#include "tasn1/number.hpp"
#include "tasn1.h"

namespace tasn1 {

Number::Number(int16_t n): Node(&number.node) {
    tasn1_init_number(&number, n);
}
Number::Number(bool b): Node(&number.node) {
    tasn1_init_number(&number, static_cast<TASN1_NUMBER_T>(b));
};

} // end namespace tasn1 //
