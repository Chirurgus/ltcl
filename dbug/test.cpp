#include <iostream>

#include "../ltcl/string.h"

#define WRITE(x) std::cout << x << std::endl

template<class C, class A>
void print_string(const ltc::String<C,A>& str) {
	for (auto x : str) {
		std::cout << x;
	}
	std::cout << std::endl;
}

int main() {
	ltc::String<char> str{"hi"};
	print_string(str);
}
