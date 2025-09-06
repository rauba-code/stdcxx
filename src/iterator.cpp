#include "iterator.h"

template <class Iter>
typename reverse_iterator<Iter>::reference
reverse_iterator<Iter>::operator*() const {
  Iter tmp = current;
  return *--tmp;
}

template <class Iter>
typename reverse_iterator<Iter>::pointer
reverse_iterator<Iter>::operator->() const {
  return &(operator*());
}

template <class Iter>
reverse_iterator<Iter> &reverse_iterator<Iter>::operator++() {
  --current;
  return *this;
}

template <class Iter>
reverse_iterator<Iter> &reverse_iterator<Iter>::operator--() {
  ++current;
  return *this;
}

template <class Iter1, class Iter2>
bool operator==(const reverse_iterator<Iter1> &lhs,
                const reverse_iterator<Iter2> &rhs) {
  return lhs.base() == rhs.base();
}

template <class Iter1, class Iter2>
bool operator!=(const reverse_iterator<Iter1> &lhs,
                const reverse_iterator<Iter2> &rhs) {
  return lhs.base() == rhs.base();
}

template <class Iter>
reverse_iterator<Iter>::reverse_iterator(
    typename reverse_iterator<Iter>::iterator_type x) {
  this->current = x;
}

template <class Iter>
typename reverse_iterator<Iter>::iterator_type
reverse_iterator<Iter>::base() const {
  return current;
}

template <class Iter> reverse_iterator<Iter> make_reverse_iterator(Iter i);

// --- insert_iterator

template <class Container>
insert_iterator<Container>::insert_iterator(Container &c,
                                            typename Container::iterator i) {
  this->container = &c;
  this->iter = i;
}

template <class Container>
insert_iterator<Container> &insert_iterator<Container>::operator=(
    const typename Container::value_type &value) {
  this->iter = container->insert(this->iter, value);
  ++(this->iter);
  return *this;
}

template <class Container>
insert_iterator<Container> &insert_iterator<Container>::operator*() {
  return *this;
}

template <class Container>
insert_iterator<Container> &insert_iterator<Container>::operator++() {
  return *this;
}

template <class Container>
insert_iterator<Container> &insert_iterator<Container>::operator++(int) {
  return *this;
}

template <class Container>
insert_iterator<Container> inserter(Container &c,
                                    typename Container::iterator i) {
  return insert_iterator<Container>(c, i);
}

#include "vector.h"

template insert_iterator<vector<int>>
inserter<vector<int>>(vector<int> &, vector<int>::iterator);
template class insert_iterator<vector<int>>;
