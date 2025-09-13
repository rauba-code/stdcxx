#include "algorithm.h"
#include "iterator.h"
#include "src/set.h"
#include "vector.h"
#include <stdio.h>

void binary_search(const size_t n, const int A[], const int &value) {
  size_t a = 0;
  size_t c = n;
  while (a < c) {
    size_t b = (a + c) / 2;
    if (A[b] < value) {
      a = b + 1;
    } else {
      c = b;
    }
  }
  printf("%d: A[%zu] (%d) %c\n", value, a, (a < n) ? A[a] : -1,
         (a < n && A[a] == value) ? '!' : ' ');
  // return this->_values.begin() + a;
}

void test_vector() {
  vector<int> x;
  x.push_back(1);
  x.push_back(4);
  x.push_back(1);
  x.insert(x.end(), 5);
  x.insert(x.begin(), 3);
  fill_n(x.begin() + 1, 2, 0);
  fill_n(inserter(x, x.end()), 3, 2);
  for (auto i : x) {
    // const char buf[] = {static_cast<char>('0' + i), 10};
    printf("%d\n", i);
  }
}

int main() {
  const int A[] = {2,  3,  5,  7,  11, 13, 17, 19, 23, 29, 31, 37, 41,
                   43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
  for (int i = 0; i < 100; i++) {
    binary_search(sizeof(A) / sizeof(*A), A, i);
  }
  int x = 1;
  set<int> s;
  do {
    x *= 17;
    x %= 23;
    s.insert(A[x - 1]);
    for (int i : s) {
      printf("%d ", i);
    }
    printf("\n");
  } while (x != 1);
}
