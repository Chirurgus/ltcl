#include <iostream>

#include "../ltcl/list.h"

#define WRITE(x) std::cout << x << '\n'
int main() {
	ltc::List<int> l;
	l.push_back(2);
	l.push_front(1);	
	WRITE(l.front());
	WRITE(l.back());
	l.pop_front();
	l.pop_back();	
	
	
}
