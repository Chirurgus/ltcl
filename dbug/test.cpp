#include <iostream>
#include <vector>

#include "../ltcl/allocators.h"
#include "../ltcl/vector.h"

int main() {
	ltc::Vector<int> v;
	for(unsigned i {0}; i < 10000; ++i) {
		v.push_back(i);
	}
	for (const auto& i : v) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;	
}
