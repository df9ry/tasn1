#ifndef TASN1_MAP_HPP
#define TASN1_MAP_HPP

#include "node.hpp"
#include "tasn1.h"

#include "number.hpp"

#include <string>
#include <list>
#include <memory>

namespace tasn1 {

class Map: public Node
{
public:
    Map();

    void add(node_ptr_t key, node_ptr_t val);
    void add(const std::string &key, node_ptr_t val);
    void add(const std::string &key, const std::string &val);
    void add(const std::string &key, const char *val) { add(key, std::string(val)); }
    void add(const std::string &key, bool     b) { add(key, node_ptr_t(new Number(b))); }
    void add(const std::string &key, int8_t   x) { add(key, node_ptr_t(new Number(x))); }
    void add(const std::string &key, uint8_t  x) { add(key, node_ptr_t(new Number(x))); }
    void add(const std::string &key, int16_t  x) { add(key, node_ptr_t(new Number(x))); }
    void add(const std::string &key, uint16_t x) { add(key, node_ptr_t(new Number(x))); }
    void add(const std::string &key, int32_t  x) { add(key, node_ptr_t(new Number(x))); }
    void add(const std::string &key, uint32_t x) { add(key, node_ptr_t(new Number(x))); }

private:
    map_t it;
    std::list<std::unique_ptr<item_t>> items{};
};

} // end namespace tasn1 //

#endif // TASN1_MAP_HPP
