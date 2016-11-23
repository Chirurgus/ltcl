#ifndef GUARD_LTCL_ITERATORS_H
#define GUARD_LTCL_ITERATORS_H

/*
 * Handling operator== is too hard, making this hierarchy a more cumbersome
 * solution than just using templates, and having iterators provide needed
 * functions.
 */

namespace ltc {

template<class T>
class Iterator {
public:
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = unsigned long long;
	
	virtual reference operator*() = 0;
	virtual const_reference operator*() const = 0;
	
	virtual Iterator<T>& operator++() = 0;
	virtual Iterator<T>& operator++(int) = 0;
	
	bool operator==(const Iterator<T>& it) const { return equal(it); }
	bool operator!=(const Iterator<T>& it) const { return !equal(it); }
	
	virtual ~Iterator() = 0;
protected:
	virtual bool equal(const Iterator<T>&) const = 0;
};// class Iterator

template<class T>
class Bidirectional_iterator : public Iterator<T> {
	using typename Iterator<T>::value_type;
	using typename Iterator<T>::pointer;
	using typename Iterator<T>::const_pointer;
	using typename Iterator<T>::reference;
	using typename Iterator<T>::const_reference;
	using typename Iterator<T>::size_type;

	virtual Bidirectional_iterator<T>& operator--() = 0; 
	virtual Bidirectional_iterator<T>& operator--(int) = 0;
};// class Bidirectional_iterator


template<class T>
class Reverse_iterator : public Iterator<T> {
public:
	using typename Iterator<T>::value_type;
	using typename Iterator<T>::pointer;
	using typename Iterator<T>::const_pointer;
	using typename Iterator<T>::reference;
	using typename Iterator<T>::const_reference;
	using typename Iterator<T>::size_type;

	Reverse_iterator<T>& operator++() override = 0; 
	Reverse_iterator<T>& operator++(int) override = 0;
};// class Reverse_iterator

}// namespace ltc

#endif
