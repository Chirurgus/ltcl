#ifndef GUARD_VECTOR_H
#define GUARD_VECTOR_H

#include <memory>

namespace ltcl {

template<class T, class Alloc = std::allocator<T>>
class vector {
	using size_type = unsigned long long;//might waht to change this later
	using value_type = T;
	
	static constexpr size_type default_size = 8;
	static constexpr size_type growth_coef = 2;
	
	vector();
	vector(const vector<T>& v);
	vector(vector<T>&& v);

	vector<T>& operator=(const vector<T>& v);
	vector<T>& operator=(vector<T>&& v);

	~vector();
		
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
	T* _elem;
	T* _size;
	T* _capa;
	Alloc<T> alloc;
};//class vector

}// namespace ltcl

ltcl::vector()
	:_elem{alloc.allcoate(default_size)},//MIGHT THROW
	:_size{_elem};
	:_capa{_size + default_size} {}

ltcl::vector(const vector<T>& v)
	:_elem{alloc.allocate(v.size())},//MIGHT THROW
	:_size{_elem + v.size()},
	:_capa{_size} {
	for (value_type *v_it {v._elem}, *it {_elem}; it != v._size;) {
		alloc.construct(it++, *v_it++)
	}
}

ltcl::vector(vector<T>&& v)
	:_elem{v._elem},
	:_size{v._size},
	:_capa{v._capa} {
	v._elem = v._size= v._capa = nullptr;
}

ltcl::vector<T>& ltcl::vector::operator=(const ltcl::vector<T>& v) {
	alloc.destroy(_elem, capacity());
	if (size() < v.size()) {
		alloc.deallocate(_elem, capacity());
		alloc.allocate(_elem, size());
	}
	for (value_type *v_it {v._elem}, *it {_elem}; it != v._size;) {
		alloc.construct(it++, *v_it++)
	}
}

ltcl::vector<T>& ltcl::vector::operator=(vector<T>&& v) {
	_elem = v._elem;
	_size = v._size;
	_capa = v._capa;
	v._elem = v._size = v._capa = nullptr;	
}

ltcl::~vector() {
	alloc.destroy(_elem, size());
	alloc.deallocate(_elem, capacity());
}

#endif
