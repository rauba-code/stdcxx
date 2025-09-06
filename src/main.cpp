#include "algorithm.h"
#include "iterator.h"
#include "vector.h"
#include <stdio.h>

int main() {
  vector<int> x;
  x.push_back(1);
  x.push_back(4);
  x.push_back(1);
  x.insert(x.end(), 5);
  x.insert(x.begin(), 3);
  fill_n(x.begin() + 1, 2, 0);
  fill_n(inserter(x, x.end()), 3, 2);
  for (auto i : x) {
    //const char buf[] = {static_cast<char>('0' + i), 10};
    printf("%d\n", i);
  }
}
