#ifndef GUARD_LTCL_DETAIL_HEAP_H
#define GUARD_LTCL_DETAIL_HEAP_H 

#include "../exceptions.h"
#include "../vector.h"
#include "../utility.h"

namespace ltc {

namespace detail {

template<class T,
	 class K,
	 class Compare,
	 class Alloc = ltc::Allocator<T>>
class Heap {
	/* 
 	 * Invariant _cond(parent, child) is true for all parents, and children
 	 * for example if _cond = operator<, Heap is a min-heap.
 	 */
public:
	using value_type = T;
	using reference = value_type&;
	using const_reference = const value_type&;
	using key_type = K;
	using key_reference = key_type&;
	using const_key_reference = const key_type&;
	using size_type = container_type::size_type;	
	
	Heap(): _heap() {}
	
	reference top();
	const_reference top() const;		
	
	void pop();
	void pop_push(const_key_reference key, const_reference val);
	void push(const_key_reference key, const_reference val);
	
	size_type size() const;
	bool empty() const;	
private:
	size_type parent(const size_type i);
	size_type left(const size_type i);
	size_type right(const size_type i);

	void heapify();
	shift_up(const size_type i);
	shift_down(const size_type i);
	
	using pair_type = ltc::Pair<T,K>;
	using allocator_type = 
		template Alloc::template rebind<pair_type>::other;
	using container_type = ltc::Vector<pair_type, allocator_type>;

	container_type _heap;
	Compare _comp;
};// Heap

}// namespace ltc

}// namespace detail


template<class T,class K,class C,class A>
inline typename ltc::detail::Heap<T,K,C,A>::reference
	ltc::detail::Heap<T,K,C,A>::top()
{
	return _heap.front().second;
}

template<class T,class K,class C,class A>
inline typename ltc::detail::Heap<T,K,C,A>::const_reference
	ltc::detail::Heap<T,K,C,A>::top() const
{
	return _heap.first().second;
}

template<class T,class K,class C,class A>
void ltc::detail::Heap<T,K,C,A>::pop()
{
	if (_heap.size() < 2) {
		if (_heap.empty()) {
			throw ltc::out_of_range
				{"ltc::Heap::pop() called on an empty heap."};
		}
		return;
	}

	ltc::swap(_heap.front(),_heap.back());
	_heap.pop_back();

	shift_down(0);
}

template<class T,class K,class C,class A>
void ltc::detail::Heap<T,K,C,A>::
	pop_push(const_key_reference key, const_reference val)
{
	_heap.front() = pair_type{key,val};
	shift_down(0);
}

template<class T,class K,class C,class A>
void ltc::detail::Heap<T,K,C,A>::
	push(const_key_reference key, const_reference val)
{
	_heap.push_back({key,val});	
	
	shift_up(size() - 1);	
}

template<class T,class K,class C,class A>
inline ltc::detail::Heap<T,K,C,A>::size_type
	ltc::detail::Heap<T,K,C,A>::size() const
{
	return _heap.size();
}

template<class T,class K,class C,class A>
inline void ltc::detail::Heap<T,K,C,A>::empty() const
{
	return size() == 0;
}

template<class T,class K,class C,class A>
inline typename ltc::detail::Heap<T,K,C,A>::size_type
	ltc::detail::Heap<T,K,C,A>::parent(const size_type i)
{
	return (i-1)/2;
}

template<class T,class K,class C,class A>
inline typename ltc::detail::Heap<T,K,C,A>::size_type
	ltc::detail::Heap<T,K,C,A>::left(const size_type i)
{
	return i*2 + 1;
}

template<class T,class K,class C,class A>
inline typename ltc::detail::Heap<T,K,C,A>::size_type
	ltc::detail::Heap<T,K,C,A>::right(const size_type i)
{
	return left(i) + 1;
}

template<class T,class K,class C,class A>
void ltc::detail::Heap<T,K,C,A>::heapify()
{
	for (size_type i{_heap.size() - 1}; i > 0; --i) {
		if (!_comp(_heap[parent(i)].second, _heap[i].second)) {
			ltc::swap(_heap[parent(i)], _heap[i]);	
		}
	}
}

template<class T,class K,class C,class A>
void ltc::detail::Heap<T,K,C,A>::shift_up(const size_type pos)
{
	for (size_type i {pos};
		i > 0 && !_comp(_heap[parent(i)],_heap[i]);
		i = parent(i))
	{
		ltc::swap(_heap[i],_heap[parent(i)]);
	}

}

template<class T,class K,class C,class A>
void ltc::detail::Heap<T,K,C,A>::shift_down(const size_type pos)
{
	for (size_type i {pos};
	     ((left(i) < _heap.size() && !_comp(_heap[i].first,
						_heap[left(i)].first)) ||
	      (right(i) < _heap.size() && !_comp(_heap[i].first,
						_heap[right(i)].first)));)
	{
		bool go_left {_comp(_heap[left(i)].first,
				   _heap[right(i)].first)};
		if (go_left) {
			ltc::swap(_heap[i],_heap[left(i)]);
			i = left(i);
		}
		else {
			ltc::swap(_heap[i],_heap[right(i)]);
			i = right(i);
		}
	}

}

#endif// !GUARD_LTCL_DETAIL_HEAP_H
