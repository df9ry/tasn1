#include "tasn1/map.hpp"
#include "tasn1/octetsequence.hpp"
#include "tasn1/number.hpp"
#include "tasn1.h"

#include <algorithm>

namespace tasn1 {

Map::Map(): Node(&it.node) {
    tasn1_init_map(&it);
}

Map::~Map() {
    std::for_each(items.begin(), items.end(), [] (const std::unique_ptr<item_t> &pi) {
        list_del(&pi->list);
    });
}

void Map::add(Node &key, Node &val) {
    if (key.isContained())
        throw std::runtime_error("Key is already contained");
    if (val.isContained())
        throw std::runtime_error("Val is already contained");
    key.setContained();
    val.setContained();
    item_t *item = new item_t();
    tasn1_init_item(item, key.getNode(), val.getNode());
    items.emplace_back(std::unique_ptr<item_t>(item));
}

void Map::add(const std::string &key, Node &val) {
    OctetSequence keySeq(key);
    add(keySeq, val);
}

void Map::add(const std::string &key, const std::string &val) {
    OctetSequence keySeq(key);
    OctetSequence valSeq(val);
    add(keySeq, valSeq);
}

void Map::add(const std::string &key, int16_t val) {
    OctetSequence keySeq(key);
    Number num(val);
    add(keySeq, num);
}

} // end namespace tasn1 //
