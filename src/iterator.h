#ifndef __SRC_ITERATOR_H
#define __SRC_ITERATOR_H

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class Iter> struct iterator_traits {
  typedef typename Iter::value_type value_type;
  typedef typename Iter::pointer pointer;
  typedef typename Iter::reference reference;
  typedef typename Iter::iterator_category iterator_category;
};

template <class T> struct iterator_traits<T *> {
  typedef T value_type;
  typedef T *pointer;
  typedef T &reference;
  typedef random_access_iterator_tag iterator_category;
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

template <class InputIt, class Distance>
void __iter_advance(InputIt &it, Distance n, random_access_iterator_tag) {
  it += n;
}

template <class InputIt, class Distance>
void __iter_advance(InputIt &it, Distance n, bidirectional_iterator_tag) {
  while (n > 0) {
    ++it;
    --n;
  }
  while (n < 0) {
    --it;
    ++n;
  }
}

template <class InputIt, class Distance> void advance(InputIt &it, Distance n) {
  __iter_advance(it, n, typename iterator_traits<InputIt>::iterator_category{});
}

template <class InputIt>
void __iter_prev(InputIt &it, bidirectional_iterator_tag) {
  --it;
}

template <class InputIt> void __iter_prev(InputIt &, forward_iterator_tag) {
  // pass
}

template <class InputIt> InputIt next(InputIt it) { ++it; }
template <class InputIt> InputIt prev(InputIt it) {
  __iter_next(it, typename iterator_traits<InputIt>::iterator_category{});
}

#include "vector.h"

extern template insert_iterator<vector<int>>
inserter<vector<int>>(vector<int> &, vector<int>::iterator);
extern template class insert_iterator<vector<int>>;

#endif
