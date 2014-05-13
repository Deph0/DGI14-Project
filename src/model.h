#ifndef MODEL_H
#define MODEL_H

#include <string>

class Scene;


class ModelException : public std::exception {
public:
	ModelException(const std::string& m)
	: msg(m)
	{ }
	virtual ~ModelException() throw() { }
	virtual const char* what() const throw() {
		return msg.c_str();
	}
private:
	std::string msg;
};


class Model {
public:
	virtual void load(const std::string& fname, Scene* scene) = 0;
};

#endif // MODEL_H

