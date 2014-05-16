#include "shader.h"
#include "exception.h"
#include "util.h"
#include <fstream>


Shader::Shader(GLenum type)
{
	handle = glCreateShaderObjectARB(type);
	if (handle == 0)
		throw Exception(util::format("failed to create shader %d", type));
}


Shader::~Shader()
{
	glDeleteObjectARB(handle);
}


void Shader::load(const std::string& fname)
{
	std::ifstream file;
	std::string code;

	printf("loading shader %s\n", fname.c_str());

	file.open(fname.c_str(), std::ios::in);
	if (!file) {
		throw Exception("failed to open " + fname);
	}

	file.seekg(0, std::ios::end);
	code.resize(file.tellg());
	file.seekg(0, std::ios::beg);

	file.read(&code[0], code.size());
	file.close();

	const char* str = code.c_str();
	glShaderSourceARB(handle, 1, &str, NULL);
	compile();
}


void Shader::compile()
{
	GLint status;

	glCompileShaderARB(handle);
	glGetObjectParameterivARB(handle, GL_OBJECT_COMPILE_STATUS_ARB, &status);

	if (status == GL_FALSE) {
		std::string errors;
		GLint len;
		// Get size of info log
		glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &len);
		if (len > 1) {
			errors.resize(len);
			glGetInfoLogARB(handle, len, NULL, &errors[0]);
		}
		throw Exception("shader compile failed\n" + errors);
	}
}

