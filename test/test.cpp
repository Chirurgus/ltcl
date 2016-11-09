#include "../ltcl/vector.h"
#include "../ltcl/exceptions.h"

#include <iostream>

int main() {
	ltc::Vector<int> v(10, 11);
	try {
		std::cout << "size = " << v.size() << std::endl;
		std::cout << "capacity = " << v.capacity() << std::endl;
		v.reserve(20);
		std::cout << "size = " << v.size() << std::endl;
		std::cout << "capacity = " << v.capacity() << std::endl;
		v.shrink_to_fit();
		std::cout << "size = " << v.size() << std::endl;
		std::cout << "capacity = " << v.capacity() << std::endl;
	}
	catch (ltc::exception& err) {
		std::cout << err.what() << std::endl;
		throw;
	}
	return 0;
}
