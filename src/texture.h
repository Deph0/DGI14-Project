#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>
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

	void load(const std::string& fname) const;// throw (TextureException);

	void select() const {
		glBindTexture(GL_TEXTURE_2D, handle);
	}

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
