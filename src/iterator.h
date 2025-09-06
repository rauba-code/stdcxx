#ifndef __SRC_ITERATOR_H
#define __SRC_ITERATOR_H

template <class Iter> struct iterator_traits {
  typedef typename Iter::value_type value_type;
  typedef typename Iter::pointer pointer;
  typedef typename Iter::reference reference;
};

template <class T> struct iterator_traits<T *> {
  typedef T value_type;
  typedef T *pointer;
  typedef T &reference;
};

template <class Iter> class reverse_iterator {
public:
  typedef Iter iterator_type;
  typedef typename iterator_traits<Iter>::pointer pointer;
  typedef typename iterator_traits<Iter>::reference reference;

  // reverse_iterator();
  explicit reverse_iterator(iterator_type x);
  // too complicated
  // template <class U> reverse_iterator(const reverse_iterator<U> &other);

  iterator_type base() const;

  reference operator*() const;
  pointer operator->() const;

  reverse_iterator &operator++();
  reverse_iterator &operator--();

protected:
  Iter current;
};

template <class Container> class insert_iterator {
public:
  typedef void value_type;
  typedef void pointer;
  typedef void reference;

  insert_iterator(Container &, typename Container::iterator);
  insert_iterator<Container> &
  operator=(const typename Container::value_type &value);
  insert_iterator<Container> &operator*();
  insert_iterator<Container> &operator++();
  insert_iterator<Container> &operator++(int);

protected:
  Container *container;
  typename Container::iterator iter;
};

template <class Container>
insert_iterator<Container> inserter(Container &c,
                                    typename Container::iterator i);

#include "vector.h"

extern template insert_iterator<vector<int>>
inserter<vector<int>>(vector<int> &, vector<int>::iterator);
extern template class insert_iterator<vector<int>>;

#endif
