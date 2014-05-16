#include "shader_program.h"
#include "exception.h"


ShaderProgram::ShaderProgram()
: handle(0)
{
}


ShaderProgram::~ShaderProgram()
{
	Uniform::Iter ui = uniforms.begin();
	for (; ui != uniforms.end(); ++ui) {
		delete ui->second;
	}
	Shader::Iter si = shaders.begin();
	for (; si != shaders.end(); ++si) {
		glDetachObjectARB(handle, (*si)->getHandle());
		delete *si;
	}
	glDeleteObjectARB(handle);
}


Shader* ShaderProgram::createShader(GLenum type)
{
	Shader* s = new Shader(type);
	shaders.push_back(s);
	return s;
}


void ShaderProgram::initialize()
{
	GLint status;

	handle = glCreateProgramObjectARB();
	if (handle == 0)
		throw Exception("failed to create program");
	// Add all shaders
	Shader::Iter i = shaders.begin();
	for (; i != shaders.end(); ++i) {
		glAttachObjectARB(handle, (*i)->getHandle());
	}
	// Link
	glLinkProgramARB(handle);
	glGetObjectParameterivARB(handle, GL_OBJECT_LINK_STATUS_ARB, &status);
	if (status != GL_TRUE) {
		std::string errors;
		GLint len;
		// Get size of info log
		glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &len);
		if (len > 1) {
			errors.resize(len);
			glGetInfoLogARB(handle, len, NULL, &errors[0]);
		}
		throw Exception("shader program link failed\n" + errors);
	}
}


Uniform* ShaderProgram::getUniform(const std::string& name)
{
	Uniform::Iter i = uniforms.find(name);
	if (i != uniforms.end())
		return i->second;
	// Create uniform
	Uniform* u = new Uniform();
	u->location = glGetUniformLocationARB(handle, name.c_str());
	u->name = name;
	uniforms[name] = u;
	return u;
}

