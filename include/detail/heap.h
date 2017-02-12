#ifndef GUARD_LTCL_DETAIL_HEAP_H
#define GUARD_LTCL_DETAIL_HEAP_H 

#include "../vector.h"
#include "../utility.h"

namespace ltc {

namespace detail {

template<class T, class K, class Cmp, class Alloc>
class Heap {
public:
	using value_type = T;
	using key_type = K;
	using size_type = container_type::size_type;	
	
	Heap(): _heap() {}
			
		
private:
	using pair_type = ltc::Pair<T,K>;
	using allocator_type = 
		template Alloc::template rebind<pair_type>::other;
	using container_type = ltc::Vector<pair_type, allocator_type>;

	container_type _heap;
	Cmp criteria;//How should I call this ????
};// Heap

}// namespace ltc

}// namespace detail

#endif// !GUARD_LTCL_DETAIL_HEAP_H
