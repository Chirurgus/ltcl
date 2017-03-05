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
void countingsort(RandIt begin, RandIt end, Fnctr crit, Int k) {
	ltc::Vector<ltc::iterator_traits<RandIt>::size_type> c(k + 1, Int{});
	for (RandIt i = begin; i != end; ++i) {
		++c[cirt(*i)];
	}
	for (auto i = ltc::next(c.begin()); i != c.end(); ++i) {
		*i += *ltc::prev(i);
	}

/*
 * 	Allocate an array, and make sure it allocates enough memeory,
 * 	then use that memory to store stuff, without initializing it
 * 	the memory is accessed via a pointer
 */
	ltc::Vector<ltc::iterator_traits<RandIt>::value_type> ret();
	ret.reserve(ltc::distance(begin,end));
	ltc::Vector<ltc::iterator_traits<RandIt>::value_type>::iterator 
		tmp {ret.begin()};
	for (ltc::Bi_reverse_iterator<RandIt> i = end;
		 i != begin;
		 ++i) {
		tmp[--c[crit(*i)]] = ltc::move(*i);
	}
	ltc::Vector<ltc::iterator_traits<RandIt>::value_type>::size_type pos{0};
	for (RandIt i = begin; i != end; ++i) {
		*i = ltc::move(tmp[pos++]);
	}
}

}// namespace detail

}// namespace ltc

#endif// GUARD_LTCL_DETAIL_COUNTINGSORT_H
