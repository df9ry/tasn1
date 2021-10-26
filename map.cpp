#include "tasn1/map.hpp"
#include "tasn1/octetsequence.hpp"
#include "tasn1/number.hpp"
#include "tasn1.h"

#include <algorithm>

using namespace std;

namespace tasn1 {

Map::Map(): Node() {
    tasn1_init_map(&it);
    setNode(&it.node);
}

Map::~Map() {
    std::for_each(items.begin(), items.end(), [] (const std::unique_ptr<item_t> &pi) {
        list_del(&pi->list);
    });
}

void Map::add(node_ptr_t key, node_ptr_t val) {
    if (key->isContained())
        throw std::runtime_error("Key is already contained");
    if (val->isContained())
        throw std::runtime_error("Val is already contained");
    key->setContained();
    children.push_back(key);
    val->setContained();
    children.push_back(val);
    item_t *item = new item_t();
    tasn1_init_item(item, key->getNode(), val->getNode());
    items.emplace_back(std::unique_ptr<item_t>(item));
}

void Map::add(const std::string &key, node_ptr_t val) {
    OctetSequence *keySeq = new OctetSequence(key);
    add(node_ptr_t(keySeq), val);
}

void Map::add(const std::string &key, const std::string &val) {
    OctetSequence *keySeq = new OctetSequence(key);
    OctetSequence *valSeq = new OctetSequence(val);
    add(node_ptr_t(keySeq), node_ptr_t(valSeq));
}

void Map::add(const std::string &key, int16_t val) {
    OctetSequence *keySeq = new OctetSequence(key);
    Number *num = new Number(val);
    add(node_ptr_t(keySeq), node_ptr_t(num));
}

} // end namespace tasn1 //
