#ifndef TASN1_NODE_HPP
#define TASN1_NODE_HPP

#include <memory>
#include <stdexcept>
#include <vector>

extern "C" {
    struct tasn1_node;
} // end extern "C" //

namespace tasn1 {

typedef std::vector<unsigned char> vector_t;

class Node {
public:
    Node() = delete;
    Node(const Node &other) = delete;
    Node(Node &&other) {
        std::swap(node,      other.node);
        std::swap(contained, other.contained);
    }
    ~Node();

    bool isContained() const { return contained; }
    void setContained() {
        if (contained)
            throw std::runtime_error("Node is already contained");
        contained = true;
    }

    struct tasn1_node *getNode() { return node; }

    void serialize(vector_t &buffer);

protected:
    Node(struct tasn1_node *_node): node{_node} {}

    struct tasn1_node *node;
    bool contained{false};
};

} // end namespace tasn1 //

#endif // TASN1_NODE_HPP
