#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

namespace util {

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


void tokenize(const std::string& str, char delim, std::vector<std::string>* tokens);

float strToFloat(const std::string& str);
int strToInt(const std::string& str);

std::string unescape(const std::string& str);

std::string format(const char* fmt, ...);

float in_range(float low, float high, float resolution);

// Append default resource path in the beginning of the string
std::string resource_path(const std::string& str);
// Append default shader path in the beginning of the string
std::string shader_path(const std::string& str);

} // namespace util

#endif // UTIL_H

