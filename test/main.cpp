#include "../ltcl/vector.h"

#include <iostream>

int main() {
	ltc::Vector<int> v(10,12345);
	std::cout << v.size() << '\n';
	for (unsigned i {0}; i < v.size(); ++i) {
		std::cout << v[i] << '\n';
	}
	
	return 0;
}
