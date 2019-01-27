#pragma once

#include <iterator>
#include <functional>

namespace Patricia {
template <typename K, typename V, class C = std::less<K> > class RadixNode;

template <typename K, typename V, class C = std::less<K> >
class RadixIter : public std::iterator<std::forward_iterator_tag, std::pair<K, V>> {
    using value_type = typename RadixNode<K, V, C>::value_type;
public:
    RadixIter();
    RadixIter(RadixNode<K, V, C> *node);
    RadixIter(const RadixIter &it);
    RadixIter &operator=(const RadixIter &it);
    ~RadixIter() = default;

    value_type& operator*() const;
    value_type* operator->() const;
    const RadixIter<K, V, C>& operator++(); // prefix
    RadixIter<K, V, C>& operator++(int); // postfix
    bool operator!=(const RadixIter<K, V, C> &other) const;
    bool operator==(const RadixIter<K, V, C> &other) const;
    RadixNode<K, V, C> *node();
private:
    RadixNode<K, V, C> *mPointed;
};

template <typename K, typename V, class C>
RadixIter<K, V, C>::RadixIter()
    : mPointed(nullptr)
{ }

template <typename K, typename V, class C>
RadixIter<K, V, C>::RadixIter(const RadixIter &it)
    : mPointed(it.mPointed)
{ }

template <typename K, typename V, class C>
RadixIter<K, V, C>& RadixIter<K, V, C>::operator=(const RadixIter &it) {
    mPointed = it.mPoined;
}

template <typename K, typename V, class C>
RadixIter<K, V, C>::RadixIter(RadixNode<K, V, C> *node)
    : mPointed(node)
{ }

template <typename K, typename V, class C>
auto RadixIter<K, V, C>::operator*() const -> value_type& {
    return mPointed->value();
}

template <typename K, typename V, class C>
auto RadixIter<K, V, C>::operator->() const -> value_type* {
    return mPointed->valuePtr();
}

template <typename K, typename V, class C>
const RadixIter<K, V, C>& RadixIter<K, V, C>::operator++() { // prefix
    if (mPointed != nullptr) {
        mPointed = RadixNode<K, V, C>::ascend(mPointed);
    }
    return *this;
}

template <typename K, typename V, class C>
RadixIter<K, V, C>& RadixIter<K, V, C>::operator++(int) { // postfix
    RadixIter<K, V, C> copy(*this);
    ++(*this);
    return copy;
}

template <typename K, typename V, class C>
RadixNode<K, V, C>* RadixIter<K, V, C>::node() {
    return mPointed;
}

template <typename K, typename V, class C>
bool RadixIter<K, V, C>::operator!=(const RadixIter<K, V, C> &other) const {
    return mPointed != other.mPointed;
}

template <typename K, typename V, class C>
bool RadixIter<K, V, C>::operator==(const RadixIter<K, V, C> &other) const {
    return mPointed == other.mPointed;
}
}
