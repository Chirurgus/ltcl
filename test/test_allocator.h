#ifndef GUARD_LTCL_TEST_ALLOCATOR_H
#define GUARD_LTCL_TEST_ALLOCATOR_H

#include "../ltcl/allocators.h"

class obj {
public:
	obj(const obj& o):_i{o._i} {}
	obj(int i):_i{i} {}
	int& get() { return _i; }
	~obj() { _i = 0; std::cout << "Destroying obj" << std::endl; }
private:
	 int _i;
};

template<class T>
class ptr {
public:
	ptr(T* p): _p(p) {}
	ptr(const ptr& p) = default;
	ptr(ptr&& p): _p(p._p) { p._p = nullptr; }
	~ptr() { delete _p; }
private:
	T* _p {nullptr};	
};

void test_allocator() {
}

void test_smart_ptr() {
	ltc::Allocator<ptr<obj>> a;
	ptr<obj>* p {a.allocate(1)};
	//std::cout << (*p).get() << '\n';
	a.construct(p, new obj{123});
	std::cout << (*(*p)).get() << '\n';
	a.destroy(p);
	std::cout << (*(*p)).get() << '\n';
	a.deallocate(p,1);
}
#endif 
