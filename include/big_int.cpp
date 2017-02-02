#include "big_int.h"

#include <limits>
#include <algorithm>
#include <utility>
#include <limits>

#include <time.h>

#ifndef NDEBUG
#include <iostream>
#endif // !NDEBUG


// Helper functions

// !Helper functions
/*
Big_int::Big_int(int i):Big_int(i) {}

Big_int::Big_int(unsigned long long l)
	:num((sizeof(unsigned long long)*char_bit_size)/word_type_size, 0) {
	unsigned long long mask {0x1};
	for (int i = 0; i < sizeof(unsigned long long)*char_bit_size; ++i) {
		if (mask & l) {
			set_bit(i, 1);
		}
		mask <<= 1;
	}
}
*/

Big_int& Big_int::operator+=(const Big_int& v) {
	size_type v_sz {v.num.size()};
	while (v_sz && v.num[v_sz - 1] == 0) { --v_sz; }
	size_type u_sz {num.size()};
	while (u_sz && num[u_sz - 1] == 0) { --u_sz; }
	
	num.resize((u_sz < v_sz ? v_sz : u_sz), 0);
	u_sz = num.size();
	size_type i {0};
	bool k {0};
	for (;i < v_sz; ++i) {
		long_word_type tmp {long_word_type(num[i]) + v.num[i] + k};
		k = tmp >= base;
		num[i] = tmp;
	}
	for (; i < u_sz; ++i) {
		long_word_type tmp {num[i] + k};
		k = tmp >= base;
		num[i] = tmp;
	}
	if (k) {
		num.push_back(k);
	}
	return *this;
	/*
	std::vector<word_type> vec {};
	bool carry_bit {false};

	auto rhs_sz = rhs.num.size();
	auto lhs_sz = num.size();
	vec.reserve(rhs_sz > lhs_sz ? rhs_sz : lhs_sz);

	size_type i {0};
	for (; i < rhs_sz && i < lhs_sz; ++i) {
		vec.push_back(adder(rhs.num[i], num[i], carry_bit, &carry_bit));
	}
	for (; i < rhs_sz; ++i) {
		vec.push_back(adder(rhs.num[i], 0, carry_bit, &carry_bit));
	}
	for (; i < lhs_sz; ++i) {
		vec.push_back(adder(num[i], 0, carry_bit, &carry_bit));
	}
	if (carry_bit) {
		vec.push_back(1);
	}
	Big_int ret {};
	ret.num.swap(vec);
	return ret;
	*/
}

Big_int& Big_int::operator-=(const Big_int& v) {
	size_type v_sz {v.num.size()};
	while (v_sz && v.num[v_sz - 1] == 0) { --v_sz; }
	size_type u_sz {num.size()};
	while (u_sz && num[u_sz - 1] == 0) { --u_sz; }
	if (u_sz == 0) ++u_sz;
	num.resize(u_sz, 0);
	bool k {0};
	size_type i {0};
	for (; i < v_sz && i < u_sz; ++i) {
		long_word_type sub {long_word_type(v.num[i]) + k};
		k = num[i] < sub;
		num[i] = num[i] - sub;
	}
	for (; i < u_sz; ++i) {
		long_word_type sub {k};
		k = num[i] < sub;
		num[i] = num[i] - sub;
	}
	return *this;
	/*
	Big_int ret {0};
	bool carry_bit {0};
	size_type i_max = 
		char_bit_size *
		(rhs.num.size() < num.size() ? num.size() : rhs.num.size());
	for (size_type i = 0; i < i_max; ++i) {
		if (carry_bit) {
			if ((*this).inspect_bit(i)) {
				if (rhs.inspect_bit(i)) {
					ret.set_bit(i, 1);
					carry_bit = 1;
				}
				else {
					ret.set_bit(i, 0);//redundant
					carry_bit = 0;
				}
			}
			else {
				if (rhs.inspect_bit(i)) {
					ret.set_bit(i, 0);//redudndant
					carry_bit = 1;
				}
				else {
					ret.set_bit(i, 1);
					carry_bit = 1;
				}
			}
		}
		else {
			if ((*this).inspect_bit(i)) {
				if (rhs.inspect_bit(i)) {
					ret.set_bit(i, 0);//redudant
					carry_bit = 0;
				}
				else {
					ret.set_bit(i, 1);
					carry_bit = 0;
				}
			}
			else {
				if (rhs.inspect_bit(i)) {
					ret.set_bit(i, 1);
					carry_bit = 1;
				}
				else {
					ret.set_bit(i, 0);//redudnant
					carry_bit = 0;
				}
			}
		}
	}
	//if carry_bit == true, the resulting number is negative
	return ret;
	*/
}

Big_int& Big_int::operator*=(const Big_int & rhs) {
	size_type u_sz {num.size()};
	while (u_sz && num[u_sz - 1] == 0)  --u_sz;
	size_type v_sz {rhs.num.size()};
	while (v_sz && rhs.num[v_sz - 1] == 0) --v_sz;
	
	Big_int ret;
	ret.num.resize(u_sz + v_sz, 0);

	
	for (size_type i = 0; i < u_sz; ++i) {
		long_word_type k {0};
		for (size_type j = 0; j < v_sz; ++j) {
			long_word_type tmp {long_word_type(num[i]) * rhs.num[j] + ret.num[i+j] + k};
			k = tmp >> word_type_size;
			ret.num[i + j] = tmp;
				//(tmp << word_type_size) >> word_type_size;
		}
		ret.num[i + v_sz] = k;
	}
	return *this = ret;
}

Big_int Big_int::operator/(Big_int rhs) const {
	return division_with_rest(*this, rhs).first;
}

Big_int Big_int::operator%(const Big_int & rhs) const {
	return division_with_rest(*this, rhs).second;
}

bool Big_int::operator<(const Big_int& rhs) const {
	size_type rhs_sz {rhs.num.size() - 1};
	size_type lhs_sz {num.size() - 1};
	while (rhs_sz < lhs_sz) {
		if (num[lhs_sz])
			return false;
		--lhs_sz;
	}
	while (lhs_sz < rhs_sz) {
		if (rhs.num[rhs_sz])
			return true;
		--rhs_sz;
	}
	//now lhs_sz == rhs_sz
	for (size_type i = lhs_sz + 1; i-- > 0;) {
		if (num[i] < rhs.num[i]) {
			return true;
		}
		if (num[i] > rhs.num[i]) {
			return false;
		}
	}
	//if we got here the numbers are equal
	return false;	
	//Old version
	/*
	size_type max_sz = 
		char_bit_size *
		(rhs.num.size() < num.size() ? num.size() : rhs.num.size());
	for (size_type i = max_sz; i-- > 0;) {
		if (rhs.inspect_bit(i) && !(*this).inspect_bit(i)) {
			return true;
		}
		if ((*this).inspect_bit(i) && !rhs.inspect_bit(i)) {
			return false;
		}
	}
	return false;
	*/
}

bool Big_int::operator==(const Big_int & rhs) const {
	size_type lhs_sz {num.size() - 1};
	size_type rhs_sz {rhs.num.size() - 1};
	while (lhs_sz && num[lhs_sz] == 0) --lhs_sz;
	while (rhs_sz && rhs.num[rhs_sz] == 0) --rhs_sz;
	if (rhs_sz != lhs_sz)
		return false;
	for (size_type i = rhs_sz + 1; i-- > 0;) {
		if (num[i] != rhs.num[i])
			return false;
	}
	return true;
	//Old (wierdly complicated) lazy version
	/*
	size_type max_sz = 
		char_bit_size *
		(rhs.num.size() < num.size() ? num.size() : rhs.num.size());
	for (size_type i = 0; i < max_sz; ++i) {
		if ((*this).inspect_bit(i) != rhs.inspect_bit(i)) {
			return false;
		}
	}
	return true;
	*/
}

unsigned long long Big_int::to_ull() const {
	/*
	unsigned long long ret {0};
	unsigned long long mask {0x1};
	for (size_type i = 0, i_max = sizeof(unsigned long long) * char_bit_size; i < i_max; ++i) {
		if (inspect_bit(i)) {
			ret |= mask;
		}
		mask <<= 1;
	}
	return ret;
	*/
	unsigned long long ret {0};
	ret |= num[0];
	if (num.size() >= 2) {
		unsigned long long tmp = unsigned long long (num[1]) << 32;
		ret |= tmp;
		
	}
	return ret;
}

Big_int::operator bool() const {
	return *this != 0;
}

bool Big_int::inspect_bit(const size_type sz) const {
		const size_type i = sz / word_type_size;
		if (i >= num.size())
			return 0;
		return static_cast<bool>(num[i] &
								 long_word_type(0x1) << sz % word_type_size);
	}

void Big_int::set_bit(const size_type sz, const bool b) {
	const size_type i = sz / word_type_size;
	long long diff = (i + 1) - static_cast<long long>(num.size());
	if (diff > 0) {
		for (size_type j = 0; j < diff; ++j) {
			num.push_back(0);
		}
	}
	word_type tmp = num[i];
	tmp ^= (-b ^ tmp) & (0x1 << sz % word_type_size);
	num[i] = tmp;
}

Big_int& Big_int::operator>>= (const size_type rhs){
	size_type shift = rhs / word_type_size;
	size_type in_word_shift = rhs % word_type_size;
	if (num.size() <= shift) {
		return *this = Big_int {0};
	}
	for (size_type i = 0; i < num.size() - shift - 1; ++i) {
		num[i] = (num[i + shift] >> in_word_shift) |
			(long_word_type(num[i + shift + 1]) << 
			(word_type_size - in_word_shift));
	}
	num[num.size() - shift - 1] =
		num[num.size() - 1] >> in_word_shift;
	for (size_type i = num.size() - shift;
		 i < num.size();
		 ++i) {
		num[i] = 0;
	}
	return *this;
}

Big_int& Big_int::operator<<=(size_type rhs) {
	//Possible optimization:
	//		check if in_word_shift is nul, 
	//		if so all the bit shift ops are 
	//		not needed
	size_type shift = rhs / word_type_size;
	size_type in_word_shift = rhs % word_type_size;
	if (in_word_shift > 0) 
		num.resize(num.size() + shift + 1, 0);
	else 
		num.resize(num.size() + shift, 0);
	for (size_type i = num.size() - 1; i >= shift + 1; --i) {
			word_type tmp = (num[i - shift] << in_word_shift) |
			(long_word_type(num[i - shift - 1]) >> (word_type_size - in_word_shift));
			num[i] = tmp;
	}
	num[shift] = num[0] << in_word_shift;
	for (size_type i = 0; i < shift; ++i) {
		num[i] = 0;
	}
	return *this;
}

Big_int get_rand_big_int(const Big_int::size_type bit_size,
						 const bool strict) {
	Big_int ret {0};
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::bernoulli_distribution dist(0.5);
	for (Big_int::size_type i = bit_size - 2;
	   i-- > 0;) {
	  ret.set_bit(i, dist(gen));
		}
	  if (strict) {
		  ret.set_bit(bit_size - 1, 1);
	  }
	return ret;
}

std::pair<Big_int, Big_int> Big_int::division_with_rest(const Big_int& rhs,
														const Big_int& lhs) const {
	Big_int u = rhs;
	Big_int v = lhs;

	size_type v_sz {v.num.size()};
	while (v_sz && v.num[v_sz - 1] == 0) { --v_sz; }
	size_type u_sz {u.num.size()};
	while (u_sz && u.num[u_sz - 1] == 0) { --u_sz; }

	//Handle the case where u and v are same sizes
	if (v_sz == 1 || v == 0) {
		if (v == 0)
			throw Bad_int {};
		//Handle case where v is single radix-base digit
		Big_int q {0};
		q.num.resize(u_sz, 0);
		word_type tmp {0};
		for (size_type i = u_sz; i-- > 0;) {
			q.num[i] = (tmp*base + u.num[i]) / v.num[0];
			tmp = (tmp*base + u.num[i]) - (q.num[i] * v.num[0]);
		}
		return {q, tmp};
	}
	/*
	if (u_sz == v_sz) {
		long_word_type qhat = long_word_type(u.num[u_sz - 1]) / v.num[v_sz - 1];
		Big_int tmp = v * qhat;
		while (tmp > u) {
			--qhat;
			tmp -= v;
		}
		return {qhat, u - tmp};
	}
	*/

	if (u_sz < v_sz) {
		return {0, u};
	}

	//Normalize
	//count number of leading 0s in v[v_sz]
	size_type shift {0};
	for (word_type i = v.num[v_sz - 1]; i; i <<= 1) {
		if (i & (0x1 << word_type_size - 1))
			break;
		++shift;
	}
	//nomalize
	v <<= shift;
	u <<= shift;
	//u_sz += 1;
	u.num.resize(u_sz+1,0);
	/*
	if (u_sz == v_sz) {
		long_word_type qhat = long_word_type(u.num[u_sz]) / v.num[v_sz - 1];
		Big_int tmp = v * qhat;
		while (tmp > u) {
			--qhat;
			tmp -= v;
		}
		return {qhat, (u - tmp) >> shift};
	}
	*/

	//place to put quotient
	Big_int q {0};
	q.num.resize(u_sz - v_sz + 1, 0);//TODO::check the size

		//main loop
	for (size_type j = q.num.size(); j-- > 0;) {//TODO:check the # of iteratations
		//Calculate qhat
		//TODO: check indexes in
		//	u.num[j + v_sz] << IT used to be this
		long_word_type qhat = (u.num[j + v_sz] * base +
							   u.num[j + v_sz - 1]) / v.num[v_sz - 1];
		long_word_type rhat = (u.num[j + v_sz] * base +
							   u.num[j + v_sz - 1]) - qhat * v.num[v_sz - 1];
			   /*
			   long_word_type rhat = (u.num[j + v_sz + 1] * base +
							   u.num[j + v_sz]) % v.num[v_sz];
							   */

		for (;;) {
			if (qhat >= base ||
				(qhat*v.num[v_sz - 2]) >
				(base*rhat + u.num[j + v_sz - 2])) {
				--qhat;
				rhat += v.num[v_sz - 1];
				if (rhat < base) {
					continue;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}

		//I'm pretty sure that for additions
		//	and substractions below implicit
		//	over, and under flow would have
		//	worked just fine

		//substract from u;
		bool u_k {false};//substraction may only underflowr by 1
		long_word_type v_k {0};
		for (size_type i = 0; i < v_sz; ++i) {
			long_word_type v_tmp {qhat * long_word_type(v.num[i]) + v_k};
			v_k = v_tmp >> word_type_size;
			word_type v_i =
				(v_tmp << word_type_size) >> word_type_size;
			long_word_type u_tmp {long_word_type(u.num[i + j]) - v_i - u_k};
			u_k = u_tmp >= base;
			word_type u_i =
				(u_tmp << word_type_size) >> word_type_size;
			u.num[i + j] = u_i;
		}
		//TODO: check indices
		v_k = u.num[j + v_sz] - v_k - u_k;
		u.num[j + v_sz] = v_k;//let it underflow
			//(v_k << word_type_size) >> word_type_size;

		//check if we substracted too much;
		if (v_k >= base) {
			--qhat;
			u_k = false;
			for (size_type i = 0; i < v_sz; ++i) {
				long_word_type u_tmp {long_word_type(u.num[i + j]) + v.num[i] + u_k};
				u.num[i + j] = u_tmp;
					//(u_tmp << word_type_size) >> word_type_size;
				u_k = u_tmp >= base;
			}
			u.num[j + v_sz] += u_k;
		}
		q.num[j] = qhat;
	}
	return {q, (u >> shift)};
}

std::ostream & operator<<(std::ostream & os, const Big_int & n) {
	if (os << n.num.size()) {
		for (const Big_int::word_type x : n.num) {
			if (!(os << x)) break;
		}
	}
	return os;
}

std::istream & operator>> (std::istream & is, Big_int & n) {
	n.num.clear();
	Big_int::size_type sz {};
	if (is >> sz) {
		n.num.reserve(sz);
		for (Big_int::word_type x {}; is >> x;) {
			n.num.push_back(x);
		}
	}
	
	return is;
}
