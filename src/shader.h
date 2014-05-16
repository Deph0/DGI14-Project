#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <list>
#include "opengl.h"


class Shader {
public:
	typedef std::list<Shader*> List;
	typedef List::const_iterator cIter;
	typedef List::iterator Iter;

	// Load shader from a file
	void load(const std::string& fname);

	GLuint getHandle() const {
		return handle;
	}

	Shader(GLenum type);
	~Shader();

private:
	void compile();

	GLuint handle;
};

#endif // SHADER_H

