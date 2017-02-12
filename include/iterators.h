#ifndef GUARD_LTCL_ITERATORS_H
#define GUARD_LTCL_ITERATORS_H

namespace ltc {

template<class I>
struct Iterator_traits {
	using value_type = I::value_type;
	using pointer_type = I::pointer_type;
	using reference_type = I::reference_type;
	using size_type = I::size_type;
};//Iterator_traits

template<class T>
struct Iterator_traits<T*> {
	using value_type = T;
	using pointer_type = T*;
	using reference_type = T&;
	using size_type = unsinged long long;
};//Iterator_traits

/*
 * Bidirectional Iterator assumes that these operators are present:
 *					 		 op++, op--, op==
 */
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

//TODO: somehow determine Iter size_type (boy aren't iterator traits cool)
template<class Iter>
unsigned long long distance(const Iter& first, const Iter& last);

template<class Iter>
void advance(Iter& it, long long = 1);

template<class Iter>
Iter next(Iter it, long long n = 1);

template<class Iter>
Iter prev(Iter it, long long n = 1);

}// namespace ltc

template<class Iter>
unsigned long long ltc::distance(const Iter& first, const Iter& last)
{
	unsigned long long ret{0};	
	for (Iter it{first}; it != last; ++it) {
		++ret;
	}
	return ret;
}

template<class Iter>
void ltc::advance(Iter& it, long long n)
{
	if (n >= 0) {
		for (long long i{0}; i < n; ++i) {
			++it;
		}
	}
	else {
		for (long long i{0}; i < -n; ++i) {
			--it;
		}
	}
}

template<class Iter>
Iter ltc::next(Iter it, long long n)
{
	advance(it, n);
	return it;
}

template<class Iter>
Iter ltc::prev(Iter it, long long n)
{
	advance(it, -n);
	return it;
}

#endif
