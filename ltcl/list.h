#ifndef GUARD_LTCL_LIST_H
#define GUARD_LTCL_LIST_H

#include "exceptions.h"
#include "allocators.h"

namespace ltc {

namespace detail {

template<class T>
class List_node {
public:
	using value_type = T;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	
	List_node() = delete;
	//List_node(const List_node<T>&) = delete;
	//List_node<T>& operator=(const List_node<T>&) = delete;
	
	~List_node() = default;	

	explicit List_node(const_reference val): _elem{val} {}
	explicit List_node(value_type&& val): _elem{val} {}

	List_node<value_type>* next() const
		{ return _next; }
	List_node<value_type>* next(const List_node<value_type>* p)
		{ return _next = p; }
	List_node<value_type>* prev() const 
		{ return _prev; }
	List_node<value_type>* prev(const List_node<value_type>* p)
		{ return _prev = p; }
	const_reference elem() const
		{ return _elem; }
	reference elem()
		{ return _elem; }
	reference elem(const_reference p) 
		{ return _elem = p; }

	List_node<value_type>* insert(List_node<value_type>* p);

	List_node<value_type>* insert_after(List_node<value_type>* p);

	List_node<T>* cut_out();

private:
	List_node<value_type>* _next {nullptr};
	List_node<value_type>* _prev {nullptr};
	value_type _elem;
};// class List_node

template<class T>
class List_iterator {
public:
	using value_type = T;

	List_iterator(): _curr{nullptr} {}
	List_iterator(List_node<T>* p): _curr{p} {}
	
	List_iterator<T>& operator++();
	List_iterator<T>& operator++(int);
	
	List_iterator<T>& operator--();
	List_iterator<T>& operator--(int);
	
	value_type& operator*();

	explicit operator bool() const
		{ return _curr != nullptr; }
		
	bool operator==(const List_iterator<T>& it) 
		{ return _curr == it._curr; }
	bool operator!=(const List_iterator<T>& it) 
		{ return !(*this == it); }


	List_node<T>* node() const { return _curr; }
private:

	List_node<T>* _curr {};
};// class List_forward_iteator

template<class T>
class List_reverse_iterator {
public:
	List_reverse_iterator(): _curr {nullptr} {}
	
private:
	List_node<T>* _curr;
};// class List_reverse_iterator

}// namespace detail

//List_node members

template<class T>
detail::List_node<T>* 
	detail::List_node<T>::insert(List_node<T>* p) 
{
	if (p) {
		p->_next = this;
		p->_prev = _prev;
	}
	if (_prev) {
		_prev->_next = p; }
	_prev = p;	
	return this;
}

template<class T>
detail::List_node<T>* 
	detail::List_node<T>::insert_after(List_node<T>* p) 
{
	if (p) {
		p->_next = _next;
		p->_prev = this;
	}
	if (_next) {
		_next->_prev = p;
	}
	_next = p;	
	return this;
}

template<class T>
detail::List_node<T>* 
	detail::List_node<T>::cut_out() 
{
	if (_next) _next->_prev = _prev;
	if (_prev) _prev->_next = _next;
	_next = _prev = nullptr;
	return this;
}

//List_iterator

template<class T>
inline detail::List_iterator<T>& 
	detail::List_iterator<T>::operator++() 
{
	 _curr = _curr->next(); 
	return *this;
}

template<class T>
detail::List_iterator<T>& 
	detail::List_iterator<T>::operator++(int) 
{
	List_iterator<T> tmp {*this};
	++*this;
	return tmp;
}

template<class T>
inline detail::List_iterator<T>& 
	detail::List_iterator<T>::operator--() 
{
	--_curr;
	return *this;
}

template<class T>
detail::List_iterator<T>& 
	detail::List_iterator<T>::operator--(int) 
{
	List_iterator<T> tmp {*this};
	--_curr;
	return tmp;
}

template<class T>
typename detail::List_iterator<T>::value_type&
	detail::List_iterator<T>::operator*()
{
	return _curr->elem();
}


template<class T, class Alloc = ltc::Allocator<T>>
class List {
public:
	using value_type = T;
	using pointer = value_type*;
	using size_type = unsigned long long;
	
	using reference = value_type&;
	using const_reference = const value_type&;	

	using iterator = detail::List_iterator<value_type>;
	using const_iterator = detail::List_iterator<const value_type>;	

	List(size_type = 0, const_reference val = value_type{});
	List(const List<value_type, Alloc>& l);
	List(List<value_type, Alloc>&& l);

	List<value_type, Alloc>& operator=(const List<value_type, Alloc>& l);
	List<value_type, Alloc>& operator=(List<value_type, Alloc>&& l);

	~List();

	bool empty() const;
	size_type size() const;
	
	void resize(const size_type i, const_reference t = value_type{});
	void swap(List<value_type, Alloc>& l);
	
	void clear();
	iterator insert(const iterator& i, const_reference t);
	void erase(const iterator& i);
	void push_back(const value_type& t);
	void pop_back();
	void push_front(const value_type& t);
	void pop_front();

	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;
	
	iterator begin();
	iterator end();
	
private:
	using Node = detail::List_node<value_type>;
	using Node_pointer = Node*;

	using Value_allocator = Alloc;
	using Node_allocator = typename Alloc::template rebind<Node>::other;

	//allcoates node, and T*
	Node_pointer allocate_node(const_reference t);
	//deallocates node, and T* if needed
	void deallocate_node(Node_pointer p);
	
	size_type _size {0};
	Node_pointer _head {nullptr};
	Node_pointer _tail {nullptr};
	Alloc _val_alloc {};
	Node_allocator _node_alloc {}; 
};// class List

}// namespace ltc

//List members

template<class T, class A>
ltc::List<T,A>::List(size_type n, const_reference val)
{
	for (size_type i {0}; i < n; ++i) {
		push_back(val);
	}
}

template<class T, class A>
ltc::List<T,A>::List(const List<value_type, A>& l)
{
	clear();
	for (const_reference x : l) {
		push_back(x);
	}
}

template<class T, class A>
ltc::List<T,A>::List(List<value_type, A>&& l) {
	_size = l._size;
	_head = l._head;
	_tail = l._tail;
	_val_alloc = l._val_alloc;
	_node_alloc = l._node_alloc;	
	
	l._size = 0;
	l._head = l._tail = nullptr;
}
	
template<class T, class A>
ltc::List<T,A>& ltc::List<T,A>::operator=(const List<T,A>& l) {
	clear();
	for (const_reference x : l) {
		push_back(x);
	}
}

template<class T, class A>
ltc::List<T,A>& ltc::List<T,A>::operator=(List<T,A>&& l)
{
	_size = l._size;
	_head = l._head;
	_tail = l._tail;
	_val_alloc = l._val_alloc;
	_node_alloc = l._node_alloc;	
	
	l._size = 0;
	l._head = l._tail = 0;
}

template<class T, class A>
ltc::List<T,A>::~List() 
{
	clear(); 
}

template<class T, class A>
bool ltc::List<T,A>::empty() const 
{ 
	return _size == 0; 
}

template<class T, class A>
typename ltc::List<T,A>::size_type ltc::List<T,A>::size() const
{
	return _size;
}
	
template<class T, class A>
void ltc::List<T,A>::resize(const size_type sz, const_reference t)
{
	if (_size < sz) {
		for (size_type i {0}; i < sz - _size; ++i) {
			push_back(t);
		}
	}
	else {
		for (size_type i {0}; i < _size - sz; ++i) {
			pop_back();
		} 
	}
	_size = sz;
}

template<class T, class A>
void ltc::List<T,A>::swap(List<T,A>& l)
{
	size_type size_tmp {_size};
	Node_pointer head_tmp {_head};
	Node_pointer tail_tmp {_tail};
	Value_allocator v_alloc_tmp {_val_alloc};
	Node_allocator n_alloc_tmp {_node_alloc};

	_size = l._size;
	_head = l._head;
	_tail = l._tail;
	_val_alloc = l._val_alloc;
	_node_alloc = l._val_alloc;

	l._size = size_tmp;
	l._head = head_tmp;
	l._tail = tail_tmp;
	l._val_alloc = v_alloc_tmp;
	l._node_alloc = n_alloc_tmp;
}
	
template<class T, class A>
void ltc::List<T,A>::clear() 
{
	if (empty()) return;
	iterator i {begin()};
	iterator j {++begin()};
	while (i != end()) {
		deallocate_node(i.node());
		i = j;
		if (j) ++j;
	}
	_head = _tail = nullptr;
	_size = 0;
}

template<class T, class A>
typename ltc::List<T,A>::iterator
	ltc::List<T,A>::insert(const iterator& i, const_reference t)
{
	Node_pointer new_node {allocate_node(t)};
	if (i.node()) i.node()->insert(new_node);	
	else _head = _tail = new_node;
	++_size;
	return iterator{new_node};
}

template<class T, class A>
void ltc::List<T,A>::erase(const iterator& i)
{
	if (!i.node()) return;
	Node_pointer to_delete {i.node()->cut_out()};
	deallocate_node(to_delete);	
	--_size;
}

template<class T, class A>
void ltc::List<T,A>::push_back(const value_type& t)
{
	Node_pointer new_ptr {allocate_node(t)};
	if (!empty()) {
		_tail->insert_after(new_ptr);
		_tail = new_ptr;
	}
	else {
		_head = _tail = new_ptr;
	}
	++_size;
}

template<class T, class A>
void ltc::List<T,A>::pop_back()
{
	if (empty()) {
		throw ltc::out_of_range{"pop_back() called on empty list."};
	}
	Node_pointer to_delete {_tail};
	_tail = _tail->prev();
	to_delete->cut_out();
	deallocate_node(to_delete);
	--_size;
}
		
template<class T, class A>
void ltc::List<T,A>::push_front(const value_type& t)
{
	Node_pointer new_ptr {allocate_node(t)};
	if (!empty()) {
		_head->insert(new_ptr);
		_head = new_ptr;
	}
	else {
		_head = _tail = new_ptr;
	}
	++_size;
}
	
template<class T, class A>
void ltc::List<T,A>::pop_front() 
{
	if (empty()) {
		throw ltc::out_of_range
			{"pop_front() called on an empty list."};	
	}
	Node_pointer to_delete {_head};
	_head = _head->next();
	to_delete->cut_out();
	deallocate_node(to_delete);
	--_size;
}


template<class T, class A>
typename ltc::List<T,A>::reference ltc::List<T,A>::front()
{
	if (empty()) throw ltc::out_of_range
				{"front() called on an empty list."};
	return _head->elem();
}

template<class T, class A>
typename ltc::List<T,A>::const_reference ltc::List<T,A>::front() const
{
	if (empty()) throw ltc::out_of_range
				{"front() called on an empty list."};
	return _head->elem();
}

template<class T, class A>
typename ltc::List<T,A>::reference ltc::List<T,A>::back()
{
	if (empty()) throw ltc::out_of_range
				{"back() called on an empty list."};
	return _tail->elem();
}

template<class T, class A>
typename ltc::List<T,A>::const_reference ltc::List<T,A>::back() const
{
	if (empty()) throw ltc::out_of_range
				{"back() called on an empty list."};
	return _tail->elem();
}

template<class T, class A>
inline typename ltc::List<T,A>::iterator ltc::List<T,A>::begin()
{
	return iterator{_head};
}

template<class T, class A>
inline typename ltc::List<T,A>::iterator ltc::List<T,A>::end()
{
	return iterator{nullptr};
}

//allcoates node, and T*
template<class T, class A>
typename ltc::List<T,A>::Node_pointer
	ltc::List<T,A>::allocate_node(const_reference t)
{
	Node_pointer new_node {_node_alloc.allocate(sizeof(Node))};
	_node_alloc.construct(new_node, Node{t});	
	return new_node;
}

//deallocates node, and T* if needed
template<class T, class A>
void ltc::List<T,A>::deallocate_node(Node_pointer p)
{
	_node_alloc.destroy(p);
	_node_alloc.deallocate(p, sizeof(Node));	
}

#endif
