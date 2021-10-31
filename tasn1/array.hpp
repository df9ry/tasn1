#ifndef TASN1_ARRAY_HPP
#define TASN1_ARRAY_HPP

#include "node.hpp"
#include "tasn1.h"
#include "number.hpp"

#include <list>
#include <string>
#include <memory>

namespace tasn1 {

class Array: public Node {
public:
    Array();

    void add(node_ptr_t child);
    void add(const std::string &val);
    void add(const char *val) { add(std::string(val)); }
    void add(bool     b) { add(node_ptr_t(new Number(b))); }
    void add(int8_t   x) { add(node_ptr_t(new Number(x))); }
    void add(uint8_t  x) { add(node_ptr_t(new Number(x))); }
    void add(int16_t  x) { add(node_ptr_t(new Number(x))); }
    void add(uint16_t x) { add(node_ptr_t(new Number(x))); }
    void add(int32_t  x) { add(node_ptr_t(new Number(x))); }
    void add(uint32_t x) { add(node_ptr_t(new Number(x))); }

private:
    array_t it;
};

} // end namespace tasn1 //

#endif // TASN1_ARRAY_HPP
