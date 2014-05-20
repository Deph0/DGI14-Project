#include "scene.h"


Scene::Scene()
: camera(NULL)
{
}


Scene::~Scene()
{
	Light::Iter li = lights.begin();
	for (; li != lights.end(); ++li) {
		delete *li;
	}
	Geometry::Iter gi = geometries.begin();
	for (; gi != geometries.end(); ++gi) {
		delete *gi;
	}
	Material::Iter mi = materials.begin();
	for (; mi != materials.end(); ++mi) {
		delete mi->second;
	}
}


void Scene::initialize()
{
	if (camera)
		camera->initialize();

	if (lights.size()) {
		glEnable(GL_LIGHTING);

		Light::Iter li = lights.begin();
		for (; li != lights.end(); ++li) {
			(*li)->initialize();
			(*li)->enable();
		}
	}

	Material::Iter mi = materials.begin();
	for (; mi != materials.end(); ++mi) {
		mi->second->initialize();
	}

	Geometry::Iter gi = geometries.begin();
	for (; gi != geometries.end(); ++gi) {
		(*gi)->initialize();
	}
}


void Scene::draw() const
{
	if (camera)
		camera->draw();

	Light::cIter li = lights.begin();
	for (; li != lights.end(); ++li) {
		(*li)->draw();
	}

	Geometry::cIter gi = geometries.begin();
	for (; gi != geometries.end(); ++gi) {
		(*gi)->draw();
	}
}


Object* Scene::getByName(const std::string& name, bool erase)
{
	if (camera && camera->name == name) {
		Camera* obj = camera;
		if (erase)
			camera = NULL;
		return obj;
	}
	// Check lights
	Light::Iter li = lights.begin();
	for (; li != lights.end(); ++li) {
		if ((*li)->name == name) {
			Light* obj = *li;
			if (erase)
				lights.erase(li);
			return obj;
		}
	}
	// Check geometries
	Geometry::Iter gi = geometries.begin();
	for (; gi != geometries.end(); ++gi) {
		if ((*gi)->name == name) {
			Geometry* obj = *gi;
			if (erase)
				geometries.erase(gi);
			return obj;
		}
	}
	// Not found
	return NULL;
}

