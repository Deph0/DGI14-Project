#ifndef UNIFORM_H
#define UNIFORM_H

#include <map>
#include <string>
#include <glm/glm.hpp>
#include "opengl.h"


class Uniform {
public:
	typedef std::map<std::string, Uniform*> Map;
	typedef Map::iterator Iter;

	void set(float v) {
		glUniform1fARB(location, v);
	}
	void set(float v1, float v2) {
		glUniform2fARB(location, v1, v2);
	}
	void set(float v1, float v2, float v3) {
		glUniform3fARB(location, v1, v2, v3);
	}
	void set(float v1, float v2, float v3, float v4) {
		glUniform4fARB(location, v1, v2, v3, v4);
	}
	void set(const glm::vec3& v) {
		glUniform3fv(location, 1, &v.x);
	}
	void set(const glm::vec4& v) {
		glUniform4fv(location, 1, &v.x);
	}
	// TODO add integers and matrices

	std::string name;

private:
	friend class ShaderProgram;

	Uniform() { }
	~Uniform() { }

	GLint location;
};

#endif // UNIFORM_H

