#include <iostream>

#include "../ltcl/iterators.h"
#include "../ltcl/list.h"
#include "../ltcl/exceptions.h"

//#include <list>
#define WRITE(x) std::cout << x << std::endl

int main() {
	ltc::List<int> l1(10, 1);
	ltc::List<int> l2(5, 2);
	WRITE("l1:");
	WRITE(l1.size());
	for (auto x : l1) WRITE(x);
	WRITE("l2:");
	WRITE(l2.size());
	for (auto x : l2) WRITE(x);
	
	l1.swap(l2);

	WRITE("l1:");
	WRITE(l1.size());
	for (auto x : l1) WRITE(x);
	WRITE("l2:");
	WRITE(l2.size());
	for (auto x : l2) WRITE(x);
}
