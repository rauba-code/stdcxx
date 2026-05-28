#include "algorithm.h"

// --- fill_n

template <class OutputIt, class Size, class T>
OutputIt fill_n(OutputIt first, Size count, const T &value) {
  for (Size i = 0; i < count; i++) {
    *first++ = value;
  }
  return first;
}

template int *fill_n<int *, int, int>(int *, int, int const &);
template insert_iterator<vector<int>>
fill_n<insert_iterator<vector<int>>, int, int>(insert_iterator<vector<int>>,
                                               int, int const &);

// --- remove

template <class ForwardIterator, class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last,
                       const T &value) {
  ForwardIterator i = first, j = first;
  for (;;) {
    if (*i == value) {
      i++;
      if (i == last) {
        return j;
      }
    }
    *j++ = *i++;
    if (i == last) {
      return j;
    }
  }
}

template int *remove<int *, int>(int *, int *, const int &);

// --- remove_if

template <class ForwardIterator, class Predicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
                          Predicate pred) {
  ForwardIterator i = first, j = first;
  for (;;) {
    if (pred(*i)) {
      i++;
      if (i == last) {
        return j;
      }
    }
    *j++ = *i++;
    if (i == last) {
      return j;
    }
  }
}

template int *remove_if<int *, bool (*)(const int &)>(int *, int *,
                                                      bool (*)(const int &));
