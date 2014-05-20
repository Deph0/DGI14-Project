#include "util.h"
#include <sstream>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>


namespace util {


void tokenize(const std::string& str, char delim, std::vector<std::string>* tokens)
{
	std::istringstream is(str);
	std::string token;

	tokens->clear();
	while (getline(is, token, delim)) {
		tokens->push_back(token);
	}
}


float strToFloat(const std::string& str)
{
	float f;
	if (sscanf(str.c_str(), "%f", &f) != 1)
		throw util::Exception("failed to convert [" + str + "] to float");
	return f;
}


int strToInt(const std::string& str)
{
	int i;
	if (sscanf(str.c_str(), "%d", &i) != 1)
		throw util::Exception("failed to convert [" + str + "] to integer");
	return i;
}


std::string unescape(const std::string& str)
{
	std::string out;
	const char* ptr = str.c_str();
	int ch;

	for (; *ptr; ptr++) {
		if (*ptr == '%') {
			sscanf(ptr + 1, "%2x", &ch);
			ptr += 2;
		}
		else ch = *ptr;
		out.append(1, ch);
	}

	return out;
}


std::string format(const char* fmt, ...)
{
	va_list ap;
	std::string out(512, 0);

	va_start(ap, fmt);
	vsnprintf(&out[0], out.size(), fmt, ap);
	va_end(ap);

	return out;
}


float in_range(float low, float high, float resolution)
{
	return (rand() % (int((high - low) * resolution) + 1)) / resolution + low;
}


std::string resource_path(const std::string& str)
{
	return "../res/" + str;
}


std::string shader_path(const std::string& str)
{
	return "../shaders/" + str;
}

} // namespace Util

