#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

class Exception : public std::exception {
public:
	Exception(const std::string& m)
	: msg(m)
	{ }
	virtual ~Exception() throw() { }
	virtual const char* what() const throw() {
		return msg.c_str();
	}
private:
	std::string msg;
};

#endif // EXCEPTION_H

