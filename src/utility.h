#ifndef __SRC_UTILITY_H
#define __SRC_UTILITY_H

template <class T1, class T2> struct pair {
  T1 first;
  T2 second;
};

#define PRINT

#ifdef PRINT
#include "debug.h"
#include <stdio.h>
template <class T1, class T2> struct _print<pair<T1, T2>> {
  void operator()(const pair<T1, T2> &x, int margin = 0) {
    char *m = new char[margin + 1];
    for (int i = 0; i < margin; i++) {
      m[i] = ' ';
    }
    m[margin] = '\0';
    printf("{\n");
    printf("%s  .first = ", m);
    _print<T1>{}(x.first, margin + 2);
    printf(",\n");
    printf("%s  .second = ", m);
    _print<T2>{}(x.second, margin + 2);
    printf("\n");
    printf("%s}", m);
    delete[] m;
  }
};
#endif

#endif
