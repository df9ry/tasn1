#ifndef TASN1_MAP_HPP
#define TASN1_MAP_HPP

#include "node.hpp"
#include "tasn1.h"

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
    void add(const std::string &key, TASN1_NUMBER_T val);

private:
    map_t it;
    std::list<std::unique_ptr<item_t>> items{};
};

} // end namespace tasn1 //

#endif // TASN1_MAP_HPP
