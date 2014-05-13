#ifndef DRAWABLE_H
#define DRAWABLE_H

class Drawable {
public:
	virtual void initialize() { }
	virtual void draw() const { }

	virtual ~Drawable() { }
};

#endif // DRAWABLE_H

