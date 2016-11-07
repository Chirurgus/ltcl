#include "../ltcl/vector.h"

#include <iostream>

int main() {
	ltc::Vector<int> v;
	for (unsigned i {0}; i < 10; ++i) {
		v.push_back(i);
	}
	std::cout << "v size = " << v.size() << '\n';
	for (const auto& x : v) {
		std::cout << x << '\n';
	}
	
	return 0;
}
