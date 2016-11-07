#ifndef GUARD_VECTOR_H
#define GUARD_VECTOR_H

#include <memory>

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

	static constexpr size_type default_last {8};
	static constexpr size_type growth_coef {2};
	
	Vector();
	Vector(const Vector<value_type,Alloc>& v);
	Vector(Vector<value_type,Alloc>&& v);
	
	//need a way to check if T is default initializable
	Vector(const size_type sz, const value_type& init = value_type{});

	Vector<value_type,Alloc>& operator=(const Vector<value_type,Alloc>& v);
	Vector<value_type,Alloc>& operator=(Vector<value_type,Alloc>&& v);

	~Vector();
		
	size_type size() const {
		return _last - _first;
	}
	size_type capacity() const {
		return _end - _first;
	}
	bool empty() const {
		return _first == _last;
	}
	
	void push_back(const value_type& v);	

	const_reference operator[](const size_type i) const {
		return _first[i];
	}
	reference operator[](const size_type i) {
		return _first[i];
	}
	
	iterator begin() {
		return _first;
	}
	const_iterator cbegin() const {
		return _first;
	}
	
	iterator end() {
		return _last;
	}
	const_iterator cend() const {
		return _last;
	}	
	
private:
	pointer _first {nullptr};
	pointer _last {nullptr};
	pointer _end {nullptr};
	Alloc alloc;
};//class Vector

}// namespace ltc

template<class T, class A>
ltc::Vector<T, A>::Vector()
	:_first {alloc.allocate(default_last)} {//MIGHT THROW
	_last = _first;
	_end = _last + default_last;
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
ltc::Vector<T,A>::Vector(const size_type sz, const T& init)
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
void ltc::Vector<T,A>::push_back(const value_type& v) {
	if (capacity() == size()) {//grow
		const size_type old_sz {size()};
		const size_type new_sz {old_sz * growth_coef};
		const pointer tmp {alloc.allocate(new_sz)};
		for (const_iterator it {cbegin()}, t_it {tmp};
			it != cend();
			++it, ++t_it) {
			alloc.construct(t_it, *it);
		}
		for (iterator it {begin()}; it != end(); ++it) {
			alloc.destroy(it);
		}
		alloc.deallocate(begin(), size());
		_first = tmp;
		_last = _first + old_sz;
		_end = _first + new_sz;
	}
	alloc.construct(_last++, v);	
}
		
	
#endif
