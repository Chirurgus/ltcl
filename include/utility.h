#ifndef GUARD_LTC_UTILITY_H
#define GUARD_LTC_UTILITY_H

namespace ltc {

template<class T>
T&& move(T& t);

template<class T>
void swap(T& t1, T& t2);

template<class F, class S>
struct Pair {
	using first_type = F;
	using second_type = S;
	F first;
	S second;
};//struct Pair

}//namespace ltc

template<class T>
T&& ltc::move(T& t)
{
	//probably a really naive implementation
	return static_cast<T&&>(t);
}

template<class T>
void swap(T& t1, T& t2)
{
	T tmp {ltc::move(t1)};
	t1 = ltc::move(t2);
	t2 = ltc::move(tmp);
}

#endif
