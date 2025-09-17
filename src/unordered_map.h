#ifndef __SRC_UNORDERED_MAP_H
#define __SRC_UNORDERED_MAP_H
#include "functional.h"
#include "iterator.h"
#include "utility.h"
#include <error.h>
#include <stdlib.h>
#include <sys/types.h> // size_t

#ifdef PRINT
#include <stdio.h>
#endif

#ifndef BITS_HASH
#define BITS_HASH 24
#endif
#ifndef BITS_BRANCH
#define BITS_BRANCH 4
#endif

const size_t STACK_SIZE = (BITS_HASH + BITS_BRANCH - 1) / BITS_BRANCH;

template <class Key, class T> class unordered_map_link_node;
template <class Key, class T> class unordered_map_branch_node;
// satisfies LegacyForwardIterator
template <class Key, class T> class unordered_map_iterator {
public:
  typedef pair<Key, T> value_type;
  typedef value_type *pointer;
  typedef value_type &reference;
  typedef forward_iterator_tag iterator_category;

  unordered_map_iterator() {
    this->slen = 0;
    this->link = nullptr;
  }

  unordered_map_iterator(unordered_map_branch_node<Key, T> *top) {
    this->slen = 1;
    this->bstack[0] = top;
    this->istack[0] = 0;
    this->link = nullptr;
    this->_next();
  }

  unordered_map_iterator(unordered_map_branch_node<Key, T> *bstack[STACK_SIZE],
                         int istack[STACK_SIZE],
                         unordered_map_link_node<Key, T> *link) {
    this->slen = STACK_SIZE;
    for (size_t i = 0; i < STACK_SIZE; i++) {
      this->bstack[i] = bstack[i];
      this->istack[i] = istack[i];
    }
    this->link = link;
  }

  reference operator*() {
    if (this->link) {
      return this->link->child;
    } else {
      error(1, 0, "Attempted dereference of a null-iterator");
      abort();
    }
  }

  unordered_map_iterator<Key, T> operator++() {
    // prefix
    this->_next();
    return *this;
  }

  unordered_map_iterator<Key, T> operator++(int) {
    // postfix
    unordered_map_iterator<Key, T> x = *this;
    this->_next();
    return x;
  }

  // private:
  void _next() {
    if (this->link && this->link->next) {
      this->link = this->link->next;
      return;
    }
    while (true) {
      if (this->slen < STACK_SIZE) {
        bool is_any = false;
        for (int i = this->istack[this->slen - 1]; i < (1 << BITS_BRANCH);
             i++) {
          if (this->bstack[this->slen - 1]->branches[i]) {
            this->istack[this->slen - 1] = i + 1;
            this->istack[this->slen] = 0;
            this->bstack[this->slen] =
                this->bstack[this->slen - 1]->branches[i];
            this->slen++;
            is_any = true;
            break;
          }
          this->istack[this->slen - 1]++;
        }
        if (!is_any) {
          this->slen--;
          if (this->slen == 0) {
            this->link = nullptr;
            return;
          }
        }
      } else {
        for (int i = this->istack[this->slen - 1]; i < (1 << BITS_BRANCH);
             i++) {
          if (this->bstack[this->slen - 1]->leaves[i]) {
            this->istack[this->slen - 1] = i + 1;
            this->link = this->bstack[this->slen - 1]->leaves[i];
            return;
          }
        }
        this->slen--;
      }
    }
  }
  unordered_map_branch_node<Key, T> *bstack[STACK_SIZE];
  int istack[STACK_SIZE];
  size_t slen;
  unordered_map_link_node<Key, T> *link;
};

template <class Key, class T> class unordered_map_branch_node {
public:
  // If true, this->leaves are null and this->branches are non-null;
  // If false, this->leaves are non-null and this->branches are null.
  unordered_map_branch_node<Key, T> *branches[1 << BITS_BRANCH];
  unordered_map_link_node<Key, T> *leaves[1 << BITS_BRANCH];
  size_t size;

  unordered_map_branch_node() {
    for (int i = 0; i < (1 << BITS_BRANCH); i++) {
      this->leaves[i] = nullptr;
      this->branches[i] = nullptr;
      this->size = 0;
    }
  }

  ~unordered_map_branch_node() { this->clear(); }

  void clear() {
    for (int i = 0; i < (1 << BITS_BRANCH); i++) {
      if (this->branches[i]) {
        delete this->branches[i];
        this->branches[i] = nullptr;
      }
      if (leaves[i]) {
        delete this->leaves[i];
        this->leaves[i] = nullptr;
      }
      this->size = 0;
    }
  }
};

template <class Key, class T> class unordered_map_link_node {
public:
  pair<Key, T> child;
  // If true, this->next is null;
  // If false, this->next is non-null.
  unordered_map_link_node<Key, T> *next;
};

template <class Key, class T>
bool operator==(unordered_map_iterator<Key, T> lhs,
                unordered_map_iterator<Key, T> rhs) {
  if (!lhs.link) {
    return (rhs.link == nullptr);
  }
  return lhs.link == rhs.link;
}

template <class Key, class T>
bool operator!=(unordered_map_iterator<Key, T> lhs,
                unordered_map_iterator<Key, T> rhs) {
  if (!lhs.link) {
    return (rhs.link != nullptr);
  }
  return lhs.link != rhs.link;
}

template <class Key, class T> class unordered_map {
public:
  typedef Key key_type;
  typedef T mapped_type;
  typedef pair<const Key, T> value_type;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef value_type *pointer;
  typedef const value_type *const_pointer;
  typedef unordered_map_iterator<Key, T> iterator;

  unordered_map() { this->tree = unordered_map_branch_node<Key, T>(); }

  ~unordered_map() {};

  iterator begin() { return iterator(&(this->tree)); }

  iterator end() { return iterator(); }

  T &operator[](const Key &key) {
    return this->insert({key, T{}}).first.operator*().second;
  }

  pair<iterator, bool> insert(const value_type &kv) {
    unordered_map_branch_node<Key, T> *bstack[STACK_SIZE];
    int istack[STACK_SIZE];
    bstack[0] = &this->tree;

    {
      size_t h = hash<Key>{}(kv.first);
      h &= (1 << BITS_HASH) - 1;
      for (int i = BITS_BRANCH; i < BITS_HASH; i += BITS_BRANCH) {
        istack[(i / BITS_BRANCH) - 1] = h & ((1 << BITS_BRANCH) - 1);
        h >>= BITS_BRANCH;
      }
      istack[STACK_SIZE - 1] = h;
    }
    for (size_t i = 1; i < STACK_SIZE; i++) {
      unordered_map_branch_node<Key, T> *&child =
          bstack[i - 1]->branches[istack[i - 1]];
      if (child == nullptr) {
        child = new unordered_map_branch_node<Key, T>();
      }
      bstack[i] = child;
    }
    int ilast = istack[STACK_SIZE - 1];
    unordered_map_branch_node<Key, T> *blast = bstack[STACK_SIZE - 1];
    if (blast->leaves[ilast] == nullptr) {
      blast->leaves[ilast] =
          new unordered_map_link_node<Key, T>{{kv.first, kv.second}, nullptr};
      for (unordered_map_branch_node<Key, T> *branch : bstack) {
        branch->size++;
      }
      return {iterator(bstack, istack, blast->leaves[ilast]), true};
    }
    unordered_map_link_node<Key, T> *link = blast->leaves[ilast];
    while (true) {
      if (link->child.first == kv.first) {
        return {iterator(bstack, istack, link), false};
      }
      if (link->next == nullptr) {
        link->next =
            new unordered_map_link_node<Key, T>{{kv.first, kv.second}, nullptr};
        for (unordered_map_branch_node<Key, T> *branch : bstack) {
          branch->size++;
        }
        return {iterator(bstack, istack, link->next), true};
      }
      link = link->next;
    }
  }

  iterator find(const Key &key) {
    unordered_map_branch_node<Key, T> *bstack[STACK_SIZE];
    int istack[STACK_SIZE];
    bstack[0] = &this->tree;

    {
      size_t h = hash<Key>{}(key);
      h &= (1 << BITS_HASH) - 1;
      for (int i = BITS_BRANCH; i < BITS_HASH; i += BITS_BRANCH) {
        istack[(i / BITS_BRANCH) - 1] = h & ((1 << BITS_BRANCH) - 1);
        h >>= BITS_BRANCH;
      }
      istack[STACK_SIZE - 1] = h;
    }
    for (size_t i = 1; i < STACK_SIZE; i++) {
      unordered_map_branch_node<Key, T> *&child =
          bstack[i - 1]->branches[istack[i - 1]];
      if (child == nullptr) {
        return this->end();
      }
      bstack[i] = child;
    }
    int ilast = istack[STACK_SIZE - 1];
    unordered_map_branch_node<Key, T> *blast = bstack[STACK_SIZE - 1];
    if (blast->leaves[ilast] == nullptr) {
      return this->end();
    }
    unordered_map_link_node<Key, T> *link = blast->leaves[ilast];
    while (true) {
      if (link->child.first == key) {
        return iterator(bstack, istack, link);
      }
      if (link->next == nullptr) {
        return this->end();
      }
      link = link->next;
    }
  }

  T &at(const Key &key) {
    iterator retv = this->find(key);
    if (retv == this->end()) {
      error(1, 0, "No such element (out_of_range)");
      abort();
    }
    return (*retv).first;
  }

  size_t size() { return this->tree.size; }

  bool empty() { return this->tree.size == 0; }

  void clear() { this->tree.clear(); }

  // private:
  unordered_map_branch_node<Key, T> tree;
};

#ifdef PRINT
#include "debug.h"
template <class Key, class T> struct _print<unordered_map<Key, T>> {
  void operator()(const unordered_map<Key, T> &x, int margin = 0) {
    char *m = new char[margin + 1];
    for (int i = 0; i < margin; i++) {
      m[i] = ' ';
    }
    m[margin] = '\0';
    printf("{\n");
    printf("%s  .tree = ", m);
    _print<unordered_map_branch_node<Key, T>>{}(x.tree, margin + 2);
    putc('\n', stdout);
    printf("%s}", m);
    delete[] m;
  }
};

template <class Key, class T> struct _print<unordered_map_branch_node<Key, T>> {
  void operator()(const unordered_map_branch_node<Key, T> &x, int margin = 0) {
    char *m = new char[margin + 1];
    for (int i = 0; i < margin; i++) {
      m[i] = ' ';
    }
    m[margin] = '\0';
    printf("{\n");
    printf("%s  .branches = {\n", m);
    for (int i = 0; i < (1 << BITS_BRANCH); i++) {
      printf("%s    [%d] = ", m, i);
      _print<unordered_map_branch_node<Key, T> *>{}(x.branches[i], margin + 4);
      printf((i + 1 < (1 << BITS_BRANCH)) ? ",\n" : "\n");
    }
    printf("%s  },\n", m);
    printf("%s  .leaves = {\n", m);
    for (int i = 0; i < (1 << BITS_BRANCH); i++) {
      printf("%s    [%d] = ", m, i);
      _print<unordered_map_link_node<Key, T> *>{}(x.leaves[i], margin + 4);
      printf((i + 1 < (1 << BITS_BRANCH)) ? ",\n" : "\n");
    }
    printf("%s  }\n", m);
    printf("%s}", m);
    delete[] m;
  }
};

template <class Key, class T> struct _print<unordered_map_link_node<Key, T>> {
  void operator()(const unordered_map_link_node<Key, T> &x, int margin = 0) {
    char *m = new char[margin + 1];
    for (int i = 0; i < margin; i++) {
      m[i] = ' ';
    }
    m[margin] = '\0';
    printf("{\n");
    printf("%s  .child = ", m);
    _print<pair<Key, T>>{}(x.child, margin + 2);
    printf(",\n");
    printf("%s  .next = ", m);
    _print<unordered_map_link_node<Key, T> *>{}(x.next, margin + 2);
    printf("\n");
    printf("%s}", m);
    delete[] m;
  }
};
#endif

#endif
