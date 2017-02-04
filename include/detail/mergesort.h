#ifndef GUARD_MERGE_SORT_H
#define GUARD_MERGE_SORT_H

#include "../iterators.h"

namespace ltc {

namespace detail {

// w is the iterator pointing to the working space 
// the vector w has to be lage enough to hold both a1 and a2
template<class I>
void mergesort_aux(I a1, I a1_end, I a2,I a2_end, I w)  {
	while (a1 != a1_end && a2 != a2_end)
		*(w++) = (*a1 < *a2 ? *(a1++) : *(a2++));
	while (a1 != a1_end)
		*(w++) = *(a1++);
	while (a2 != a2_end)
		*(w++) = *(a2++);
}

template<class I>
void mergesort(I a, I a_end) {
	typedef typename I::value_type value_type;
	vector<value_type>::size_type size = a_end - a;
	//TODO: remove referece to Vector

	if (size < 2)
		return;
	
	mergesort(a, a + (size - size / 2));
	mergesort(a + (size - size / 2), a_end);

	vector<value_type> tmp(a, a_end);
	mergesort_aux(tmp.begin(),
		     tmp.begin() + (size - size / 2),
		     tmp.begin() + (size - size / 2),
		     tmp.end(), a);
}

}//namespace detail

}//namespace ltc

#endif// GUARD_LTCL_DETAIL_MERGESORT_H
