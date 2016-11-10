#ifndef GUARD_LTCL_TEST_VECTOR_H
#define GUARD_LTCL_TEST_VECTOR_H

#include "../ltcl/vector.h"

namespace ltc {

namespace test {

template<class T = int>
void test_vector_constructors() {
	ltc::Vector<T> empty_v {};
	ltc::Vector<T> normal_v(16, T{});
	ltc::Vector<T> copy_v(normal_v);
}

template<class T = int>
void test_vector_manip() {
	ltc::Vector<T> v(16);
	v.resize(32);
	v.resize(16);
	v.reserve(64);
	v.resize(32);
	v.shrink_to_fit();
}

template<class T = int>
void test_vector_inspect() {
	ltc::Vector<T> v(16);
	ltc::Vector<T> u(8)
	v.swap(u);	
	v.size();
	v.capacity()
	v.empty();
}

template<class T = int>
void test_vector_access() {
	ltc::Vector<T> v;
	for (unsigned i {0}; i < 10; ++i) {
		v.push_back(i);
	}
	ltc::Vector<T> cp;
	for (const auto& x : v) {
		cp.push_back(x);
	}
	for (ltc::Vector<T>::iterator i{v.begin()};
		i != v.end();
		++i) {}
	for (ltc::Vector<T>::const_iterator i {v.cbegin()};
		i != v.cend();
		++i) {}
	for (ltc::Vector<T>::size_type i {0};
		i < v.size();	
		++i) {}	
}

template<class T = int>
void test_vector() {
	test_vector_constructors();
	test_vector_manip();
	test_vector_inspect();
	test_vector_access();
}

}// namespace test

}// namespace ltc
	
#endif
