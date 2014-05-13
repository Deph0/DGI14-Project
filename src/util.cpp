#include "util.h"
#include <sstream>
#include <stdio.h>


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

} // namespace Util

