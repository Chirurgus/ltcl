#ifndef GUARD_LTCL_ALLOCATORS_H
#define GUARD_LTCL_ALLOCATORS_H

#include <new>

namespace ltc {

/*
 * A rudementary allocator.
 * 
 * Has one major flaw, when construct is called it uses copy construction
 * this can be very troublesome, consider what happens when T is some sort
 * of smart pointer that deletes it's pointer upon going out of scope.
 * Update: Actually std::allocator has the same behavior.
 */
template<class T>
class Allocator {
public:
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = unsigned long long;
	
	Allocator() = default;
	Allocator(const Allocator<T>& a) = default;
	
	pointer allocate(const size_type sz);
	void deallocate(const pointer p, const size_type sz);
	void construct(const pointer p, const value_type& v);
	void destroy(const pointer p);
};// class Allocator


}// namespace ltc

template<class T>
typename ltc::Allocator<T>::pointer
	ltc::Allocator<T>::allocate(const size_type sz) {
	return static_cast<pointer>
		(::operator new(sizeof(value_type) * sz, std::nothrow));
}
	
template<class T>
void ltc::Allocator<T>::deallocate(const pointer p, const size_type) {
	//ignore size_type argument
	::operator delete(p);
}
	
template<class T>
void ltc::Allocator<T>::construct(const pointer p, const value_type& v) {
	::new((void*)p) value_type{v};
}
template<class T>
void ltc::Allocator<T>::destroy(const pointer p) {
	p->~value_type();
}

#endif
