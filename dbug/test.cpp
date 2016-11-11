
#include <iostream>
#include "../ltcl/allocators.h"

class obj {
public:
	obj(const obj& o):_i{o._i} {}
	obj(int i):_i{i} {}
	int& get() { return _i; }
	~obj() { _i = 0; std::cout << "Destroying obj\n"; }
private:
	 int _i;
};
template<class T>
class ptr {
public:
	ptr(T* p): _p(p) {}
	~ptr() { delete _p; }
	T& operator*() { return *_p; }
private:
	T* _p {nullptr};	
};
int main() {
	ltc::Allocator<ptr<obj>> a;
	ptr<obj>* p {a.allocate(1)};
	//std::cout << (*p).get() << '\n';
	a.construct(p, new obj{123});
	std::cout << (*(*p)).get() << '\n';
	a.destroy(p);
	std::cout << (*(*p)).get() << '\n';
	a.deallocate(p,1);
}
