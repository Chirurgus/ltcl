#ifndef GURAD_LTCL_DETAIL_QUICKSORT_H
#define GURAD_LTCL_DETAIL_QUICKSORT_H


#include "../vector.h"
#include <iterator>
#include <random>//TODO: try to implement this, not to rely on anything non-ltcl

#include "insertsort.h"

namespace ltc {

namespace detail {

template<class I>
void partition(I begin, I end, I pivot)
{
	typedef typename std::I::value_type value_type;

	value_type tmp = *(end - 1);
	*(end - 1) = *pivot;
	*pivot = tmp;
	pivot = end - 1;

	I i = begin;
	for (I j = i; j != pivot; ++j) {
		if (*j < *pivot) {
			tmp = *j;
			*j = *i;
			*i = tmp;
			++i;
		}
	}

	tmp = *i;
	*i = *pivot;
	*pivot = tmp;
	pivot = i;
}

template<class I>
void quicksort_recurrsive(I begin, I end)
{
	for (;;) {
		if ((end - begin) <= 60) {//opimal on my machine
			insertsort(begin, end);
			return;
		}

		I pivot = begin + (rand() % (end - begin));

		partition(begin, end, pivot);
		
		quicksort(begin, pivot);
		begin = pivot;
		end = end;
	}
}

//for whatever reason the recurrsive version is faster
//than the stack version.
template<class I>
void quicksort(I begin, I end)
{
	struct vec_s {
		I begin, end;
	};
	Stack<vec_s> stack;

	stack.push({begin, end});

	while (!stack.isEmpty()) {
		vec_s cur = stack.pop();
		if ((cur.end - cur.begin) <= 100) {
			insertSort(cur.begin, cur.end);
			continue;
		}
		I pivot = cur.begin + (rand() % (cur.end - cur.begin));

		partition(cur.begin, cur.end, pivot);

		stack.push({cur.begin, pivot});
		stack.push({pivot, cur.end});
	}
}

}//namespace detail

}//namespace ltc

#endif
