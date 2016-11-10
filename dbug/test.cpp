#include "../ltcl/vector.h"
#include "../ltcl/exceptions.h"

#include <iostream>

int main() {
	ltc::Vector<int> v(10, 11);
	try {
		std::cout << "size = " << v.size() << '\n';
		std::cout << "capacity = " << v.capacity() << '\n';
		ltc::Vector<int> u(0, 0);
		std::cout << "size = " << u.size() << '\n';
		std::cout << "capacity = " << u.capacity() << '\n';
		v.swap(u);
		std::cout << "size = " << v.size() << '\n';
		std::cout << "capacity = " << v.capacity() << '\n';
		std::cout << "size = " << u.size() << '\n';
		std::cout << "capacity = " << u.capacity() << '\n';
	}
	catch (ltc::exception& err) {
		std::cout << err.what() << std::endl;
		throw;
	}
	return 0;
}
