#ifndef __SRC_FUCTIONAL_H
#define __SRC_FUCTIONAL_H

#include "cstddef.h"

template <class Key> struct hash;

#define _hash_arithmetic_decl(T)                                               \
  template <> struct hash<T> {                                                 \
    size_t operator()(T v) { return v; }                                       \
  }

_hash_arithmetic_decl(bool);
_hash_arithmetic_decl(char);

_hash_arithmetic_decl(signed char);
_hash_arithmetic_decl(unsigned char);
_hash_arithmetic_decl(short int);
_hash_arithmetic_decl(unsigned short int);
_hash_arithmetic_decl(int);
_hash_arithmetic_decl(unsigned int);
_hash_arithmetic_decl(long int);
_hash_arithmetic_decl(unsigned long int);
_hash_arithmetic_decl(long long int);
_hash_arithmetic_decl(unsigned long long int);

_hash_arithmetic_decl(float);
_hash_arithmetic_decl(double);

#endif
