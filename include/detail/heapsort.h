#ifndef GUARD_LTC_DETAIL_HEAPSORT_H
#define GUARD_LTC_DETAIL_HEAPSORT_H

#include "../Vector.h"

namespace ltc {

namespace detail {

inline size_t maxheap_parent(const size_t & pos) {
	return (pos - 1)/2;
}

inline size_t maxheap_right(const unsigned long long & pos) {
	return pos * 2 + 2;
}

inline size_t maxheap_left(const size_t & pos) {
	return pos * 2 + 1;
}

template<class T>
void maxheapify(Vector<T>& a, typename Vector<T>::size_type i)
{
	for (;;) {
		Vector<T>::size_type  max_child =
			(a[maxheap_left(i)] > a[maxheap_right(i)] ?
				maxheap_left(i)
			      : maxheap_right(i));

		if (a[max_child] > a[i]) {
			T tmp = a[i];
			a[i] = a[max_child];
			a[max_child] = tmp;

			i = max_child;
		}
		else
			break;
	}
}

template<class T>
void heapsort(Vector<T>& a)
{
	for (Vector<T>::size_type i = a.size()/2; i-- > 0;) {
		maxheapify(a, i);
	}

	for (Vector<T>::size_type i = 0; i < a.size(); ++i) {
		T ret_int = a[0];
		a[0] = a[a.size() - i];
		a[a.size() - i] = ret_int;

		maxheapify(a, 0);
	}
}

}//namespace detail

}//namespace ltc

#endif
