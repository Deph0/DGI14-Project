#ifndef MODEL_H
#define MODEL_H

#include <string>

class Scene;
class Camera;


class Model {
public:
	virtual void load(
		const std::string& fname, Scene* scene, Camera* camera = NULL) = 0;
};

#endif // MODEL_H

