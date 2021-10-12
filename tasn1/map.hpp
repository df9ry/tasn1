#ifndef TASN1_MAP_HPP
#define TASN1_MAP_HPP

#include "node.hpp"

#include <string>

namespace tasn1 {

class Map: public Node
{
public:
    Map();

    void add(Node &key, Node &val);
    void add(const std::string &key, Node &val);
};

} // end namespace tasn1 //

#endif // TASN1_MAP_HPP
