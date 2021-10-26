#ifndef TASN1_NODE_HPP
#define TASN1_NODE_HPP

#include <jsonx.hpp>

#include <memory>
#include <stdexcept>
#include <vector>
#include <string>
#include <memory>
#include <list>

extern "C" {
    struct tasn1_node;
} // end extern "C" //

namespace tasn1 {

class Node;

typedef std::vector<unsigned char> vector_t;
typedef std::shared_ptr<Node> node_ptr_t;

class Node {
public:
    static node_ptr_t fromJson(const jsonx::json &j);

    Node() = default;
    Node(const Node &other) = delete;
    Node(Node &&other) = delete;
    ~Node();

    bool isContained() const { return contained; }
    void setContained() {
        if (contained)
            throw std::runtime_error("Node is already contained");
        contained = true;
    }

    void serialize(vector_t &buffer);

    tasn1_node *getNode() { return node; }

protected:
    void setNode(tasn1_node *_node) { node = _node; }

    tasn1_node *node{nullptr};
    bool contained{false};
    std::list<node_ptr_t> children{};
};

} // end namespace tasn1 //

#endif // TASN1_NODE_HPP
