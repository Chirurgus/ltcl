#ifndef GUARD_SORT_TEST_H
#define GUARD_SORT_TEST_H

#include <vector>
#include <random>
#include <limits>

#include "bench.h"

/*
	test function has to take a std::vector
	and has to sort the vector in place 
*/
bool sortTest(void test(std::vector<int>& a),
			  std::vector<int>::size_type n = 0xFFFF);

std::vector<int> genRandVector(const std::vector<int>::size_type& n);
#endif