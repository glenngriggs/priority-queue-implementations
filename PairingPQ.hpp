// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include <deque>
#include <utility>
#include "Eecs281PQ.hpp"

// A specialized version of the priority queue ADT implemented as a pairing heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // ============================
    // Node class with doubly-linked siblings
    // ============================
    class Node {
    public:
        explicit Node(const TYPE &val)
          : elt(val), child(nullptr), sibling(nullptr),
            parent(nullptr), prev(nullptr) {}

        const TYPE &getElt() const { return elt; }
        const TYPE &operator*() const { return elt; }

        friend PairingPQ;  // So the outer class can access private fields
    private:
        TYPE elt;

        // The 'child' pointer points to the leftmost child in the pairing heap.
        Node *child;

        // The 'sibling' pointer points to the next sibling in the parent's child list.
        Node *sibling;

        // The 'parent' pointer points to the parent in the heap.
        Node *parent;

        // The 'prev' pointer points to the previous sibling in the parent's child list
        // (or to the parent if we prefer that approach). We'll use it to cut the node in O(1).
        Node *prev;
    };

    // ============================
    // Constructors, destructor
    // ============================
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR())
      : BaseClass{comp}, root(nullptr), numNodes(0) {}

    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR())
      : BaseClass{comp}, root(nullptr), numNodes(0) {
        for (; start != end; ++start) {
            push(*start);
        }
    }

    // Copy constructor
    PairingPQ(const PairingPQ &other)
      : BaseClass{other.compare}, root(nullptr), numNodes(0) {
        // Collect all nodes from 'other'
        std::deque<Node*> nodes;
        collectNodes(other.root, nodes);
        // Push them into *this* (rebuild structure)
        for (Node *node : nodes) {
            push(node->elt);
        }
    }

    // Copy-assignment operator
    PairingPQ &operator=(const PairingPQ &rhs) {
        if (this != &rhs) {
            PairingPQ temp(rhs);
            std::swap(root, temp.root);
            std::swap(numNodes, temp.numNodes);
        }
        return *this;
    }

    // Destructor
    ~PairingPQ() {
        clear(root);
    }

    // ============================
    // Required interface
    // ============================
    virtual void push(const TYPE &val) {
        addNode(val);
    }

    virtual void pop() {
        if (!root) return;

        Node *oldRoot = root;

        // Gather the children of the root in a deque
        std::deque<Node*> childList;
        Node *curr = root->child;
        while (curr) {
            Node *next = curr->sibling;
            // Detach each child from the old root
            curr->parent = nullptr;
            curr->prev   = nullptr;
            curr->sibling = nullptr;
            childList.push_back(curr);
            curr = next;
        }

        delete oldRoot;
        --numNodes;
        root = nullptr;

        // Meld children pairwise
        while (childList.size() > 1) {
            Node *first = childList.front(); 
            childList.pop_front();
            Node *second = childList.front();
            childList.pop_front();

            childList.push_back(meld(first, second));
        }
        // If there's exactly one child left
        if (!childList.empty()) {
            root = childList.front();
        }
    }

    virtual const TYPE &top() const {
        if (!root) {
            throw std::runtime_error("PairingPQ: top() called on empty PQ!");
        }
        return root->elt;
    }

    [[nodiscard]] virtual std::size_t size() const { return numNodes; }
    [[nodiscard]] virtual bool empty() const { return (root == nullptr); }

    // ============================
    // Pairing-heap-specific interface
    // ============================
    void updateElt(Node *node, const TYPE &new_value) {
        if (!node) return;

        // For a max-heap, we only "bubble up" if the new_value is larger
        if (!this->compare(node->elt, new_value)) {
            // means node->elt >= new_value => no increase => do nothing
            return;
        }
        node->elt = new_value;

        // If it's already the root, no need to re-meld
        if (node != root) {
            // 1) Cut 'node' from its parent's child list in O(1)
            Node *parent = node->parent;

            // If we have a previous sibling, fix that
            if (node->prev) {
                node->prev->sibling = node->sibling;
            }
            // If we have a next sibling, fix that
            if (node->sibling) {
                node->sibling->prev = node->prev;
            }
            // If this node was the direct child of the parent, update that pointer
            if (parent && parent->child == node) {
                parent->child = node->sibling;
            }

            // 2) Reset pointers & meld with root
            node->parent  = nullptr;
            node->prev    = nullptr;
            node->sibling = nullptr;

            root = meld(root, node);
        }
    }

    // Rebuild entire PQ after changes
    void updatePriorities() {
        if (!root) return;

        // 1) Collect all nodes in a container
        std::deque<Node*> allNodes;
        collectNodes(root, allNodes);

        // 2) Reset pointers
        for (Node *node : allNodes) {
            node->child   = nullptr;
            node->sibling = nullptr;
            node->parent  = nullptr;
            node->prev    = nullptr;
        }

        // 3) Rebuild the pairing heap by melding one by one
        root = nullptr;
        numNodes = 0;
        for (Node *node : allNodes) {
            root = meld(root, node);
            ++numNodes;
        }
    }

    // Return a pointer to the newly added node, for use with updateElt
    Node *addNode(const TYPE &val) {
        Node *newNode = new Node(val);
        root = meld(root, newNode);
        ++numNodes;
        return newNode;
    }

private:
    Node *root;         // The root of the pairing heap
    std::size_t numNodes;

    // ============================
    // Private helper functions
    // ============================

    // Meld two trees, returning the resulting root.
    Node *meld(Node *first, Node *second) {
        if (!first) return second;
        if (!second) return first;

        // For a max-heap, if first < second => swap so that first is the bigger root
        if (this->compare(first->elt, second->elt)) {
            std::swap(first, second);
        }
        // Attach `second` as a child of `first`
        // Insert `second` at the head of first->child list
        second->parent = first;
        second->prev   = nullptr;
        second->sibling = first->child;

        if (first->child) {
            first->child->prev = second;
        }
        first->child = second;

        return first;
    }

    // Collect all nodes in a DFS manner
    void collectNodes(Node *node, std::deque<Node*> &nodes) {
        if (!node) return;
        std::deque<Node*> stack;
        stack.push_back(node);

        while (!stack.empty()) {
            Node *curr = stack.back();
            stack.pop_back();

            nodes.push_back(curr);
            if (curr->child)   stack.push_back(curr->child);
            if (curr->sibling) stack.push_back(curr->sibling);
        }
    }

    // Recursively free all nodes
    void clear(Node *node) {
        if (!node) return;
        clear(node->child);
        clear(node->sibling);
        delete node;
    }
};

#endif  // PAIRINGPQ_H
