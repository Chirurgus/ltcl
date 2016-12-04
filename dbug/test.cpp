#include <iostream>

#include "../ltcl/iterators.h"
#include "../ltcl/list.h"
//#include <list>

#define WRITE(x) std::cout << x << std::endl

int main() {
	ltc::List<int> l1(5, 1);
	ltc::List<int> l2(5, 2);
	WRITE("l1:");
	//WRITE(l1.size());
	for (auto x : l1) WRITE(x);
	WRITE("l2:");
	//WRITE(l2.size());
	for (auto x : l2) WRITE(x);

	ltc::List<int>::iterator it {l2.begin()};
	for (unsigned i {0}; i < 3; ++i) ++it;
	l1.splice(l1.begin(), l2, l2.begin(), it);

	WRITE("l1:");
	//WRITE(l1.size());
	for (auto x : l1) WRITE(x);
	WRITE("l2:");
	//WRITE(l2.size());
	for (auto x : l2) WRITE(x);
}
