#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "shader.h"
#include "uniform.h"


class ShaderProgram {
public:
	Shader* createShader(GLenum type);
	Uniform* getUniform(const std::string& name);

	void initialize();

	void begin() const {
		glUseProgramObjectARB(handle);
	}
	void end() const {
		glUseProgramObjectARB(0);
	}

	ShaderProgram();
	~ShaderProgram();

private:
	GLuint handle;
	Shader::List shaders;
	Uniform::Map uniforms;
};

#endif // SHADER_PROGRAM_H

