#ifndef GUARD_LTC_STRING_H
#define GUARD_LTC_STRING_H

#include "allocators.h"
#include "exceptions.h"
#include "utility.h"
#include "vector.h"

namespace ltc
{

namespace detail
{

template<class Char, class Alloc>
union basic_string {

	basic_string();

	destroy_array() {}
	destroy_vector() { _long.~Vector(); }

	~basic_string() {}

	char_type _short[sizeof(ltc::Vector<Char, Alloc>)/sizeof(Char)]{'\0'};
	ltc::Vector<Char,Alloc> _long;
};//union basic_string

}//namespace detail

template<class Char = char, class Alloc = ltc::Allocator<Char>>
class String {
public:
	using char_type = Char;
	using size_type = unsigned long long;
	using reference = char_type&;
	using const_reference = const char_type&;
	using pointer = char_type*;
	using const_pointer = const char_type*;
	using iterator = pointer;
	using const_iterator = const_pointer;

	String(size_type n = 0, char_type c = null_char);
	
	String(const String<Char, Alloc>& str);
	String(String<Char, Alloc>&& str);	
	String(const char_type* cstr);
	String(const char_type* cstr, const size_type len);

	~String();	

	String<Char,Alloc>& operator=(const String<Char,Alloc>& str);
	String<Char,Alloc>& operator=(String<Char, Alloc>&& str);

	reference operator[](const size_type n);
	const_reference operator[](const size_type n) const;

	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;
	
	const char_type* c_str() const;
	
	iterator begin();
	iterator end();	
	
	bool empty();
	size_type size();
	size_type length();
	size_type capacity();
	void reserve(const size_type n);
	void clear();
	void swap(String<Char, Alloc>& str);

	iterator insert(const iterator i, const char_type t);
/*
	erase
*/
	void push_back(const char_type c);
	void pop_back();
	
/*
	String<char_type, Alloc> operator+=(
*/
/*
	bool compare(const String<Char, Alloc>& str);
*/
/*
	replace
*/
	
	
private:
	static constexpr char_type null_char = '\0';
	static constexpr size_type 
		max_short_len = sizeof(ltc::Vector<char_type, Alloc>) /
					sizeof(char_type) - 1;
	void move_to_long();
	void move_to_short();
	
	bool using_long() const;

	size_type _len {0};
	union {
		char_type _short[max_short_len + 1]{null_char};
		ltc::Vector<char_type,Alloc> _long;
	};
	detail::basic_string _str;
};//class String

}//namespace ltc

template<class C, class A>
ltc::String<C,A>::String(size_type n, char_type c)
	: _len{n}
{
	if (_len > max_short_len) {
		_long.Vector(n,c);
	}
	else {
		for (size_type i {0}; i < max_short_len; ++i) {
			_short[i] = c;
		}
		_short[max_short_len] = null_char;
	}
}

template<class C, class A>
ltc::String<C,A>::String(const String<C, A>& str)
{
	reserve(str.size());
	for (auto x : str) {
		push_back(x);
	}
}

template<class C, class A>
ltc::String<C,A>::String(String<C, A>&& str)
	: _len{str._len}
{
	if (_len > max_short_len) {
		_long.Vector(ltc::move(str._long));
	}
	else {
		for (size_type i {0}; i < max_short_len; ++i) {
			_short[i] = str._short[i];
		}
		_short[max_short_len] = null_char;
	}
}

template<class C, class A>
ltc::String<C,A>::String(const char_type* cstr)
{
	size_type sz {0};
	for (const char_type* cp{cstr}; *cp; ++cp) {
		if (sz > max_short_len)
			break;
		++sz;
	}
	if (sz > max_short_len) {
		new(&_long) Vector();
		_long.reserve(max_short_len + 1);
		for (const char_type* cp{cstr}; *cp; ++cp) {
			_long.push_back(*cp);
		}	
	}
	else {
		size_type i {0};
		for (const char_type* cp{cstr}; *cp; ++cp) {
			_short[i++] = *cp;
		}	
		_short[i] = null_char;
	}
}

template<class C, class A>
ltc::String<C,A>::String(const char_type* cstr, const size_type len)
{
	if (len > max_short_len) {
		_long.Vector();
		_long.reserve(max_short_len + 1);
		for (const char_type* cp{cstr}; *cp; ++cp) {
			_long.push_back(*cp);
		}	
	}
	else {
		size_type i {0};
		for (const char_type* cp{cstr}; *cp; ++cp) {
			_short[i++] = *cp;
		}	
		_short[i] = null_char;
	}
}

template<class C, class A>
ltc::String<C,A>::~String()
{
	if (using_long()) {
		_long.~Vector();
	}
}

template<class C, class A>
ltc::String<C,A>& 
	ltc::String<C,A>::operator=(const String<C,A>& str)
{
	String<C,A> s{str};
	swap(s);
	return *this;
}

template<class C, class A>
ltc::String<C,A>& ltc::String<C,A>::operator=(String<C, A>&& str)
{
	String<C,A> s{ltc::move(str)};
	swap(s);
	return *this;
}

template<class C, class A>
typename ltc::String<C,A>::reference
	ltc::String<C,A>::operator[](const size_type n)
{
	if (n >= size()) throw ltc::out_of_range{"String subscript operator "
						 "is out of range."};
	return using_long() ? _long[n] : _short[n];
}
template<class C, class A>
typename ltc::String<C,A>::const_reference 
	ltc::String<C,A>::operator[](const size_type n) const
{
	if (n >= size()) throw ltc::out_of_range{"String subscript operator "
						 "is out of range."};
	return using_long() ? _long[n] : _short[n];
}

template<class C, class A>
typename ltc::String<C,A>::reference ltc::String<C,A>::front()
{
	if (empty()) throw ltc::out_of_range{"String::front() called on an "
						"empty string."};
	return using_long() ? _long[0] : _short[0];
}
template<class C, class A>
typename ltc::String<C,A>::const_reference ltc::String<C,A>::front() const
{
	if (empty()) throw ltc::out_of_range{"String::front() called on an "
						"empty string."};
	return using_long() ? _long[0] : _short[0];
}
template<class C, class A>
typename ltc::String<C,A>::reference ltc::String<C,A>::back()
{
	if (empty()) throw ltc::out_of_range{"String::back() called on an "
						"empty string."};
	return using_long() ? _long[_len] : _short[_len];
}
template<class C, class A>
typename ltc::String<C,A>::const_reference ltc::String<C,A>::back() const
{
	if (empty()) throw ltc::out_of_range{"String::back() called on an "
						"empty string."};
	return using_long() ? _long[_len] : _short[_len];
}

template<class C, class A>
inline const typename ltc::String<C,A>::char_type* 
	ltc::String<C,A>::c_str() const
{
	return using_long() ? &_long.front() : _short;
}

template<class C, class A>
inline typename ltc::String<C,A>::iterator ltc::String<C,A>::begin()
{
	return using_long() ? _long.begin() : _short;
}

template<class C, class A>
inline typename ltc::String<C,A>::iterator ltc::String<C,A>::end()
{
	return using_long() ? _long.end() : _short + _len;
}

template<class C, class A>
inline bool ltc::String<C,A>::empty()
{
	return size() == 0;
}

template<class C, class A>
inline typename ltc::String<C,A>::size_type ltc::String<C,A>::size()
{
	return _len;
}

template<class C, class A>
inline typename ltc::String<C,A>::size_type ltc::String<C,A>::length()
{
	return size();
}

template<class C, class A>
inline typename ltc::String<C,A>::size_type ltc::String<C,A>::capacity()
{
	return using_long() ? _long.capacity() : max_short_len;
}

template<class C, class A>
void ltc::String<C,A>::reserve(const size_type n)
{
	if (n <= capacity()) return;
	if (using_long()) {
		_long.reserve(n);
	}
	else  {
		if (n > max_short_len) {
			move_to_long();
			_long.reserve(n);
		}
	}
}

template<class C, class A>
void ltc::String<C,A>::clear()
{
	if (using_long()) {
		_long.~Vector();
	}
	_len = 0;
}

template<class C, class A>
void ltc::String<C,A>::swap(String<C,A>& str)
{
	if (using_long()) {
		_long.swap(str._long);
	}
	else {
		char_type tmp_array[max_short_len + 1];
		for (size_type i{0}; i < max_short_len; ++i) {
			tmp_array[i] = _short[i];
		}
		for (size_type i{0}; i < max_short_len; ++i) {
			_short[i] = str._short[i];
		}
		for (size_type i{0}; i < max_short_len; ++i) {
			str._short[i] = tmp_array[i];
		}
	}
	size_type tmp {_len};
	_len = str._len;
	str._len = tmp;
}

template<class C, class A>
typename ltc::String<C,A>::iterator 
	ltc::String<C,A>::insert(const iterator i, const char_type t)
{	
}

/*
template<class C, class A>
erase
*/

template<class C, class A>
void ltc::String<C,A>::push_back(const char_type c)
{
	if (_len = max_short_len) {
		move_to_long();
		_long.back() = c;
		_long.push_back(null_char);
	}
	else {
		_short[_len++] = c;
		_short[_len] = null_char;
	}
}

template<class C, class A>
void ltc::String<C,A>::pop_back()
{
	//TODO: handle case when have to move to short
	if (empty()) throw ltc::out_of_range{"String::pop_back() called "
						"on an empty string."};
	if (using_long()) {
		_long.pop_back();
		if (_len - 1 == max_short_len) move_to_short();
	}
	else {
		_short[_len - 1] = null_char;
	}
	--_len;
		
}

template<class C, class A>
void ltc::String<C,A>::move_to_long()
{
	ltc::Vector<C,A> vec;
	vec.reserve(_len + 1);//+1 for null_char
	for (size_type i{0}; i < _len; ++i) {
		vec.push_back(_short[i]);
	}	
	vec[_len + 1] = null_char;
	_long.Vector(ltc::move(vec));
}

template<class C, class A>
void ltc::String<C,A>::move_to_short()
{
	char_type tmp[max_short_len + 1];
	for (size_type i{0}; i < max_short_len + 1; ++i) {
		tmp[i] = null_char;
	}
	for (size_type i{0}; i < _long.size(); ++i) {
		tmp[i] = _long[i];
	}
	_long.~Vector();
	for (size_type i{0}; tmp[i]; ++i) {
		_short[i] = tmp[i];
	}
}

template<class C, class A>
inline bool ltc::String<C,A>::using_long() const
{
	return _len > max_short_len;
}

#endif
