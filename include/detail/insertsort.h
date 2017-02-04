#ifndef GUARD_LTCL_DETAIL_INSERTSORT_H
#define GUARD_LTCL_DETAIL_INSERTSORT_H

namespace ltc {

namespace detail {


}//namespace ltc

template<class I>
void insertsort(I begin, I end);

}//namespace detail

template<class I>
void ltc::detail::insertsort(I begin, I end) {
	typedef typename std::I::value_type value_type;

	for (I i = begin; i != end; ++i) {
		I min = i;
		for (I j = i + 1; j != end; ++j) {
			if (*min > *j) {
				min = j;
			}
		}
		value_type tmp = *min;
		*min = *i;
		*i = tmp;
	}
}

#endif
