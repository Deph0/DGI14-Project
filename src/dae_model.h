#ifndef DAE_MODEL_H
#define DAE_MODEL_H

#include "model.h"
#include "scene.h"
#include <pugixml/pugixml.hpp>
#include <vector>


class DaeModel : public Model {
public:
	DaeModel();
	~DaeModel();

	void load(const std::string& fname, Scene* scene);

private:
	static void readObjectParams(
		const pugi::xml_node& node, Object* obj);
	static void readCameraParams(
		const pugi::xml_document& doc, const std::string& ref, Camera* camera);
	static Light* readLightParams(
		const pugi::xml_document& doc, const std::string& ref);
	static void readGeometryParams(
		const pugi::xml_document& doc, const std::string& ref, Geometry* g,
		const Material::Map* lst);
	static void readMaterials(
		const pugi::xml_document& doc, Material::Map* lst);
};

#endif // DAE_MODEL_H

