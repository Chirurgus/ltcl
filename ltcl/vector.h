#ifndef GUARD_VECTOR_H
#define GUARD_VECTOR_H

#include <memory>

namespace ltc {

template<class T, class Alloc = std::allocator<T>>
class Vector {
public:
	using size_type = unsigned long long;//might waht to change this later
	using value_type = T;
	
	static constexpr size_type default_size {8};
	static constexpr size_type growth_coef {2};
	
	Vector();
	Vector(const Vector<T,Alloc>& v);
	Vector(Vector<T,Alloc>&& v);
	
	//need a way to check if T is default initializable
	Vector(const size_type sz, const T& init = T{});

	Vector<T,Alloc>& operator=(const Vector<T,Alloc>& v);
	Vector<T,Alloc>& operator=(Vector<T,Alloc>&& v);

	~Vector();
		
	size_type size() const {
		return _size - _elem;
	}
	size_type capacity() const {
		return _capa - _elem;
	}
	
	const T& operator[](const size_type i) const {
		return _elem[i];
	}
	T& operator[](const size_type i) {
		return _elem[i];
	}
	
private:
	T* _elem {nullptr};
	T* _size {nullptr};
	T* _capa {nullptr};
	Alloc alloc;
};//class Vector

}// namespace ltc

template<class T, class A>
ltc::Vector<T, A>::Vector()
	:_elem {alloc.allocate(default_size)} {//MIGHT THROW
	_size = _elem;
	_capa = _size + default_size;
}

template<class T, class A>
ltc::Vector<T, A>::Vector(const Vector<T, A>& v)
	:_elem {alloc.allocate(v.size())} {//MIGHT THROW
	_size = _elem + v.size();
	_capa = _size;
	for (value_type *v_it {v._elem}, *it {_elem}; it != v._size;) {
		alloc.construct(it++, *v_it++);
	}
}

template<class T, class A>
ltc::Vector<T,A>::Vector(Vector<T,A>&& v)
	:_elem{v._elem},
	_size{v._size},
	_capa{v._capa} {
	v._elem = v._size= v._capa = nullptr;
}

template<class T, class A>
ltc::Vector<T,A>::Vector(const size_type sz, const T& init)
	:_elem {alloc.allocate(sz)} {
	_size = _elem + sz;
	_capa = _size;
	for (T* it {_size}; it != _size; ++it) {
		alloc.construct(it, init);
	}
}

template<class T, class A>
ltc::Vector<T,A>& ltc::Vector<T,A>::operator=(const Vector<T,A>& v) {
	alloc.destroy(_elem, capacity());
	if (size() < v.size()) {
		alloc.deallocate(_elem, capacity());
		alloc.allocate(_elem, size());
	}
	for (value_type *v_it {v._elem}, *it {_elem}; it != v._size;) {
		alloc.construct(it++, *v_it++);
	}
}

template<class T, class A>
ltc::Vector<T,A>& ltc::Vector<T,A>::operator=(Vector<T,A>&& v) {
	_elem = v._elem;
	_size = v._size;
	_capa = v._capa;
	v._elem = v._size = v._capa = nullptr;	
}

template<class T, class A>
ltc::Vector<T,A>::~Vector() {
	for (T* it {_elem}; it != _size; ++it) {
		alloc.destroy(it);
	}
	alloc.deallocate(_elem, capacity());
}

#endif
