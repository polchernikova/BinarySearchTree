#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_

#include <iostream>
#include <functional>
#include <iterator>

template<typename Iterator>
bool IsSorted(Iterator begin, Iterator end) {
    Iterator next = begin;
    if (begin == end) {
        return true;
    }
    while (++next != end) {
        if (*next < *begin) {
            return false;
        }
        ++begin;
    }
    return true;
}

template<typename Iterator>
Iterator MaxElement(Iterator begin, Iterator end) {
    if (begin == end) {
        return end;
    }
    Iterator max = begin;
    begin++;
    while (begin != end) {
        if (*max < *begin) {
            max = begin;
        }
        begin++;
    }
    return max;
}

template<typename Iterator>
typename std::iterator_traits<Iterator>::value_type
Accumulate(Iterator begin, Iterator end) {
    typename std::iterator_traits<Iterator>::value_type result =
            typename std::iterator_traits<Iterator>::value_type();
    while (begin != end) {
        result += *begin;
        begin++;
    }
    return result;
}


template<typename Iterator, typename Functor>
int CountIf(Iterator begin, Iterator end, Functor pred) {
    int result{0};
    while (begin != end) {
        if (pred(*begin)) {
            result++;
        }
        begin++;
    }
    return result;
}

template<typename Iterator,
        typename T = typename std::iterator_traits<Iterator>::value_type>
int CountIfNot(Iterator begin, Iterator end,
               bool(*pred)(const T &)) {
    int result{0};
    while (begin != end) {
        if (!pred(*begin)) {
            result++;
        }
        begin++;
    }
    return result;
}

template<typename Iterator,
        typename T = typename std::iterator_traits<Iterator>::value_type>
Iterator
FindIf(Iterator begin, Iterator end, std::function<bool(const T &)> pred) {
    while (begin != end) {
        if (pred(*begin)) {
            return begin;
        }
        begin++;
    }
    return end;
}

#endif  // ALGORITHMS_H_
