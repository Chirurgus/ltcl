#ifndef GUARD_LTCL_EXCEPTIONS_H
#define GUARD_LTCL_EXCEPTIONS_H

namespace ltc {

class exception {
public:
	using string = const char*;

	virtual string what() const = 0;
	
	virtual ~exception() {};
};// class excpetion

class out_of_range : public exception {
public:
	out_of_range(const string& err = "Out of range error.") : _err{err} {}
	string what() const override { return _err; }
	
	~out_of_range() override {};
private:
	string _err;
};// class out_of_range

}// namespace ltc

#endif
