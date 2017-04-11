#include <iostream>

#include "../test/test_string.h"

#define WRITE(x) std::cout << x << std::endl


int main() {
	try {
		ltc::test::test_string::size();
		ltc::test::test_string::length();
		ltc::test::test_string::capacity();
		ltc::test::test_string::push_back();
		ltc::test::test_string::pop_back();
		ltc::test::test_string::c_str();
		ltc::test::test_string::append();	
	}
	catch (error& err) {
		WRITE(err.what());
	}
}
