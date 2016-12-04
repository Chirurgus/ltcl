#ifndef GUARD_LTCL_ITERATORS_H
#define GUARD_LTCL_ITERATORS_H
/*
 * Bidirectional Iterator assumes: op++, op--, ==
 */

namespace ltc {

template<class BiIter>
class Bi_reverse_iterator {
public:
	Bi_reverse_iterator() = default;
	Bi_reverse_iterator(BiIter bi) : _iter{bi} {}	
	
	bool operator==(const Bi_reverse_iterator<BiIter>& it)
		{ return _iter == it._iter; }
	bool operator!=(const Bi_reverse_iterator<BiIter>& it)
		{ return *this != it; }
	
	Bi_reverse_iterator<BiIter> operator++()
		{ --_iter; return *this; }
	Bi_reverse_iterator<BiIter> operator++(int)
		{ Bi_reverse_iterator<BiIter> ret{*this}; ++*this; return ret; }
	Bi_reverse_iterator<BiIter> operator--()
		{ ++_iter; return *this; }
	Bi_reverse_iterator<BiIter> operator--(int)
		{ Bi_reverse_iterator<BiIter> ret{*this}; --*this; return ret; }
	
	typename BiIter::value_type& operator*()
		{ return *_iter; }
	const typename BiIter::value_type& operator*() const
		{ return *_iter; }
private:
	BiIter _iter;
};// class Reverse_iterator

}// namespace ltc

#endif
