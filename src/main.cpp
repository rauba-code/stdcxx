#define PRINT

#include "algorithm.h"
#include "functional.h"
#include "iterator.h"
#include "set.h"
#include "string.h"
#include "unordered_map.h"
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

void umap_test() {
  typedef unsigned int uint;
  const uint MOD = 1524181; // any prime number > 2^BITS_HASH
  const uint CP = 189757;   // any prime number
  const uint CM = 1657;     // any prime number
  // MOD * CM < 2^32
  unordered_map<uint, uint> umap;
  uint i = 0;
  uint k = 0;
  do {
    // printf("%d\n", k);
    if (!umap.insert({k, i}).second) {
      error(1, 0, "insert({%d, %d}) failed", k, i);
      abort();
    }
    k += CP;
    k %= MOD;
    i++;
  } while (k != 0);
  printf("cycled over %d numbers\n", i);
  printf("size = %zu\n", umap.size());
  i = 0;
  k = 1;
  //_print<unordered_map<uint, uint>>{}(umap);
  putc('\n', stdout);
  do {
    //printf("delete %d\n", k);
    unordered_map_iterator<uint, uint> delit = umap.find(k);
    if (delit == umap.end()) {
      error(1, 0, "find(%d) failed", k);
      abort();
    }
    umap.erase(delit);
    k *= CM;
    k %= MOD;
    i++;
  } while (k != 1);
  printf("erased %d numbers\n", i);
  i = 0;
  int cksum = 1227; // any number
  for (pair<uint, uint> p : umap) {
    if (i < 100) {
      printf("%d:%d ", p.first, p.second);
    }
    cksum *= CM;
    cksum %= MOD;
    cksum += p.first + (2309 * p.second);
    cksum %= MOD;
    i++;
  }
  printf("\ncksum = %d\n", cksum);
  printf("size = %zu\n", umap.size());
}

int main() {
  vector<string> duomenys;
  duomenys.push_back("lorem ipsum dolor sit amet");
  duomenys.push_back("consectetur adipiscing elit");
  duomenys.push_back("sed do eiusmod tempor");
  for (const string &i : duomenys) {
    _print<string>{}(i);
    printf(" %d\n", static_cast<int>(hash<string>{}(i)));
  }
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
      printf("%d ", (int)hash<int>{}(i));
    }
    printf("\n");
  } while (x != 1);
  unordered_map<string, size_t> umap;
  for (const string &x : duomenys) {
    umap.insert({x, hash<string>{}(x)});
  }
  _print<unordered_map<string, size_t>>{}(umap);
  putc('\n', stdout);
  unordered_map<size_t, size_t> umap2;
  umap2[31415] = hash<size_t>{}(31415);
  umap2[31415 + (1 << BITS_HASH)] = hash<size_t>{}(31415);
  umap2[31415] = 7;
  umap2[31415 + (1 << BITS_HASH)] = 14;
  umap.erase(umap.find("consectetur adipiscing elit"));
  //_print<unordered_map<size_t, size_t>>{}(umap2);
  // putc('\n', stdout);
  for (auto i : umap2) {
    _print<pair<size_t, size_t>>{}(i);
    putc('\n', stdout);
  }
  printf("size: %zu\n", umap2.size());
  putc('\n', stdout);
  for (auto i : umap) {
    _print<pair<string, size_t>>{}(i);
    putc('\n', stdout);
  }
  printf("size: %zu\n", umap.size());
  _print<pair<string, size_t>>{}(*umap.find("lorem ipsum dolor sit amet"));
  putc('\n', stdout);
  umap.clear();
  _print<unordered_map<string, size_t>>{}(umap);
  putc('\n', stdout);
  printf("size: %zu\n", umap.size());
  umap_test();
}
