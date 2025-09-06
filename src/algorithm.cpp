#include "algorithm.h"

template <class OutputIt, class Size, class T>
OutputIt fill_n(OutputIt first, Size count, const T &value) {
    for (Size i = 0; i < count; i++) {
        *first++ = value;
    }
    return first;
}

template int* fill_n<int*, int, int>(int*, int, int const&);
template insert_iterator<vector<int> > fill_n<insert_iterator<vector<int> >, int, int>(insert_iterator<vector<int> >, int, int const&);
