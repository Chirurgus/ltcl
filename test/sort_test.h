#ifndef GUARD_LTCL_TEST_SORT_H
#define GUARD_LTCL_TEST_SORT_H

#include "../include/vector.h"
#include <random>
#include <limits>

namespace ltc {

namespace test {

/*
	test function has to take a std::vector
	and has to sort the vector in place 
*/
bool test_sort(void test(std::vector<int>& a),
			  std::vector<int>::size_type n = 0xFFFF);

std::vector<int> genRandVector(const std::vector<int>::size_type& n);
	

/* Generates vector containing all ints between 0 and INT_MAX
   in random order.
   No dublicates, or negative numbers 
 */
vector<int> genRandVector(const vector<int>::size_type& n) {
	vector<int> ret_v(n);

	for (vector<int>::size_type i = 0;
		 i < n;
		 ++i) {
		ret_v[i] = i;
	}
	for (vector<int>::size_type i = 0;
		 i < n;
		 ++i) {
		vector<int>::size_type rand_pos = rand() % n;
		int tmp = ret_v[i];
		ret_v[i] = ret_v[rand_pos];
		ret_v[rand_pos] = tmp;
	}

	return ret_v;
}

/* Dummy function to passed to benchmark */
bool sortTest(void test(vector<int>& a), vector<int>::size_type n) {
	vector<int> test_vector = genRandVector(n);
	
	test(test_vector); // Sort the vector

	for (vector<int>::size_type i = 1; i < test_vector.size(); ++i) {
		if (test_vector[i] < test_vector[i - 1])
			return false;
	}

	if (test_vector.size() != n) {
		return false;
	}
	
	return true;
}

}// namespace test
}// namespace ltc
#endif// GUARD_LTCL_TEST_SORT_H
