#include "vector.h"

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
  this->_grow(1);
  T *i = (this->end() - 1);
  while (i > pos) {
    T tmp = *i;
    *i = *(i - 1);
    *(i - 1) = tmp;
    --i;
  }
  *i = value;
  return i;
}

template class vector<int>;
