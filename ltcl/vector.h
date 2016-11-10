#ifndef GUARD_LTCL_VECTOR_H
#define GUARD_LTCL_VECTOR_H

#include <memory>

#include "exceptions.h"

namespace ltc {

template<class T, class Alloc = std::allocator<T>>
class Vector {
public:
	using size_type = unsigned long long;//might waht to change this later
	using value_type = T;

	using reference = value_type&;
	using const_reference = const value_type&;

	using pointer = value_type*;
	using const_pointer = const value_type*;

	using iterator = pointer;
	using const_iterator = const_pointer;

	static constexpr size_type default_size {8};
	static constexpr size_type growth_coef {2};
	
	Vector();
	Vector(const Vector<value_type,Alloc>& v);
	Vector(Vector<value_type,Alloc>&& v);
	
	//need a way to check if T is default initializable
	explicit 
	Vector(const size_type sz, const value_type& init = value_type{});

	Vector<value_type,Alloc>& operator=(const Vector<value_type,Alloc>& v);
	Vector<value_type,Alloc>& operator=(Vector<value_type,Alloc>&& v);

	~Vector();
	
	void clear();
	void swap(Vector<value_type, Alloc>& v);
	void resize(const size_type i, const value_type& v = value_type{});	
	void reserve(const size_type i);
	void shrink_to_fit();
	
	size_type size() const;
	size_type capacity() const;
	bool empty() const;
	
	void push_back(const value_type& v);	
	//void push_back(value_type&& v);//no clue how to implement this
	
	const_reference at(const size_type i) const;
	reference at(const size_type i);

	const_reference front() const;
	reference front();

	const_reference back() const;
	reference back();

	const_reference operator[](const size_type i) const;
	reference operator[](const size_type i);
	
	iterator begin();
	const_iterator cbegin() const;
	
	iterator end();
	const_iterator cend() const;
	
private:
	void grow(const size_type sz);

	pointer _first {nullptr};
	pointer _last {nullptr};
	pointer _end {nullptr};
	Alloc alloc;
};//class Vector

}// namespace ltc

template<class T, class A>
ltc::Vector<T, A>::Vector()
	:_first {alloc.allocate(default_size)} {//MIGHT THROW
	_last = _first;
	_end = _last + default_size;
}

template<class T, class A>
ltc::Vector<T, A>::Vector(const Vector<T, A>& v)
	:_first {alloc.allocate(v.size())} {//MIGHT THROW
	_last = _first + v.size();
	_end = _last;
	for (pointer v_it {v._first}, it {_first}; it != v._last;) {
		alloc.construct(it++, *v_it++);
	}
}

template<class T, class A>
ltc::Vector<T,A>::Vector(Vector<T,A>&& v)
	:_first{v._first},
	_last{v._last},
	_end{v._end} {
	v._first = v._last= v._end = nullptr;
}

template<class T, class A>
ltc::Vector<T,A>::Vector(const size_type sz, const value_type& init)
	:_first {alloc.allocate(sz)} {
	_last = _first + sz;
	_end = _last;
	for (pointer it {_first}; it != _last; ++it) {
		alloc.construct(it, init);
	}
}

template<class T, class A>
ltc::Vector<T,A>& ltc::Vector<T,A>::operator=(const Vector<T,A>& v) {
	alloc.destroy(_first, capacity());
	if (size() < v.size()) {
		alloc.deallocate(_first, capacity());
		alloc.allocate(_first, size());
	}
	for (pointer v_it {v._first}, it {_first}; it != v._last;) {
		alloc.construct(it++, *v_it++);
	}
}

template<class T, class A>
ltc::Vector<T,A>& ltc::Vector<T,A>::operator=(Vector<T,A>&& v) {
	_first = v._first;
	_last = v._last;
	_end = v._end;
	v._first = v._last = v._end = nullptr;	
}

template<class T, class A>
ltc::Vector<T,A>::~Vector() {
	for (pointer it {_first}; it != _last; ++it) {
		alloc.destroy(it);
	}
	alloc.deallocate(_first, capacity());
}
	
template<class T, class A>
void ltc::Vector<T,A>::clear() {
	for (; _last != _first; --_last) {
		alloc.destroy(_last - 1);
	}
}

template<class T, class A>
void ltc::Vector<T,A>::swap(Vector<T, A>& v) {
	pointer tmp {_first};
	_first = v._first;
	v._first = tmp;

	tmp = _last;
	_last = v._last;
	v._last = tmp;

	tmp = _end;
	_end = v._end;
	v._end = tmp;
}

template<class T, class A>
void ltc::Vector<T,A>::resize(const size_type i,
				 const value_type& v) {
	if (i <= capacity()) {
		if (i <= size()) {
			for (iterator it{_first + i}; it != _last; ++it) {
				alloc.destroy(it);
			}
			_last = _first + i;
			return;
		}
		else {
			for (;_last != _first + i; ++_last) {
				alloc.construct(_last, v);
			}
			return;
		}
	}
	grow(i);
	for (; _last != _end; ++_last) {
		alloc.construct(_last, v);
	}
}

template<class T, class A>
inline void ltc::Vector<T,A>::reserve(const size_type i) {
	grow(i);
}

template<class T, class A>
void ltc::Vector<T,A>::shrink_to_fit() {
	if (size() == capacity()) return;
	const size_type sz {size()};
	pointer tmp {alloc.allocate(sz)};
	for (pointer t_it {tmp}, it {_first}; it != _last; ++it, ++t_it ) {
		alloc.construct(t_it, *it);
	}
	for (pointer it {_first}; it != _last; ++it) {
		alloc.destroy(it);
	}
	alloc.deallocate(_first, capacity());
	_first = tmp;
	_last = _end = _first + sz;
}

template<class T, class A>
inline typename ltc::Vector<T,A>::size_type ltc::Vector<T,A>::size() const {
	return _last - _first;
}

template<class T, class A>
inline typename ltc::Vector<T,A>::size_type ltc::Vector<T,A>::capacity() const {
	return _end - _first;
}

template<class T, class A>
bool ltc::Vector<T,A>::empty() const {
	return _first == _last;
}

template<class T, class A>
void ltc::Vector<T,A>::push_back(const value_type& v) {
	if (capacity() == size()) {
		grow(capacity() * growth_coef);
	}
	alloc.construct(_last++, v);	
}

template<class T, class A>
void ltc::Vector<T,A>::grow(const size_type sz) {
	if (sz <= size()) return;
	const size_type old_sz {size()};
	pointer tmp {alloc.allocate(sz)};
	for (pointer it {_first}, t_it {tmp};
		it != _last;
		++it, ++t_it) {
		alloc.construct(t_it, *it);
	}
	for (pointer it {_first}; it != _last; ++it) {
		alloc.destroy(it);
	}
	alloc.deallocate(_first, capacity());
	_first = tmp;
	_last = _first + old_sz;
	_end = _first + sz;
}
template<class T, class A>
typename ltc::Vector<T,A>::const_reference
	ltc::Vector<T,A>::at(const size_type i) const {
	if (i >= size()) {
		throw out_of_range {"ltc::Vector::at out of range."};
	}
	return _first[i];
}

template<class T, class A>
typename ltc::Vector<T,A>::reference
	ltc::Vector<T,A>::at(const size_type i) {
	if (i >= size()) {
		throw out_of_range {"ltc::Vector::at out of range."};
	}
	return _first[i];
}

template<class T, class A>
typename ltc::Vector<T,A>::const_reference
	ltc::Vector<T,A>::front() const {
	if (empty()) throw out_of_range {"ltc::Vector::front out of range."};
	return *begin();
}

template<class T, class A>
typename ltc::Vector<T,A>::reference
	ltc::Vector<T,A>::front() {
	if (empty()) throw out_of_range {"ltc::Vector::front out of range."};
	return *begin();
}

template<class T, class A>
typename ltc::Vector<T,A>::const_reference
	ltc::Vector<T,A>::back() const {
	if (empty()) throw out_of_range {"ltc::Vector::back out of range."};
	return *(end() - 1);
}

template<class T, class A>
typename ltc::Vector<T,A>::reference
	ltc::Vector<T,A>::back() {
	if (empty()) throw out_of_range {"ltc::Vector::back out of range."};
	return *(end() - 1);
}


template<class T, class A>
typename ltc::Vector<T,A>::const_reference 
	ltc::Vector<T,A>::operator[](const size_type i) const {
	if (i >= size()) {
		throw out_of_range 
			{"ltc::Vector subscript operator out of range"};
	}
	return _first[i];
}

template<class T, class A>		
typename ltc::Vector<T,A>::reference 
	ltc::Vector<T,A>::operator[](const size_type i) {
	if (i >= size()) {
		throw out_of_range 
			{"ltc::Vector subscript operator out of range"};
	}
	return _first[i];
}

template<class T, class A>		
typename ltc::Vector<T,A>::iterator  ltc::Vector<T,A>::begin() {
	return _first;
}

template<class T, class A>		
typename ltc::Vector<T,A>::const_iterator  ltc::Vector<T,A>::cbegin() const {
	return _first;
}
	
template<class T, class A>		
typename ltc::Vector<T,A>::iterator  ltc::Vector<T,A>::end() {
	return _last;
}

template<class T, class A>		
typename ltc::Vector<T,A>::const_iterator  ltc::Vector<T,A>::cend() const {
	return _last;
}
	
#endif
