#ifndef GAURD_BIG_INT_TEST_H
#define GUARD_BIG_INT_TEST_H

#include <iostream>
#include <fstream>
#include <vector>

#include "big_int.h"
#include "big_int_number_theory.h"



template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
	/*
	for (unsigned i {0}; i < sizeof(std::vector<T>::size_type); ++i) {
		char c {0};
		for (unsigned j {0}; j < 8; ++j) {
			c |= (v.size() & (0x1 << (j + i)));
		}
		os << c;
	}
	for (const auto& x : v) {
		for (unsigned i {0}; i < sizeof(T); ++i) {
			char c {0};
			for (unsigned j {0}; j < 8; ++j) {
				c |= (x & (0x1 << (j + i)));
			}
			os << c;
		}
	}
	return os;
	*/
	std::vector<T>::size_type sz{v.size()};
	char* cp {reinterpret_cast<char*>(&sz)};
	for (unsigned i {0}; i < sizeof(std::vector<T>::size_type); ++i) {
		os << cp[i];
	}
	cp = reinterpret_cast<char*>(&v.begin());
	for (unsigned i {0}; i < v.size(); ++i) {
		for (unsigned j {0}; j < sizeof(T); ++j) {
			os << cp[i + j];
		}
	}
	return os;
}

template<class T>
std::istream& operator >> (std::istream& is, std::vector<T>& v) {
	/*
	std::vector<T>::size_type sz {0};
	for (unsigned i {0}; i < sizeof(std::vector<T>::size_type); ++i) {
		char c {0};
		if (!(is >> c)) return is;
		for (unsigned j {0}; j < 8; ++j) {
			sz |= (c & (0x1 << (j + i)));
		}
	}
	std::vector<T> vect(sz);
	for (std::vector<T>::size_type i {0}; i < vect.size(); ++i) {
		T n {0};
		for (unsigned j {0}; j < sizeof(T); ++j) {
			char c {0};
			if (!(is >> c)) return is;
			n |= (c << (j * 8));
		}
		vect[i] = n;
	}
	v.swap(vect);
	return is;
	*/
	char ar[sizeof(std::vector<T>::size_type)];
	char* cp = ar;
	for (unsigned i {0}; i < sizeof(std::vector<T>::size_type); ++i) {
		if (!is.get(cp[i])) return is;
	}
	std::vector<T> vec(*reinterpret_cast<std::vector<T>::size_type*>(cp));
	for (unsigned i {0}; i < vec.size(); ++i) {
		char n[sizeof(T)];
		/*
			for (unsigned j {0}; j < sizeof(T); ++j) {
				if(!(is >> n[j])) return is;
			}
			*/
		if (!is.get(n, sizeof(T))) return is;
		vec[i] = *reinterpret_cast<T*>(n);
	}
	v.swap(vec);
	return is;
}

void write_to_file_test() {
	std::vector<unsigned long long> v {1, 2 ,3 ,4, 5, 6, 7, 8, 9, 0};
	std::cout << v << '\n';
	std::ofstream os("test.txt", std::ios::binary);
	if (os << v)//<< ' ')
		std::cout << "wrote to file\n";
	else
		std::cout << "couldn't write to file\n";
	std::ifstream is("test.txt", std::ios::binary);
	std::vector<unsigned long long> vect;
	if (is >> vect)
		std::cout << "got the num\n";
	else
		std::cout << "couln't get the num\n";
	std::cout << vect << '\n';
}

Big_int generate_prime(Big_int::size_type sz) {
	Big_int num {get_rand_big_int(sz)};
	auto i {0ull};
	num.set_bit(0, 1);
again:
	while (!miller_rabin_prime_test(num, 1)) {
		std::cout << "another one failed" << i << '\n';
		num += 2;
		++i;
	}
	if (miller_rabin_prime_test(num, 10)) {
		std::cout << "found one\n";
		return num;
	}
	else {
		goto again;
	}
}

Big_int RSA_tester(const Big_int& M, const unsigned b_size) {
	Big_int prime1 {generate_prime(b_size)};
	Big_int prime2 {generate_prime(b_size)};
	Big_int n {prime1*prime2};
	Big_int phi {(prime1 - 1)*(prime2 - 1)};
	Big_int e {1997};
	Big_int d {};
	modular_equation(e, 1, phi, d);
	RSA_key pub{e, n};
	RSA_key priv {d, n};
	Big_int ciph {RSA(pub, M)};
	Big_int de_ciph {RSA(priv, ciph)};
	
	std::cout << "Original message:    " << M.to_ull() << '\n';
	std::cout << "Deciphered message:  " << RSA(priv, ciph).to_ull() << '\n';

	return de_ciph;
}
void randomized_left_shift_testing(unsigned rep = 1000000) {
	for (Big_int::size_type i = 0; i < rep; ++i) {
		Big_int t {get_rand_big_int(43)};
		Big_int u {get_rand_big_int(5, false)};
		unsigned long long t_int {t.to_ull()};
		unsigned long long u_int {u.to_ull()};
		if ((t << u_int) != (t_int << u_int)) {
			std::cout << "FOUND ONE!\n";
			std::ofstream os("error.txt", std::ios::app);
			os << t_int << " << " << u_int <<
				" != " << (t_int << u_int) << '\n';
			os.close();
		}
	}
}
void randomized_right_shift_testing(unsigned rep = 1000000) {
	for (Big_int::size_type i = 0; i < rep; ++i) {
		Big_int t {get_rand_big_int(64, false)};
		Big_int u {get_rand_big_int(5, false)};
		unsigned long long t_int {t.to_ull()};
		unsigned long long u_int {u.to_ull()};
		if ((t >> u_int) != (t_int >> u_int)) {
			std::cout << "FOUND ONE!\n";
			std::ofstream os("error.txt", std::ios::app);
			os << t_int << " >> " << u_int <<
				" != " << (t_int >> u_int) << '\n';
			os.close();
		}
	}
}
void randomized_division_testing(unsigned rep = 1000000) {
	for (Big_int::size_type i = 0; i < rep; ++i) {
		Big_int t {get_rand_big_int(64, false)};
		Big_int u {get_rand_big_int(58, false)};
		unsigned long long t_int {t.to_ull()};
		unsigned long long u_int {u.to_ull()};
		if (u_int == 0) continue;
		if ((t / u) != (t_int / u_int)) {
			std::cout << "FOUND ONE!\n";
			std::ofstream os("error.txt", std::ios::app);
			os << t_int << " / " << u_int <<
				" != " << t_int / u_int << '\n';
			os.close();
		}
		if ((t % u) != (t_int % u_int)) {
			std::cout << "FOUND ONE!\n";
			std::ofstream os("error.txt", std::ios::app);
			os << t_int << " % " << u_int <<
				" != " << t_int % u_int << '\n';
			os.close();
		}
	}
}
void randomized_multiplication_testing(unsigned rep = 1000000) {
	for (Big_int::size_type i = 0; i < rep; ++i) {
		Big_int t {get_rand_big_int(16,false)};
		Big_int u {get_rand_big_int(16,false)};
		unsigned long long t_int {t.to_ull()};
		unsigned long long u_int {u.to_ull()};
		if ((t * u) != (t_int * u_int)) {
			std::cout << "FOUND ONE!\n";
			std::ofstream os("error.txt", std::ios::app);
			os << t_int << " * " << u_int <<
				" != " << t_int * u_int << '\n';
			os.close();
		}
	}
}
void randomized_addition_testing(unsigned rep = 1000000) {
	for (Big_int::size_type i = 0; i < rep; ++i) {
		Big_int t {get_rand_big_int(63,false)};
		Big_int u {get_rand_big_int(63,false)};
		unsigned long long t_int {t.to_ull()};
		unsigned long long u_int {u.to_ull()};
		if ((t + u).to_ull() != (t_int + u_int)) {
			std::cout << "FOUND ONE!\n";
			std::ofstream os("error.txt", std::ios::app);
			os << t_int << " + " << u_int <<
				" != " << t_int + u_int << '\n';
			os.close();
		}
	}
}
void randomized_substraction_testing(unsigned rep = 1000000) {
	for (Big_int::size_type i = 0; i < rep; ++i) {
		Big_int t {get_rand_big_int(63,true)};
		Big_int u {get_rand_big_int(62,false)};
		unsigned long long t_int {t.to_ull()};
		unsigned long long u_int {u.to_ull()};
		if ((t - u) != (t_int - u_int)) {
			std::cout << "FOUND ONE!\n";
			std::ofstream os("error.txt", std::ios::app);
			os << t_int << " - " << u_int <<
				" != " << t_int - u_int << '\n';
			os.close();
		}
	}
}

#endif