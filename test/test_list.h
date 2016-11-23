#ifndef GUARD_LTCL_TEST_LIST_H
#define GUARD_LTCL_TEST_LIST_H

#include <list>

#include "../ltcl/list.h"

namespace ltc {
namespace test {

template<class L1, class L2>
bool cmp_list(const L1& l1, const L2& l2);

bool test_constructors() {

}

bool test();
}// namespace test
}// namespace ltc

template<class L1, class L2>
bool ltc::test::cmp_list(const L1& l1, const L2& l2)
{
	bool ret {true};
	ret |= (size() == tl.size());
	Test_list<L>::iterator l_it {begin()};
	Test_list<T>::iterator t_it {tl.begin()}; 
	for (l_it != end() && t_it != tl.end(); ++l_it, ++t_it) {
		ret |= (*t_it == *l_it);
	}
}
#endif
