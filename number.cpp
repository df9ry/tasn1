#include "tasn1/number.hpp"
#include "tasn1/range_exception.hpp"
#include "tasn1.h"

#include <climits>

using namespace std;

namespace tasn1 {

Number::Number(int16_t n): Node() {
    //cerr << "Number::Number(" << n << ")" << endl;
    tasn1_init_number(&number, n);
    setNode(&number.node);
}

TASN1_NUMBER_T Number::downcast(int32_t x) {
    if ((x < SHRT_MIN) || (x > SHRT_MAX))
        throw TASN1_RangeError("Number " + to_string(x) + " out of range [" +
            to_string(SHRT_MIN) + ".." +
            to_string(SHRT_MAX) + "]");
    return static_cast<int16_t>(x);
}

TASN1_NUMBER_T Number::downcast(uint32_t x) {
    if (x > static_cast<uint32_t>(SHRT_MAX))
        throw TASN1_RangeError("Number " + to_string(x) + " out of range [" +
            to_string(SHRT_MIN) + ".." +
            to_string(SHRT_MAX) + "]");
    return static_cast<int16_t>(x);
}

} // end namespace tasn1 //
