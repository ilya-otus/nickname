#pragma once
#include <stdexcept>
#include <map>
#include <functional>
#include "radix_helpers.h"

namespace Patricia {

template <typename K, typename V, class C> class RadixNode;

template <typename K, typename V, typename C>
RadixNode<K, V, C>* ascend(RadixNode<K, V, C> *node);

template <typename K, typename V, typename C>
RadixNode<K, V, C>* descend(RadixNode<K, V, C> *node);

template <typename K, typename V, class C>
RadixNode<K, V, C>* begin(RadixNode<K, V, C> *node);

template <typename K, typename V, class C>
RadixNode<K, V, C>* findNode(const K &key, RadixNode<K, V, C> *node, size_t depth);

template <typename K, typename V, class C>
RadixNode<K, V, C>* append(RadixNode<K, V, C> *node, const typename RadixNode<K, V, C>::value_type &value);

template <typename K, typename V, class C>
RadixNode<K, V, C>* prepend(RadixNode<K, V, C> *node, const typename RadixNode<K, V, C>::value_type &value);

template <typename K, typename V, typename C>
class RadixNode {
public:
    using value_type = std::pair<const K, V>;
    using children_type = std::map<K, RadixNode<K, V, C> *, C>;

    explicit RadixNode(C &predicate);
    RadixNode(const value_type &value, C &predicate);
    ~RadixNode();
    K& key();
    void setKey(const K &key);
    value_type& value() const;
    value_type* valuePtr() const;
    bool isLeaf() const;
    size_t depth() const;
    void setDepth(size_t depth);
    RadixNode* parent();
    void setParent(RadixNode *node);
    void erase(const K &key);
    size_t size() const;
    bool empty() const;
    const children_type& children() const;
    void setChild(const K &key, RadixNode *node);

    template <typename K_, typename V_, class C_>
    friend RadixNode<K_, V_, C_>* ascend(RadixNode<K_, V_, C_> *node);
    template <typename K_, typename V_, class C_>
    friend RadixNode<K_, V_, C_>* descend(RadixNode<K_, V_, C_> *node);

    template <typename K_, typename V_, class C_>
    friend RadixNode<K_, V_, C_>* begin(RadixNode<K_, V_, C_> *node);
    template <typename K_, typename V_, class C_>
    friend RadixNode<K_, V_, C_>* findNode(const K_ &key, RadixNode<K_, V_, C_> *node, size_t depth);
    template <typename K_, typename V_, class C_>
    friend RadixNode<K_, V_, C_>* append(RadixNode<K_, V_, C_> *node, const typename RadixNode<K_, V_, C_>::value_type &value);
    template <typename K_, typename V_, class C_>
    friend RadixNode<K_, V_, C_>* prepend(RadixNode<K_, V_, C_> *node, const typename RadixNode<K_, V_, C_>::value_type &value);
private:
    RadixNode(const RadixNode &) = delete;
    RadixNode& operator=(const RadixNode &) = delete;
private:
    children_type mChildren;
    RadixNode<K, V, C> *mParent;
    K mKey;
    value_type *mValue;
    C &mPredicate;
    size_t mDepth;
    bool mIsLeaf;
};

template <typename K, typename V, typename C>
RadixNode<K, V, C>::RadixNode(C &predicate) 
    : mChildren(std::map<K,
                RadixNode<K, V, C>*,
                C>(predicate)),
    mParent(nullptr),
    mKey(),
    mValue(nullptr),
    mPredicate(predicate),
    mDepth(0),
    mIsLeaf(false) { }

template <typename K, typename V, typename C>
RadixNode<K, V, C>::RadixNode(const value_type &value, C &predicate) 
    : mChildren(std::map<K,
                RadixNode<K, V, C>*,
                C>(predicate)),
    mParent(nullptr),
    mKey(),
    mValue(new value_type(value)),
    mPredicate(predicate),
    mDepth(0),
    mIsLeaf(false) { }


template <typename K, typename V, typename C>
RadixNode<K, V, C>::~RadixNode() {
    for (auto it = mChildren.begin(); it != mChildren.end(); ++it) {
        delete it->second;
    }
    if (mValue != nullptr) {
        delete mValue;
    }
}

template <typename K, typename V, typename C>
auto RadixNode<K, V, C>::value() const -> value_type& {
    return *mValue;
}

template <typename K, typename V, typename C>
auto RadixNode<K, V, C>::valuePtr() const -> value_type* {
    return mValue;
}

template <typename K, typename V, class C>
RadixNode<K, V, C>* ascend(RadixNode<K, V, C> *node) {
    if (node->mParent == nullptr) {
        return nullptr;
    }
    auto it = node->mParent->mChildren.find(node->mKey);
    if (it == node->mParent->mChildren.end()) {
        throw std::out_of_range("Calling method ascend with externall node");
    }
    ++it;
    if (it == node->mParent->mChildren.end()) {
        return ascend(node->mParent);
    } else {
        return descend(it->second);
    }

}
template <typename K, typename V, class C>
RadixNode<K, V, C>* descend(RadixNode<K, V, C> *node) {
    if (node->mIsLeaf) {
        return node;
    }
    auto it = node->mChildren.begin();
    if (it == node->mChildren.end()) {
        throw std::length_error("Node doesn't store any child");
    }
    return descend(it->second);
}

template <typename K, typename V, class C>
RadixNode<K, V, C>* begin(RadixNode<K, V, C> *node) {
    if (node->mIsLeaf) {
        return node;
    }
    if (node->mChildren.empty()) {
        throw std::length_error("Node doesn't store any child");
    }
    return begin(node->mChildren.begin()->second);
}

template <typename K, typename V, class C>
RadixNode<K, V, C>* findNode(const K &key, RadixNode<K, V, C> *node, size_t depth) {
    if (node->mChildren.empty()) {
        return node;
    }
    size_t size = radixSize(key) - depth;
    for (auto it = node->mChildren.begin(); it != node->mChildren.end(); ++it) {
        if (size == 0) {
            if (it->second->mIsLeaf) {
                return it->second;
            } else {
                continue;
            }
        }
        if (!it->second->mIsLeaf && key[depth] == it->first[0]) {
            size_t childSize = radixSize(it->first);
            K childKey = radixSubstr(key, depth, childSize);
            if (childKey == it->first) {
                return findNode(key, it->second, depth + childSize);
            } else {
                return it->second;
            }
        }
    }
    return node;
}

template <typename K, typename V, class C>
RadixNode<K, V, C>* append(RadixNode<K, V, C> *node, const typename RadixNode<K, V, C>::value_type &value) {
    auto defaultKey = radixSubstr(value.first, 0, 0);
    size_t depth = node->mDepth + radixSize(node->mKey);
    size_t size = radixSize(value.first) - depth;

    auto newNode = new RadixNode<K, V, C>(value, node->mPredicate);
    newNode->mParent = node;
    newNode->mDepth = depth;
    if (size == 0) {
        newNode->mKey = defaultKey;
        newNode->mIsLeaf = true;
        node->mChildren[defaultKey] = newNode;
        return newNode;
    }
    auto newKey = radixSubstr(value.first, depth, size);
    newNode->mKey = newKey;
    node->mChildren[newKey] = newNode;

    auto newChildNode = new RadixNode<K, V, C>(value, node->mPredicate);
    newChildNode->mParent = newNode;
    newChildNode->mDepth = depth + size;
    newChildNode->mKey = defaultKey;
    newChildNode->mIsLeaf = true;
    newNode->mChildren[defaultKey] = newChildNode;
    return newChildNode;
}

template <typename K, typename V, class C>
RadixNode<K, V, C>* prepend(RadixNode<K, V, C> *node, const typename RadixNode<K, V, C>::value_type &value) {
    size_t nodeSize = radixSize(node->mKey);
    size_t valueSize = radixSize(value.first) - node->mDepth;
    size_t count;
    for (count = 0; count < nodeSize && count < valueSize; ++count) {
        if (!(node->mKey[count] == value.first[count + node->mDepth])) {
            break;
        }
    }
    if (count == 0) {
        throw std::logic_error("Trying to prepend inconsistant node");
    }
    node->mParent->mChildren.erase(node->mKey);
    auto newParentNode = new RadixNode<K, V, C>(node->mPredicate);
    newParentNode->mParent = node->mParent;
    newParentNode->mDepth = node->mDepth;
    newParentNode->mKey = radixSubstr(node->mKey, 0, count);
    newParentNode->mParent->mChildren[newParentNode->mKey] = newParentNode;

    node->mParent = newParentNode;
    node->mDepth += count;
    node->mKey = radixSubstr(node->mKey, count, nodeSize - count);
    node->mParent->mChildren[node->mKey] = node;

    auto defaultKey = radixSubstr(value.first, 0, 0);
    auto newNode = new RadixNode<K, V, C>(value, node->mPredicate);
    newNode->mParent = newParentNode;
    newNode->mDepth = newParentNode->mDepth + count;
    if (count == valueSize) {
        newNode->mKey = defaultKey;
        newNode->mIsLeaf = true;
        newParentNode->mChildren[defaultKey] = newNode;
        return newNode;
    }
    auto newKey = radixSubstr(value.first, newNode->mDepth, valueSize - count);
    newNode->mKey = newKey;
    newParentNode->mChildren[newKey] = newNode;

    auto newChildNode = new RadixNode<K, V, C>(value, node->mPredicate);
    newChildNode->mDepth = radixSize(value.first);
    newChildNode->mParent = newNode;
    newChildNode->mKey = defaultKey;
    newChildNode->mIsLeaf = true;
    newNode->mChildren[defaultKey] = newChildNode;
    return newChildNode;
}

template <typename K, typename V, class C>
void RadixNode<K, V, C>::erase(const K &key) {
    mChildren.erase(key);
}

template <typename K, typename V, class C>
size_t RadixNode<K, V, C>::size() const {
    return mChildren.size();
}

template <typename K, typename V, class C>
bool RadixNode<K, V, C>::empty() const {
    return mChildren.empty();
}

template <typename K, typename V, class C>
auto RadixNode<K, V, C>::children() const -> const children_type& {
    return mChildren;
}

template <typename K, typename V, class C>
void RadixNode<K, V, C>::setChild(const K &key, RadixNode<K, V, C> *node) {
    mChildren[key] = node;
}

template <typename K, typename V, class C>
bool RadixNode<K, V, C>::isLeaf() const {
    return mIsLeaf;
}

template <typename K, typename V, typename C>
K& RadixNode<K, V, C>::key() {
    return mKey;
}

template <typename K, typename V, typename C>
void RadixNode<K, V, C>::setKey(const K &key) {
    mKey = key;
}

template <typename K, typename V, typename C>
size_t RadixNode<K, V, C>::depth() const {
    return mDepth;
}

template <typename K, typename V, typename C>
void RadixNode<K, V, C>::setDepth(size_t depth) {
    mDepth = depth;
}

template <typename K, typename V, typename C>
RadixNode<K, V, C>* RadixNode<K, V, C>::parent() {
    return mParent;
}

template <typename K, typename V, typename C>
void RadixNode<K, V, C>::setParent(RadixNode<K, V, C> *node) {
    mParent = node;
}

} //namespace Patricia
