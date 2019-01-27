#pragma once

#include "radix_iter.h"
#include "radix_node.h"
#include "radix_helpers.h"
#include <string>

namespace Patricia {

using namespace std::string_literals;
template <typename K, typename V, typename C = std::less<K>>
class RadixTrie {
    using key_type = K;
    using mapped_type = V;
    using value_type = typename RadixNode<K, V, C>::value_type;
    using iterator = RadixIter<K, V, C>;
    using size_type = std::size_t;
public:
    RadixTrie();
    RadixTrie(C predicate);
    ~RadixTrie();
    size_type size() const;
    bool empty() const;
    void clear();

    iterator find(const K &key);
    iterator begin();
    iterator end();

    std::pair<iterator, bool> insert(const value_type &value);
    bool erase(const K &key);
    void erase(iterator it);
    void prefixMatch(const K &key, std::vector<iterator> &result);
    void dump();
private:
    void dump(RadixNode<K, V, C> *node, const std::string &prefix = ""s);
    RadixNode<K, V, C> *mRoot;
    size_t mSize;
    C mPredicate;
};

template <typename K, typename V, typename C>
RadixTrie<K, V, C>::RadixTrie()
    : mRoot(nullptr),
    mSize(0),
    mPredicate(C()) { }

template <typename K, typename V, typename C>
RadixTrie<K, V, C>::RadixTrie(C predicate)
    : mRoot(nullptr),
    mSize(0),
    mPredicate(predicate) { }

template <typename K, typename V, typename C>
RadixTrie<K, V, C>::~RadixTrie() {
    delete mRoot;
}

template <typename K, typename V, typename C>
auto RadixTrie<K, V, C>::size() const -> size_type {
    return mSize;
}

template <typename K, typename V, typename C>
bool RadixTrie<K, V, C>::empty() const {
    return mSize == 0;
}

template <typename K, typename V, typename C>
void RadixTrie<K, V, C>::clear() {
    delete mRoot;
    mRoot = nullptr;
    mSize = 0;
}

template <typename K, typename V, typename C>
auto RadixTrie<K, V, C>::find(const K &key) -> iterator {
    if (mRoot == nullptr) {
        return iterator(nullptr);
    }
    auto node = RadixNode<K, V, C>::findNode(key, mRoot, 0);
    if (!node->isLeaf()) {
        return iterator(nullptr);
    }
    return iterator(node);
}

template <typename K, typename V, typename C>
auto RadixTrie<K, V, C>::begin() -> iterator {
    if (mRoot == nullptr || mSize == 0) {
        return iterator(nullptr);
    }
    return iterator(RadixNode<K, V, C>::begin(mRoot));
}

template <typename K, typename V, typename C>
auto RadixTrie<K, V, C>::end() -> iterator {
    return iterator(nullptr);
}

template <typename K, typename V, typename C>
auto RadixTrie<K, V, C>::insert(const value_type &value) -> std::pair<iterator, bool> {
    if (mRoot == nullptr) {
        auto defaultKey = radixSubstr(value.first, 0, 0);
        mRoot = new RadixNode<K, V, C>(mPredicate);
        mRoot->setKey(defaultKey);
    }
    auto node = RadixNode<K, V, C>::findNode(value.first, mRoot, 0);
    if (node->isLeaf()) {
        return {node, false};
    } else if (node == mRoot) {
        ++mSize;
        return {RadixNode<K, V, C>::append(mRoot, value), true};
    }
    ++mSize;
    int size = radixSize(node->key());
    auto childKey = radixSubstr(value.first, node->depth(), size);
    if (childKey == node->key()) {
        return {RadixNode<K, V, C>::append(node, value), true};
    }
    return {RadixNode<K, V, C>::prepend(node, value), true};
}

template <typename K, typename V, typename C>
bool RadixTrie<K, V, C>::erase(const K &key) {
    if (mRoot == nullptr) {
        return false;
    }
    auto defaultKey = radixSubstr(key, 0, 0);
    auto child = RadixNode<K, V, C>::findNode(key, mRoot, 0);
    if (!child->isLeaf()) {
        return false;
    }

    auto parent = child->parent();
    parent->erase(defaultKey);
    delete child;
    --mSize;
    if (parent == mRoot) {
        return true;
    }
    if (parent->size() > 1) {
        return true;
    }

    auto grandparent = parent->parent();
    if (parent->empty()) {
        grandparent->erase(parent->key());
        delete parent;
    } else {
        grandparent = parent;
    }
    if (grandparent == mRoot) {
        return true;
    }
    if (grandparent->size() == 1) {
        auto uncle = RadixNode<K, V, C>::begin(grandparent);
        if (uncle->isLeaf()) {
            return true;
        }
        auto oldKey = uncle->key();
        uncle->setDepth(grandparent->depth());
        uncle->setKey(radixJoin(grandparent->key(), uncle->key()));
        uncle->setParent(grandparent->parent());

        grandparent->erase(uncle->key());
        auto uncleParent = uncle->parent();
        uncleParent->erase(grandparent->key());
        uncleParent->setChild(grandparent->key(), uncle);
        delete grandparent;
    }
    return true;
}

template <typename K, typename V, typename C>
void RadixTrie<K, V, C>::erase(iterator it) {
    erase(it->first);
}

template <typename K, typename V, typename C>
void RadixTrie<K, V, C>::dump() {
    dump(mRoot);
}

template <typename K, typename V, typename C>
void RadixTrie<K, V, C>::dump(RadixNode<K, V, C> *node, const std::string &prefix) {
    auto parent = node->parent();
    if (node->key().empty() && node->isLeaf()) {
        return;
    }
    std::string pref = prefix;
    if (parent != nullptr) {
        auto lastPtr = parent->children().rbegin()->second;
        if (node != lastPtr) {
            pref += "| "s;
        } else {
            pref += "  "s;
        }
        std::cout << prefix << "+ ";
    }
    std::cout << node->key();
    auto defaultKey = radixSubstr(node->key(), 0, 0);
    if (auto sign = node->children().find(defaultKey); sign != node->children().end() && sign->second->isLeaf()) {
        std::cout << "$";
    }
    std::cout << std::endl;

    for (auto child : node->children()) {
        dump(child.second, pref);
    }
}

} // namespace Patricia
