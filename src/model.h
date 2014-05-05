#ifndef MODEL_H
#define MODEL_H

#include "object.h"
#include "material.h"
#include "texture.h"


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
	void load(const std::string& fname);// throw (ModelException);
	void draw() const;

	~Model() {
		cleanup();
	}

private:
	void loadMaterials(const std::string& fname);// throw (ModelException);
	void cleanup();
    static void tokenize(
        const std::string& str, char delim, std::list<std::string>* tokens);

	Object::List objects;
	Material::List materials;
	Texture::List textures;
};

#endif // MODEL_H

