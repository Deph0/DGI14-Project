#ifndef OBJ_MODEL_H
#define OBJ_MODEL_H

#include "model.h"
#include "scene.h"
#include "material.h"
#include "texture.h"
#include <vector>


class ObjModel : public Model {
public:
	void load(const std::string& fname, Scene* scene);

private:
	static void loadMaterials(
		const std::string& fname, Material::Map* lst);
};

#endif // OBJ_MODEL_H

