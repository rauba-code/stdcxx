#ifndef __SRC_ALGORITHM_H
#define __SRC_ALGORITHM_H

template <class OutputIt, class Size, class T>
OutputIt fill_n(OutputIt first, Size count, const T &value);

template <class ForwardIterator, class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last,
                       const T &value);

template <class ForwardIterator, class Predicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
                          Predicate pred);

// template instantiations

#include "iterator.h"
#include "vector.h"

extern template int *fill_n<int *, int, int>(int *, int, int const &);
extern template insert_iterator<vector<int>>
fill_n<insert_iterator<vector<int>>, int, int>(insert_iterator<vector<int>>,
                                               int, int const &);

extern template int *remove<int *, int>(int *, int *, const int &);
extern template int *
remove_if<int *, bool (*)(const int &)>(int *, int *, bool (*)(const int &));

#endif
