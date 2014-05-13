#ifndef TEXTURE_H
#define TEXTURE_H

#include "opengl.h"
#include <string>
#include <list>


class TextureException : public std::exception {
public:
	TextureException(const std::string& m)
	: msg(m)
	{ }
	virtual ~TextureException() throw() { }
	virtual const char* what() const throw() {
		return msg.c_str();
	}
private:
	std::string msg;
};


class Texture {
public:
	typedef std::list<Texture*> List;
	typedef List::const_iterator cIter;
	typedef List::iterator Iter;

	std::string filename;

	void load();

	// Select/Unselect the texture
	void select(bool en = true) const;

	Texture() {
		glGenTextures(1, &handle);
	}
	~Texture() {
		glDeleteTextures(1, &handle);
	}

private:
	GLuint handle;
};

#endif // TEXTURE_H

