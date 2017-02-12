#ifndef GUARD_LTC_UTILITY_H
#define GUARD_LTC_UTILITY_H

namespace ltc {

template<class T>
T&& move(T& t);

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

#endif
