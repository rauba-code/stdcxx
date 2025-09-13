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

  size_t size();

  value_type &operator[](size_t index);

private:
  size_t _size;
  size_t _cap;
  T *_ptr;
  void _grow(size_t);
};

template <class T>
typename vector<T>::value_type &vector<T>::operator[](size_t index) {
  return this->_ptr[index];
}

template <class T> vector<T>::vector() {
  this->_ptr = nullptr;
  this->_size = 0;
}

template <class T> vector<T>::~vector() {
  if (this->_ptr != nullptr) {
    delete[] this->_ptr;
  }
}

template <class T> void vector<T>::_grow(size_t n) {
  if (this->_ptr == nullptr) {
    size_t &&new_cap = ((this->_size + n) < 8) ? 8 : n + this->_size;
    this->_ptr = new T[new_cap];
    this->_cap = new_cap;
  } else if (this->_cap < this->_size + n) {
    size_t &&new_cap = this->_cap * 2;
    T *&&new_ptr = new T[new_cap];
    for (size_t i = 0; i < this->_size; i++) {
      new_ptr[i] = this->_ptr[i];
    }
    delete[] this->_ptr;
    this->_ptr = new_ptr;
    this->_cap = new_cap;
  }
  this->_size += n;
}

template <class T> void vector<T>::push_back(T &&a) {
  this->_grow(1);
  this->_ptr[this->_size - 1] = a;
}

template <class T> typename vector<T>::iterator vector<T>::begin() {
  return this->_ptr;
}

template <class T> typename vector<T>::iterator vector<T>::end() {
  return this->_ptr + this->_size;
}

template <class T>
typename vector<T>::iterator vector<T>::insert(const_iterator pos,
                                               const T &value) {
  const_iterator oldptr = this->_ptr;
  this->_grow(1);
  const_iterator newpos = this->_ptr + (pos - oldptr);
  T *i = (this->end() - 1);
  while (i > newpos) {
    T tmp = *i;
    *i = *(i - 1);
    *(i - 1) = tmp;
    --i;
  }
  *i = value;
  return i;
}

template <class T> size_t vector<T>::size() { return this->_size; }

#endif
