#ifndef __SRC_SET_H
#define __SRC_SET_H
#include "vector.h"

template <class Key> class set {
public:
  typedef Key key_type;
  typedef Key value_type;
  typedef Key *iterator;
  typedef const Key *const_iterator;
  typedef Key *pointer;
  typedef value_type &reference;

  set();
  ~set();

  iterator insert(const value_type &value);
  iterator insert(const_iterator pos, const value_type &value);

  iterator begin();
  iterator end();

private:
  vector<Key> _values;
};

template <class T> set<T>::set() {}
template <class T> set<T>::~set() {}

template <class T> typename set<T>::iterator set<T>::begin() {
  return this->_values.begin();
}

template <class T> typename set<T>::iterator set<T>::end() {
  return this->_values.end();
}

template <class Key>
typename set<Key>::iterator
set<Key>::insert(const set<Key>::value_type &value) {
  size_t a = 0;
  size_t c = this->_values.size();
  while (a < c) {
    size_t b = (a + c) / 2;
    if (value < this->_values[b]) {
      c = b;
    } else {
      a = b + 1;
    }
  }
  return this->_values.insert(this->_values.begin() + a, value);
}

#endif
