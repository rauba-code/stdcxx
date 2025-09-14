#ifndef __SRC_STRING_H
#define __SRC_STRING_H

#include "cstddef.h"
#include "cstring.h"
#include "iterator.h"
#include "functional.h"

struct string {
public:
  typedef char value_type;
  typedef size_t size_type;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef value_type *pointer;
  typedef const value_type *const_pointer;
  typedef value_type *iterator;
  typedef const value_type *const_iterator;

  static constexpr size_type npos = -1;

  string();
  string(size_type, char);
  template <class InputIt> string(InputIt first, InputIt last) {
    size_t i = 0;
    size_t cap = 8;
    char *arr = new char[cap];
    while (first != last) {
        if (i >= cap) {
            char *narr = new char[cap * 2];
            for (size_t j = 0; j < i; j++) {
                narr[j] = arr[i];
            }
            delete[] arr;
            arr = narr;
            cap *= 2;
        }
        arr[i] = *first;
        i++;
        next(first);
    }
    this->_ptr = arr;
    this->_sz = i;
    this->_is_owner = true;
  }
  string(const char *);
  string(const char *, size_type);
  string(const string&);
  string(const string&, size_type);
  string(const string&, size_type, size_type);
  ~string();

  iterator begin();
  iterator end();

private:
  value_type *_ptr;
  size_type _sz;
  bool _is_owner;
};

template <> struct hash<string> {
    size_t operator()(string v) {
        // very rudimentary hash
        size_t h = 7919;
        for (char c : v) {
            h += 97;
            h += c * 7907;
            h %= 1000000009;
        }
        return h;
    }
};

#endif
