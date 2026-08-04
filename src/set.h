#ifndef __SRC_SET_H
#define __SRC_SET_H

#include "iterator.h"
#include "utility.h"
#include <error.h>
#include <stdio.h>     // debug output
#include <stdlib.h>    // abort
#include <sys/types.h> // size_t

template <class Key> class set_node {
public:
  set_node<Key> *lhs;
  set_node<Key> *rhs;
  set_node<Key> *parent;
  bool is_red;
  Key key;

  ~set_node() {
    if (this->lhs) {
      delete lhs;
    }
    if (this->rhs) {
      delete rhs;
    }
  }
};

// satisfies LegacyForwardIterator
template <class Key> class set_iterator {
public:
  typedef Key value_type;
  typedef value_type *pointer;
  typedef value_type &reference;
  typedef forward_iterator_tag iterator_category;

  set_node<Key> *node;

  set_iterator() : node(nullptr) {}
  set_iterator(set_node<Key> *node) : node(node) {}

  set_iterator(const set_iterator<Key> &x) : node(x.node) {}

  static set_iterator<Key> from_root(set_node<Key> *top) {
    if (!top) {
      return set_iterator();
    }
    while (top->lhs) {
      top = top->lhs;
    }
    return set_iterator(top);
  }

  reference operator*() const {
    if (this->node) {
      return this->node->key;
    } else {
      error(1, 0, "Attempted dereference of a null-iterator");
      abort();
    }
  }

  set_iterator<Key> operator++() {
    // prefix
    this->_next();
    return *this;
  }

  set_iterator<Key> operator++(int) {
    // postfix
    set_iterator<Key> x = *this;
    this->_next();
    return x;
  }

  // private:
  void _next() {
    if (!this->node) {
      // UB?
      return;
    }
    if (this->node->rhs) {
      this->node = this->node->rhs;
      while (this->node->lhs) {
        this->node = this->node->lhs;
      }
    } else {
      for (;;) {
        if (!this->node->parent) {
          this->node = nullptr;
          break;
        }
        if (this->node == this->node->parent->lhs) {
          this->node = this->node->parent;
          break;
        }
        this->node = this->node->parent;
      }
    }
  }
};

template <class Key>
bool operator==(const set_iterator<Key> &lhs, const set_iterator<Key> &rhs) {
  if (!lhs.node) {
    return (rhs.node == nullptr);
  }
  return lhs.node == rhs.node;
}

template <class Key>
bool operator!=(const set_iterator<Key> &lhs, const set_iterator<Key> &rhs) {
  if (!lhs.node) {
    return (rhs.node != nullptr);
  }
  return lhs.node != rhs.node;
}

template <class Key> class set {
public:
  typedef Key key_type;
  typedef Key value_type;
  typedef set_iterator<Key> iterator;
  typedef set_iterator<const Key> *const_iterator;
  typedef Key *pointer;
  typedef value_type &reference;
  typedef size_t size_type;

  set();
  ~set();

  iterator find(const Key &key);

  pair<iterator, bool> insert(const value_type &value);
  iterator insert(iterator pos, const value_type &value);
  template <class InputIt> void insert(InputIt first, InputIt last);

  iterator begin() const;
  iterator end() const;

  size_type size() const;

  iterator erase(iterator pos);
  // size_type erase(const Key &key);

  bool _verify() const;

  // private:
  set_node<Key> *root;
  size_type _size;
  static int _verify_inner(const set_node<Key> *node);
};

template <class Key> set<Key>::set() {
  this->root = nullptr;
  this->_size = 0;
}

template <class Key> set<Key>::~set() {
  if (this->root) {
    delete this->root;
  }
}

template <class Key> typename set<Key>::iterator set<Key>::begin() const {
  return set_iterator<Key>::from_root(this->root);
}

template <class Key> typename set<Key>::iterator set<Key>::end() const {
  return set_iterator<Key>();
}

template <class Key> typename set<Key>::size_type set<Key>::size() const {
  return this->_size;
}

template <class Key>
inline void rotate_right(set_node<Key> *g, set_node<Key> **g_ref) {
  // Case 6a.
  // [((n), p=R, [b?]), g=B, [u?]]
  // Rotate right.
  // ((n), p=R, [[b?], g=b, [u?]])
  set_node<Key> *p = g->lhs;
  set_node<Key> *b = p->rhs;      // opt
  set_node<Key> *ggp = g->parent; // opt
  g->lhs = b;
  if (b) {
    b->parent = g;
  }
  // ((n), p=R, !)    [[b?], g=B, [u?]]
  // g->rhs and u?->parent do not change
  // p->lhs and n?->parent do not change
  p->rhs = g;
  g->parent = p;

  p->parent = ggp;
  *g_ref = p;
}

template <class Key>
inline void rotate_left(set_node<Key> *g, set_node<Key> **g_ref) {
  // Case 6b.
  // [[u?], g=B, ([b?], p=R, (n))]
  // Rotate left.
  // ([[u?], g=b, [b?]], p=R, (n))
  set_node<Key> *p = g->rhs;
  set_node<Key> *b = p->lhs;      // opt
  set_node<Key> *ggp = g->parent; // opt
  g->rhs = b;
  if (b) {
    b->parent = g;
  }
  // g->lhs and u?->parent do not change
  // p->rhs and n?->parent do not change
  p->lhs = g;
  g->parent = p;

  p->parent = ggp;
  *g_ref = p;
}

template <class Key>
pair<typename set<Key>::iterator, bool>
set<Key>::insert(const set<Key>::value_type &value) {
  if (!this->root) {
    this->root = new set_node<Key>{nullptr, nullptr, nullptr, false, value};
    this->_size++;
    return {set_iterator<Key>(this->root), true};
  }
  set_node<Key> *parent = this->root;
  set_node<Key> *node =
      new set_node<Key>{nullptr, nullptr, nullptr, true, value};
  for (;;) {
    if (parent->key < value) {
      if (parent->rhs) {
        parent = parent->rhs;
        continue;
      } else {
        parent->rhs = node;
        node->parent = parent;
        break;
      }
    } else if (parent->key > value) {
      if (parent->lhs) {
        parent = parent->lhs;
        continue;
      } else {
        parent->lhs = node;
        node->parent = parent;
        break;
      }
    } else /* (parent->key == value) */ {
      return {set_iterator<Key>(parent), false};
    }
  }
  this->_size++;
  for (;;) {
    if (!parent->is_red) {
      // Case 1. Parent is black.
      break;
    }
    // Grandparent is always black, if exists.
    set_node<Key> *grandparent = parent->parent;
    if (!grandparent) {
      // Case 4. Parent is red and parent is the root node.
      parent->is_red = false;
      break;
    }
    set_node<Key> *uncle =
        (grandparent->lhs == parent) ? grandparent->rhs : grandparent->lhs;
    if (!uncle || !uncle->is_red) {
      if (grandparent->lhs == parent) {
        if (node == parent->rhs) {
          // Case 5a. Parent is red, sibling of parent (uncle) is black or does
          // not exist, parent->key < node->key < grandparent->key.
          // [([b?], p=R, (n)), g=B, [u?]]
          // Rotate left
          // [(([b?], p=R, .), n=R, .), g=B, [u?]]
          rotate_left(parent, &(grandparent->lhs));
          node = parent;
          parent = grandparent->lhs;
          // [(([b?], n=R, .), p=R, .), g=B, [u?]]
        }
        // Case 6a. Parent is red, sibling of parent (uncle) is black or does
        // not exist, node->key < parent->key < grandparent->key.
        // [((n), p=R, [b?]), g=B, [u?]]
        // Rotate right
        // ((n), p=R, [[b?], g=B, [u?]])
        // Recolour
        set_node<Key> *ggp = grandparent->parent;
        rotate_right(
            grandparent,
            (ggp) ? ((ggp->lhs == grandparent) ? &(ggp->lhs) : &(ggp->rhs))
                  : &(this->root));
        parent->is_red = false;
        grandparent->is_red = true;
        // [(n), p=B, ([b?], g=R, [u?])]
      } else { /* if (grandparent->rhs == parent) */
        if (node == parent->lhs) {
          // Case 5b. Parent is red, sibling of parent (uncle) is black or does
          // not exist, grandparent->key < node->key < parent->key.
          rotate_right(parent, &(grandparent->rhs));
          node = parent;
          parent = grandparent->rhs;
        }
        // Case 6b. Parent is red, sibling of parent (uncle) is black or does
        // not exist, grandparent->key < parent->key < node->key.
        set_node<Key> *ggp = grandparent->parent;
        rotate_left(grandparent, (ggp)
                                     ? ((ggp->lhs == grandparent) ? &(ggp->lhs)
                                                                  : &(ggp->rhs))
                                     : &(this->root));
        parent->is_red = false;
        grandparent->is_red = true;
      }
      break;
    }
    // Case 2. Parent is red, uncle is red.
    parent->is_red = false;
    uncle->is_red = false;
    grandparent->is_red = true;
    node = grandparent;
    if (!(parent = node->parent)) {
      // Case 3. Grandparent of the last iteration (now node) is the root node.
      break;
    }
  }
  return {this->find(value), true};
}

template <class Key>
typename set<Key>::iterator
set<Key>::insert(set<Key>::iterator pos, const set<Key>::value_type &value) {
  (void)pos;
  return this->insert(value).first;
}

template <class Key>
template <class InputIt>
void set<Key>::insert(InputIt first, InputIt last) {
  while (first != last) {
    this->insert(*first);
    first++;
  }
}

template <class Key>
typename set<Key>::iterator set<Key>::find(const Key &key) {
  set_iterator<Key> result;
  result.node = this->root;
  if (!this->root) {
    return result;
  }
  for (;;) {
    if (result.node->key < key) {
      if (result.node->rhs) {
        result.node = result.node->rhs;
        continue;
      } else {
        return set_iterator<Key>();
      }
    } else if (result.node->key > key) {
      if (result.node->lhs) {
        result.node = result.node->lhs;
        continue;
      } else {
        return set_iterator<Key>();
      }
    } else {
      return result;
    }
  }
}

template <class Key>
typename set<Key>::iterator set<Key>::erase(set<Key>::iterator pos) {
  set<Key>::iterator next = pos;
  next++;
  set_node<Key> *parent = pos.node->parent;
  set_node<Key> *lhs = pos.node->lhs;
  set_node<Key> *rhs = pos.node->rhs;
  pos.node->lhs = nullptr;
  pos.node->rhs = nullptr;
  if (lhs && rhs) {
    // swap
    Key t = pos.node->key;
    pos.node->key = next.node->key;
    next.node->key = t;
    pos.node->lhs = lhs;
    pos.node->rhs = rhs;
    return this->erase(next);
  } else if (lhs && !rhs) {
    if (!parent) {
      this->root = lhs;
    } else if (pos.node == parent->lhs) {
      parent->lhs = lhs;
    } else {
      parent->rhs = lhs;
    }
    lhs->parent = parent;
    lhs->is_red = false;
    this->_size--;
    delete pos.node;
  } else if (!lhs && rhs) {
    if (!parent) {
      this->root = rhs;
    } else if (pos.node == parent->lhs) {
      parent->lhs = rhs;
    } else {
      parent->rhs = rhs;
    }
    rhs->parent = parent;
    rhs->is_red = false;
    this->_size--;
    delete pos.node;
  } else if (!parent) {
    this->root = nullptr;
    this->_size--;
    delete pos.node;
  } else if (pos.node->is_red) {
    if (pos.node == parent->lhs) {
      parent->lhs = nullptr;
    } else {
      parent->rhs = nullptr;
    }
    this->_size--;
    delete pos.node;
  } else {
    // rebalance
    bool dir_lhs = pos.node == parent->lhs;
    if (dir_lhs) {
      parent->lhs = nullptr;
    } else {
      parent->rhs = nullptr;
    }
    this->_size--;
    delete pos.node;
    int c = 1;
    set_node<Key> *sibling;
    set_node<Key> *close_nephew;
    set_node<Key> *distant_nephew;
    for (;;) {
      if (dir_lhs) {
        sibling = parent->rhs;
        close_nephew = sibling->lhs;
        distant_nephew = sibling->rhs;
      } else {
        sibling = parent->lhs;
        close_nephew = sibling->rhs;
        distant_nephew = sibling->lhs;
      }
      if (sibling->is_red) {
        // Case 3.
        set_node<Key> *gp = parent->parent;
        set_node<Key> **p_ref =
            (gp) ? ((gp->lhs == parent) ? &(gp->lhs) : &(gp->rhs))
                 : &(this->root);
        if (dir_lhs) {
          rotate_left(parent, p_ref);
        } else {
          rotate_right(parent, p_ref);
        }
        parent->is_red = true;
        sibling->is_red = false;
        sibling = close_nephew;
        distant_nephew = (dir_lhs) ? sibling->rhs : sibling->lhs;
        close_nephew = (dir_lhs) ? sibling->lhs : sibling->rhs;
        if (distant_nephew && distant_nephew->is_red) {
          // Case 6.
          c = 6;
          break;
        } else if (close_nephew && close_nephew->is_red) {
          // Case 5.
          c = 5;
          break;
        } else {
          // Case 4.
          c = 4;
          break;
        }
      }
      if (distant_nephew && distant_nephew->is_red) {
        // Case 6.
        c = 6;
        break;
      } else if (close_nephew && close_nephew->is_red) {
        // Case 5.
        c = 5;
        break;
      }
      if (parent->is_red) {
        // Case 4.
        c = 4;
        break;
      }
      // Case 2.
      sibling->is_red = true;
      if (!parent->parent) {
        break;
      } else {
        dir_lhs = parent == parent->parent->lhs;
        parent = parent->parent;
      }
    }
    switch (c) {
    case 1:
      // pass
      break;
    case 4:
      sibling->is_red = true;
      parent->is_red = false;
      break;
    case 5:
      if (dir_lhs) {
        rotate_right(sibling, &parent->rhs);
      } else {
        rotate_left(sibling, &parent->lhs);
      }
      sibling->is_red = true;
      close_nephew->is_red = false;
      distant_nephew = sibling;
      sibling = close_nephew;
      break;
    }
    if (c == 5 || c == 6) {
      set_node<Key> *gp = parent->parent;
      set_node<Key> **p_ref =
          (gp) ? ((gp->lhs == parent) ? &(gp->lhs) : &(gp->rhs))
               : &(this->root);
      if (dir_lhs) {
        rotate_left(parent, p_ref);
      } else {
        rotate_right(parent, p_ref);
      }
      sibling->is_red = parent->is_red;
      parent->is_red = false;
      distant_nephew->is_red = false;
    }
  }
  return next;
}

template <class Key> int set<Key>::_verify_inner(const set_node<Key> *node) {
  int depth_lhs = 1;
  int depth_rhs = 1;
  if (node->lhs) {
    if (node->is_red && node->lhs->is_red) {
      return -1;
    }
    depth_lhs = _verify_inner(node->lhs);
  }
  if (node->rhs) {
    if (node->is_red && node->rhs->is_red) {
      return -1;
    }
    depth_rhs = _verify_inner(node->rhs);
  }
  if (depth_lhs != depth_rhs || depth_lhs == -1) {
    return -1;
  }
  return depth_lhs + ((node->is_red) ? 0 : 1);
}

template <class Key> bool set<Key>::_verify() const {
  // Every node is either red or black.
  // All null nodes are considered black.
  // A red node does not have a red child.
  // Every path from a given node to any of its leaf nodes (that is, to any
  // descendant null node) goes through the same number of black nodes.
  return set<Key>::_verify_inner(this->root) != -1;
}

#ifdef PRINT
#include "debug.h"

template <class Key> struct _print<set_node<Key>> {
  void operator()(const set_node<Key> &x, int margin = 0) {
    char *m = new char[margin + 1];
    for (int i = 0; i < margin; i++) {
      m[i] = ' ';
    }
    m[margin] = '\0';
    printf("{\n");
    printf("%s  .lhs = ", m);
    _print<set_node<Key> *>{}(x.lhs, margin + 2);
    printf(",\n");
    printf("%s  .is_red = %s", m, (x.is_red) ? "true" : "false");
    printf(",\n");
    printf("%s  .key = ", m);
    _print<Key>{}(x.key, margin + 2);
    printf(",\n");
    printf("%s  .rhs = ", m);
    _print<set_node<Key> *>{}(x.rhs, margin + 2);
    putc('\n', stdout);
    printf("%s}", m);
    delete[] m;
  }
};

template <class Key> struct _print<set<Key>> {
  void operator()(const set<Key> &x, int margin = 0) {
    char *m = new char[margin + 1];
    for (int i = 0; i < margin; i++) {
      m[i] = ' ';
    }
    m[margin] = '\0';
    printf("{\n");
    printf("%s  .root = ", m);
    _print<set_node<Key> *>{}(x.root, margin + 2);
    printf(",\n");
    printf("%s  ._size = %zu", m, x._size);
    putc('\n', stdout);
    printf("%s}", m);
    delete[] m;
  }
};

#endif

#endif
