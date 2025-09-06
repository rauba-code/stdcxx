#ifndef __SRC_VECTOR_H
#define __SRC_VECTOR_H

#include <sys/types.h> // size_t

template <class T> class vector {
public:
  typedef T value_type;
  typedef T *iterator;
  typedef const T *const_iterator;
  typedef T *pointer;
  typedef value_type &reference;
  // typedef reverse_iterator<const_iterator> const_reverse_iterator;
  // typedef reverse_iterator<iterator> reverse_iterator;

  vector();
  ~vector();
  void push_back(T &&);

  iterator begin();
  iterator end();

  iterator insert(const_iterator pos, const T &value);

private:
  size_t _size;
  size_t _cap;
  T *_ptr;
  void _grow(size_t);
};

extern template class vector<int>;

#endif
