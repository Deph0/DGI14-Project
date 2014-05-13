#ifndef LIGHT_H
#define LIGHT_H

#include <list>
#include "opengl.h"
#include "object.h"


class Light : public Object {
public:
	typedef std::list<Light*> List;
	typedef List::const_iterator cIter;
	typedef List::iterator Iter;

	enum Type {
		POINT_LIGHT,
		DIRECTIONAL_LIGHT
	};

	Light();

	// Initialize
	void initialize();
	// Draw a sphere at light position
	void draw() const;
	// Enable/Disable
	void enable(bool en = true) const;
	// Print debug info
	void print() const;

	virtual Type getType() const = 0;

	glm::vec4 color;

protected:
	static GLenum lightIndex;
	GLenum index;
};

#endif // LIGHT_H

