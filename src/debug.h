#ifndef __SRC_DEBUG_H
#define __SRC_DEBUG_H

#define PRINT

#ifdef PRINT
#include <stdio.h>

template <class T> struct _print {};

#define __print_decl(T, FMT)                                                   \
  template <> struct _print<T> {                                               \
    void operator()(const T &x, int margin = 0) {                              \
      (void)(margin);                                                          \
      printf(FMT, x);                                                          \
    }                                                                          \
  }

__print_decl(bool, "%d");
__print_decl(char, "%d");

__print_decl(signed char, "%d");
__print_decl(unsigned char, "%u");
__print_decl(short int, "%d");
__print_decl(unsigned short int, "%u");
__print_decl(int, "%d");
__print_decl(unsigned int, "%u");
__print_decl(long int, "0x%lx");
__print_decl(unsigned long int, "0x%lx");
__print_decl(long long int, "0x%llx");
__print_decl(unsigned long long int, "0x%llx");

__print_decl(float, "%f");
__print_decl(double, "%f");

template <class T> struct _print<T *> {
  void operator()(T *const &x, int margin = 0) {
    (void)(margin);
    if (x) {
      _print<T>{}(*x, margin);
    } else {
      printf("nullptr");
    }
  }
};

#endif

#endif
