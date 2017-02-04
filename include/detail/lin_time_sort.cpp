#include <iterator>

#include "lin_time_sort.h"

std::vector<unsigned> countsort(std::vector<unsigned>& a, unsigned k) {
	std::vector<unsigned> c(k + 1, 0);
	for (std::vector<unsigned>::iterator i = a.begin();
		 i != a.end();
		 ++i) {
		++c[*i];
	}
	for (std::vector<unsigned>::iterator i = std::next(c.begin());
		 i != c.end();
		 ++i) {
		*i += *std::prev(i);
	}
	std::vector<unsigned> ret(a.size());
	for (std::vector<unsigned>::reverse_iterator i = a.rbegin();
		 i != a.rend();
		 ++i) {
		ret[c[*i] - 1] = *i;
		--c[*i];
	}
	return ret;;
}
