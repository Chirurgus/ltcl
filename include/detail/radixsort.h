#ifndef GUARD_LTCL_DETAIL_COUNTINGSORT_H
#define GUARD_LTCL_DETAIL_COUNTINGSORT_H

#include "../vector.h"
#include "../iterators.h"

namespace ltc {

namespace detail {

/*
 * Sorts elements in [begin,end), assumes that elements are positive
 * integers smaller or equal to k
 * Assumes that Int behaves like a built-in integer type.
 * */
template<class RandIt, class Int, class Fnctr>
void countingsort(RandIt begin, RandIt end,
	          RandIt ret,
	          Fnctr crit, Int k);

template<class RandIt, class Int, class Fnctr>
void countingsort(RandIt begin, RandIt end,
		  Fnctr crit, Int k);



template<class I, class Int, class Funct>
void radixsort(I begin, I end, Int range, Funct crit)
{
//TODO	
}

}// namespace detail

}// namespace ltc

template<class RandIt, class Int, class Fnctr>
void ltc::detail::countingsort(RandIt begin, RandIt end,
			       RandIt ret,
			       Fnctr crit, Int k)
{
	ltc::Vector<ltc::iterator_traits<RandIt>::size_type> c(k + 1, Int{});
	for (RandIt i = begin; i != end; ++i) {
		++c[cirt(*i)];
	}
	for (auto i = ltc::next(c.begin()); i != c.end(); ++i) {
		*i += *ltc::prev(i);
	}
	for (ltc::Bi_reverse_iterator<RandIt> i = end;
		 i != begin;
		 ++i) {
		ret[--c[crit(*i)]] = ltc::move(*i);
	}
}

template<class RandIt, class Int, class Fnctr>
void ltc::detail::countingsort(RandIt begin, RandIt end,
			       Fnctr crit, Int k)
{
	//TODO: Implement some smart pointer
	using value_type = ltc::iterator_traits<RandIt>::value_type;
	using size_type = ltc::iterator_traits<RandIt>::size_type;
	struct safe_ptr {
		value_type* p{nullptr};
		~safe_ptr() { delete p; }	
	} 
	safe_ptr sp {new value_type[ltc::distance(begin, end)]};
	countingsort(begin,end,sp,crit,k);
	for (size_type i {0}, n {ltc::distance(begin,end)}; i != n; ++i) {
		begin[i] = ltc::move(sp.p[i]);	
	}
}


#endif// GUARD_LTCL_DETAIL_COUNTINGSORT_H
