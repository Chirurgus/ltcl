#ifndef GUARD_LTCL_LIST_H
#define GUARD_LTCL_LIST_H

#include "exceptions.h"
#include "allocators.h"

namespace ltc {

template<class T, class Alloc = ltc::Allocator<T>>
class List;

namespace detail {

class List_node_base {
public:
	List_node_base* insert(List_node_base* p);
	List_node_base* insert_after(List_node_base* p);
	List_node_base* splice_in(List_node_base* first,
				  List_node_base* last);
	List_node_base* splice_in_after(List_node_base* first,
				        List_node_base* last);

	List_node_base* cut_out();
	List_node_base* cut_out(List_node_base* last);
	
	List_node_base* next() const { return _next; }
	List_node_base* prev() const { return _prev; }
	
	virtual ~List_node_base() {}
private:
	List_node_base* _next{nullptr};
	List_node_base* _prev{nullptr};
};

template<class T>
class List_node : public List_node_base {
public:
	using value_type = T;

	using reference = value_type&;
	using const_reference = const value_type &;

	List_node() = delete;
	explicit List_node(const_reference val): _elem{val} {}
	explicit List_node(value_type&& val): _elem{val} {}

	const_reference elem() const { return _elem; }
	reference elem() { return _elem; }
	reference elem(const_reference p) { return _elem = p; }

private:
	value_type _elem;
};// class List_node

template<class T>
class List_iterator {
public:
	template<class U, class V> friend class ltc::List;

	using value_type = T;

	using reference = value_type&;
	using const_reference = const value_type&;

	explicit List_iterator(List_node_base * const p)
		: _curr{static_cast<List_node<T>*>(p)} {}
	
	List_iterator<value_type>& operator++();
	List_iterator<value_type>& operator++(int);
	
	List_iterator<value_type>& operator--();
	List_iterator<value_type>& operator--(int);
	
	reference operator*();
	const_reference operator*() const;

	explicit operator bool() const {
		return _curr != nullptr;
	}
		
	bool operator==(const List_iterator<value_type>& it) const {
		return _curr == it._curr;
	}
	bool operator!=(const List_iterator<value_type>& it) const {
		return !(*this == it);
	}

private:
	List_node<value_type>* node() { return _curr; }
	
	List_node<value_type>* _curr {nullptr};
};// class List_iteator

template<class T>
class List_reverse_iterator : public List_iterator<T> {
public:
	using value_type = T;

	using List_iterator<value_type>::List_iterator;

	using reference = typename List_iterator<value_type>::reference;
	using const_reference = 
		typename List_iterator<value_type>::const_reference;


	List_reverse_iterator<value_type>& operator++() {
		List_iterator<value_type>::operator--();
		return *this;
	}
	List_reverse_iterator<value_type>& operator++(int) {
		List_reverse_iterator<value_type>& ret{*this};
		List_iterator<value_type>::operator--();			
		return ret;
	}
	
	List_reverse_iterator<value_type>& operator--() {
		List_iterator<value_type>::operator++();
		return *this;
	}
	List_reverse_iterator<value_type>& operator--(int) {
		List_reverse_iterator<value_type>& ret{*this};
		List_iterator<value_type>::operator++();			
		return ret;
	}
};//class List_reverse_iterator

}// namespace detail

//List_node members

detail::List_node_base* detail::List_node_base::insert(List_node_base* p) 
{
	if (p) {
		p->_next = this;
		p->_prev = _prev;
	}
	if (_prev) {
		_prev->_next = p; 
	}
	_prev = p;	
	return this;
}

detail::List_node_base* detail::List_node_base::insert_after(List_node_base* p) 
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

detail::List_node_base* detail::List_node_base::splice_in(List_node_base* first,
						      	  List_node_base* last)
{
	if (!first || !last) return this;//maybe throw instead?
	if (last->_next) {
		last->_next->_prev = first->_prev ? first->_prev : nullptr;
	}
	if (first->_prev) {
		first->_prev->_next = last->_next ? last->_next : nullptr;
	}
	if (_prev) {
		_prev->_next = first;
	}
	first->_prev = _prev;
	last->_next = this;
	_prev = last;
	return this;	
}

detail::List_node_base*
	detail::List_node_base::splice_in_after(List_node_base* first,
						List_node_base* last)
{
	if (!first || !last) throw ltc::out_of_range
			{"splice_in_after(List_node_base*,List_node_base*)"
			 " called on an invalid range."};
	if (last->_next) {
		last->_next->_prev = first->_prev ? first->_prev : nullptr;
	}
	if (first->_prev) {
		first->_prev->_next = last->_next ? last->_next : nullptr;
	}
	if (_next) {
		_next->_prev = last;
	}
	first->_prev = this;
	last->_next = _next;
	_next = first;
	return this;	
}

detail::List_node_base* detail::List_node_base::cut_out() 
{
	if (_next) _next->_prev = _prev;
	if (_prev) _prev->_next = _next;
	_next = _prev = nullptr;
	return this;
}

detail::List_node_base* detail::List_node_base::cut_out(List_node_base* last)
{
	if (!last) throw ltc::out_of_range{"cut_out(List_node_base*) called on "
					   "a invalid range."};
	if (_prev) _prev->_next = last->_next;
	if (last->_next) last->_next->_prev = _prev;
	_prev = last->_next = nullptr;
	return this;
}

//List_node

//List_iterator

template<class T>
inline detail::List_iterator<T>& detail::List_iterator<T>::operator++() 
{
	 _curr = static_cast<detail::List_node<T>*>(_curr->next()); 
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
	_curr = static_cast<detail::List_node<T>*>(_curr->prev());
	return *this;
}

template<class T>
detail::List_iterator<T>& 
	detail::List_iterator<T>::operator--(int) 
{
	List_iterator<T> tmp {*this};
	--*this;
	return tmp;
}

template<class T>
typename detail::List_iterator<T>::value_type&
	detail::List_iterator<T>::operator*()
{
	return _curr->elem();
}

template<class T>
typename detail::List_iterator<T>::const_reference 
	detail::List_iterator<T>::operator*() const
{
	return _curr->elem();
}


template<class T, class Alloc>
class List {
public:
	using value_type = T;
	using pointer = value_type*;
	using size_type = unsigned long long;
	
	using reference = value_type&;
	using const_reference = const value_type&;	

	using iterator = detail::List_iterator<value_type>;
	using const_iterator = detail::List_iterator<const value_type>;	
	using reverse_iterator = detail::List_reverse_iterator<value_type>;
	using const_reverse_iterator = 
		detail::List_reverse_iterator<const value_type>;

	List(const size_type n = 0, const_reference val = value_type{});
	List(const List<value_type, Alloc>& l);
	List(List<value_type, Alloc>&& l);
	/*
	template<class Iter>
	List(const Iter start, const Iter finish) : List() {
		for (Iter i {start}; i != finish; ++i) {
			push_back(*i);
		}
	}
	*/

	List<value_type, Alloc>& operator=(const List<value_type, Alloc>& l);
	List<value_type, Alloc>& operator=(List<value_type, Alloc>&& l);

	~List();

	bool empty() const;
	size_type size() const;
	
	void resize(const size_type sz, const_reference val = value_type{});
	void swap(List<value_type, Alloc>& l);
	
	void clear();
	void erase(const iterator& i);
	void push_back(const value_type& t);
	void pop_back();
	void push_front(const value_type& t);
	void pop_front();
	
	iterator insert(const iterator pos, const_reference val);
	void insert(const iterator pos, const size_type n, const_reference val);
	template<class Iter>
	void insert(const iterator pos, const Iter start, const Iter end) {
		for (Iter i {start}; i != end; ++i) {
			insert(pos, *i);
		}
	}
	
	void merge(List<T,Alloc>& other);

	void splice(iterator pos, List<T,Alloc>& l);
	void splice(iterator pos, List<T,Alloc>& l, iterator i);
	void splice(iterator pos, List<T,Alloc>& l,
		    iterator first, iterator last);

	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;
	
	iterator begin();
	iterator end();
	const_iterator cbegin() const;
	const_iterator cend() const;
	reverse_iterator rbegin();
	reverse_iterator rend();
	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;
	
private:
	using Base_node = detail::List_node_base;
	using Node = detail::List_node<value_type>;
	using Node_pointer = Node*;
	
	using Node_allocator = typename Alloc::template rebind<Node>::other;

	Node_pointer allocate_node(const_reference t);
	void deallocate_node(Base_node* bp);
	
	void initialize_senteniel();	
	void clear_senteniel();

	Base_node _sent {}; //prev() = last node
			    //next() = first node
	Node_allocator _node_alloc {}; 
};// class List

}// namespace ltc

//List members

template<class T, class A>
ltc::List<T,A>::List(size_type n, const_reference val)
{
	initialize_senteniel();
	for (size_type i {0}; i < n; ++i) {
		push_back(val);
	}
}

template<class T, class A>
ltc::List<T,A>::List(const List<value_type, A>& l)
{
	initialize_senteniel();
	clear();
	for (const_iterator it {l.cbegin()}; it != l.cend(); ++it) {
		push_back(*it);
	}
}

template<class T, class A>
ltc::List<T,A>::List(List<value_type, A>&& l)
	: _sent{}
	, _node_alloc{l._node_alloc}
{
	initialize_senteniel();
	_sent.splice_in_after(l._sent.next()->cut_out(l._sent.prev()),
			      l._sent.prev());
}

/*//
template<class T, class A, class Iter>
ltc::List<T,A>::List(const Iter start, const Iter finish) : List()
{
	for (Iter i {start}; i != finish; ++i) {
		push_back(*i);
	}	
}
*/
	
template<class T, class A>
ltc::List<T,A>& ltc::List<T,A>::operator=(const List<T,A>& l) {
	clear();
	for (const_iterator it{l.cbegin()}; it != l.cend(); ++it) {
		push_back(*it);
	}
}

template<class T, class A>
ltc::List<T,A>& ltc::List<T,A>::operator=(List<T,A>&& l)
{
	clear();
	_sent.splice_in_after(l._sent.next()->cut_out(l._sent.prev()),
			      l._sent.prev());
	_node_alloc = l._node_alloc;	
}

template<class T, class A>
ltc::List<T,A>::~List() 
{
	clear(); 
}

template<class T, class A>
inline bool ltc::List<T,A>::empty() const 
{ 
	return _sent.prev() == &_sent; 
}

template<class T, class A>
inline typename ltc::List<T,A>::size_type ltc::List<T,A>::size() const
{
	return ltc::distance(cbegin(), cend());
}
	
template<class T, class A>
void ltc::List<T,A>::resize(const size_type sz, const_reference val)
{
	size_type old_sz{size()};
	if (old_sz < sz) {
		for (size_type i {0}; i < sz - old_sz; ++i) {
			push_back(val);
		}
	}
	else {
		for (size_type i {0}; i < old_sz - sz; ++i) {
			pop_back();
		} 
	}
}

template<class T, class A>
void ltc::List<T,A>::swap(List<T,A>& l)
{
	Base_node* last {_sent.prev()};
	splice(end(), l);
	l.splice(l.begin(), *this, iterator{_sent.next()}
				 , ltc::next(iterator{last}));	
}
	
template<class T, class A>
void ltc::List<T,A>::clear() 
{
	for (Base_node *curr {_sent.next()},
		       *next {curr->next()};
		curr != &_sent;
		curr = next, next = next->next()) {
		deallocate_node(curr);
	}
	clear_senteniel();
}

template<class T, class A>
void ltc::List<T,A>::erase(const iterator& pos)
{
	if (empty() || !pos) throw ltc::out_of_range
			{"erase(iterator) called on an invalid iterator."};
	deallocate_node(pos.node()->cut_out());	
}

template<class T, class A>
void ltc::List<T,A>::push_back(const value_type& t)
{
	_sent.prev()->insert_after(allocate_node(t));
}

template<class T, class A>
void ltc::List<T,A>::pop_back()
{
	if (empty()) throw ltc::out_of_range{"pop_back() called on an empty "
						"list"};
	deallocate_node(_sent.prev()->cut_out());
}
		
template<class T, class A>
void ltc::List<T,A>::push_front(const value_type& t)
{
	_sent.next()->insert(allocate_node(t));
}
	
template<class T, class A>
void ltc::List<T,A>::merge(List<T,A>& other)
{
	if (other.empty()) return;
	_sent.prev()->splice_in_after
			(other._sent.next().node()->cut_out(other._sent.prev()),
			 other._sent.prev());
}

template<class T, class A>
void ltc::List<T,A>::splice(iterator pos, List<T,A>& l)
{
	if (!pos) throw ltc::out_of_range
				{"splice(iterator,list)"
				 " called on an invalid iterator"};
	pos.node()->
		splice_in(l._sent.next()->cut_out(l._sent.prev()),
			  l._sent.prev());
}

template<class T, class A>
void ltc::List<T,A>::splice(iterator pos, List<T,A>& l, iterator l_pos)
{
	if (!pos || !l_pos) throw ltc::out_of_range
				{"splice(iterator,list,iterator,iterator)"
				 " called on an invalid iterator"};
	pos.node()->insert(l_pos.node()->cut_out());
}

template<class T, class A>
void ltc::List<T,A>::splice(iterator pos, List<T,A>& l,
			    iterator first, iterator last)
{
	if (!pos || !first || !last) throw ltc::out_of_range
				{"splice(iterator,list,iterator,iterator)"
				 " called on an invalid iterator"};
	pos.node()->splice_in(first.node()->cut_out(last.node()->prev()),
			     last.node()->prev());
}

template<class T, class A>
void ltc::List<T,A>::pop_front() 
{
	if (empty()) throw ltc::out_of_range
				{"pop_front called on an empty list"};
	Node_pointer to_delete 
		{_sent.next()->cut_out()};
	deallocate_node(to_delete);
}

template<class T, class A>
void ltc::List<T,A>::insert(const iterator pos,
			    const size_type n,
			    const_reference val)
{
	for (size_type i{n}; i < n; ++i) {
		insert(pos,val);
	}
}

template<class T, class A>
typename ltc::List<T,A>::iterator 
	ltc::List<T,A>::insert(const iterator pos, const_reference val)
{
	if (!pos.node()) throw ltc::out_of_range
				{"insert called on an invalid iterator"};
	pos.node()->insert(allocate_node(val));
}

/*
template<class T, class A, class Iter>
void List<T,A>::insert(const iterator pos, const Iter start, const Iter finish)
{
	for (Iter i {start}; i != finish; ++i) {
		insert(pos, *i);
	}
}
*/

template<class T, class A>
typename ltc::List<T,A>::reference ltc::List<T,A>::front()
{
	if (empty()) throw ltc::out_of_range
				{"front() called on an empty list."};
	return *begin();
}

template<class T, class A>
typename ltc::List<T,A>::const_reference ltc::List<T,A>::front() const
{
	if (empty()) throw ltc::out_of_range
				{"front() called on an empty list."};
	return *cbegin();
}

template<class T, class A>
typename ltc::List<T,A>::reference ltc::List<T,A>::back()
{
	if (empty()) throw ltc::out_of_range
				{"back() called on an empty list."};
	return *iterator{_sent.prev()};
}

template<class T, class A>
typename ltc::List<T,A>::const_reference ltc::List<T,A>::back() const
{
	if (empty()) throw ltc::out_of_range
				{"back() called on an empty list."};
	return *iterator{_sent.prev()};
}

template<class T, class A>
inline typename ltc::List<T,A>::iterator ltc::List<T,A>::begin()
{
	return iterator{_sent.next()};
}

template<class T, class A>
inline typename ltc::List<T,A>::iterator ltc::List<T,A>::end()
{
	return iterator{&_sent};
}

template<class T, class A>
inline typename ltc::List<T,A>::const_iterator ltc::List<T,A>::cbegin() const
{
	return const_iterator{_sent.next()};
}

template<class T, class A>
inline typename ltc::List<T,A>::const_iterator ltc::List<T,A>::cend() const
{
	return const_iterator{_sent.next()->prev()};
}

template<class T, class A>
inline typename ltc::List<T,A>::reverse_iterator
	ltc::List<T,A>::rbegin()
{
	return reverse_iterator{_sent.prev()};
}

template<class T, class A>
inline typename ltc::List<T,A>::reverse_iterator
	ltc::List<T,A>::rend()
{
	return reverse_iterator{&_sent};
}

template<class T, class A>
inline typename ltc::List<T,A>::const_reverse_iterator
	ltc::List<T,A>::crbegin() const
{
	return const_reverse_iterator{_sent.prev()};
}

template<class T, class A>
inline typename ltc::List<T,A>::const_reverse_iterator
	ltc::List<T,A>::crend() const
{
	return const_reverse_iterator{_sent.next()->prev()};
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

/*
 * Base_node does not have a virtual destructor, but this is safe since classes
 * derived from Base_node won't have non-emtpy destructors.
 */
template<class T, class A>
void ltc::List<T,A>::deallocate_node(Base_node* bp)
{
	Node_pointer p {static_cast<Node_pointer>(bp)};
	_node_alloc.destroy(p);
	_node_alloc.deallocate(p, sizeof(Node));	
}

template<class T, class A>
void ltc::List<T,A>::initialize_senteniel()
{
	_sent.insert(&_sent); 
}

template<class T, class A>
void ltc::List<T,A>::clear_senteniel()
{
	_sent = Base_node{};
	initialize_senteniel();
}

#endif
