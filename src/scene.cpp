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

	// Sort geometries by transparency.
	// Transparent objects has to be sorted in how
	// close they are to the camera. This is a
	// simple sort for our scene.
	Geometry::List transparent;
	Geometry::Iter gi = geometries.begin();
	while (gi != geometries.end()) {
		//(*gi)->initialize();
		Material* m = (*gi)->material;
		if (m && m->transparency) {
			transparent.push_back(*gi);
			gi = geometries.erase(gi);
		}
		else ++gi;
	}
	geometries.insert(geometries.end(),
		transparent.begin(), transparent.end());
}


void Scene::draw() const
{
	if (camera)
		camera->draw();

	Light::cIter li = lights.begin();
	for (; li != lights.end(); ++li) {
		(*li)->draw();
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	Geometry::cIter gi = geometries.begin();
	for (; gi != geometries.end(); ++gi) {
		(*gi)->draw();
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}


Object* Scene::getByName(const std::string& name)
{
	if (camera && camera->name == name)
		return camera;
	// Check lights
	Light::Iter li = lights.begin();
	for (; li != lights.end(); ++li) {
		if ((*li)->name == name)
			return *li;
	}
	// Check geometries
	Geometry::Iter gi = geometries.begin();
	for (; gi != geometries.end(); ++gi) {
		if ((*gi)->name == name)
			return *gi;
	}
	// Not found
	return NULL;
}

