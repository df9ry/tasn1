#ifndef RANGE_EXCEPTION_HPP
#define RANGE_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace tasn1 {

class TASN1_RangeError: public std::range_error {
public:
    explicit TASN1_RangeError(const std::string& what_arg): std::range_error(what_arg) {};
    explicit TASN1_RangeError(const char* what_arg): std::range_error(what_arg) {};
};

} // end namespace tasn1 //

#endif // RANGE_EXCEPTION_HPP
