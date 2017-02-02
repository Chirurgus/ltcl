#ifndef GUARD_LTC_UTILITY_H
#define GUARD_LTC_UTILITY_H

namespace ltc
{
	template<class T>
	T&& move(T& t);
}//namespace ltc

template<class T>
T&& ltc::move(T& t)
{
	//probably a really naive implementation
	return static_cast<T&&>(t);
}

#endif
