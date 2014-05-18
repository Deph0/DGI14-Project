#include "dae_model.h"
#include "util.h"
#include "texture.h"
#include "point_light.h"
#include "exception.h"
#include <stdio.h>
#include <string.h>


DaeModel::DaeModel()
{
}


DaeModel::~DaeModel()
{
}


void DaeModel::load(const std::string& fname, Scene* scene)
{
	pugi::xml_document doc;
	pugi::xml_node node, instance;
	pugi::xml_parse_result result = doc.load_file(fname.c_str());
	std::string ref;
	Object* obj;

	if (!result)
		throw Exception(result.description());

	// Load materials
	readMaterials(doc, &scene->materials);

	// Go throw objects in the scene
	node = doc.select_single_node("//visual_scene").node();
	if (node.empty())
		throw Exception("no scene present");
	scene->name = node.attribute("name").value();
	pugi::xml_node_iterator ni = node.begin();
	for (; ni != node.end(); ++ni) {
		// Find type
		instance = ni->last_child();
		if (strncmp(instance.name(), "instance_", 9) != 0)
			throw Exception("can't find type of node");
		ref = instance.attribute("url").value() + 1;
		if (strcmp(instance.name() + 9, "camera") == 0) {
			obj = new Camera();
			readCameraParams(doc, ref, (Camera*)obj);
			scene->camera = (Camera*)obj;
		}
		else if (strcmp(instance.name() + 9, "light") == 0) {
			obj = readLightParams(doc, ref);
			if (obj) {
				scene->lights.push_back((Light*)obj);
			}
		}
		else if (strcmp(instance.name() + 9, "geometry") == 0) {
			obj = new Geometry();
			scene->geometries.push_back((Geometry*)obj);
			readGeometryParams(doc, ref, (Geometry*)obj, &scene->materials);
		}
		else {
			printf("warning: skipping unknown object [%s]\n",
				ni->attribute("name").value());
			continue;
		}
		// Read in common parameters
		obj->name = ni->attribute("name").value();
		readObjectParams(*ni, obj);
	}
}


void DaeModel::readMaterials(const pugi::xml_document& doc, Material::Map* lst)
{
	pugi::xml_node node, effect, phong, type;
	std::string id, str;
	Material* cur;
	Material::Element* elem;
	bool unknown;

	node = doc.select_single_node("//library_materials").node();
	if (node.empty()) {
		printf("no materials present\n");
		return;
	}
	node = node.child("material");
	for (; node; node = node.next_sibling("material")) {
		cur = new Material();
		cur->name = node.attribute("name").value();
		(*lst)[cur->name] = cur;

		id = node.child("instance_effect").attribute("url").value() + 1;
		// Read effect
		effect = doc.select_single_node(
			("//library_effects/effect[@id='" + id + "']").c_str()).node();
		if (effect.empty())
			throw Exception("missing parameters for material, id " + id);
		phong = effect.select_single_node(
			"profile_COMMON/technique/phong").node();
		if (phong.empty())
			throw Exception("missing common parameters for material, id " + id);
		pugi::xml_node_iterator it = phong.begin();
		for (; it != phong.end(); ++it) {
			type = it->first_child();
			elem = NULL;
			unknown = false;

			// Prepare elements
			if (strcmp(type.name(), "texture") == 0) {
				elem = new Material::Element();
				elem->texture = new Texture();
			}
			else if (strcmp(type.name(), "color") == 0) {
				elem = new Material::Element();
			}

			str = it->name();
			const char* val = type.text().get();

			if (elem) {
				if (str == "emission")
					cur->emission = elem;
				else if (str == "ambient")
					cur->ambient = elem;
				else if (str == "diffuse")
					cur->diffuse = elem;
				else if (str == "specular")
					cur->specular = elem;
				else {
					delete elem;
					unknown = true;
				}
			}
			else if (str == "shininess") {
				cur->shininess = new float(atof(val));
				continue;
			}
			else if (str == "transparency") {
				cur->transparency = new float(atof(val));
				continue;
			}
			else {
				unknown = true;
			}

			if (unknown) {
				printf("skipping material property [%s] for [%s]\n",
					str.c_str(), cur->name.c_str());
				continue;
			}
			else if (elem->texture) {
				id = type.attribute("texture").value();
				type = effect.select_single_node(
					("profile_COMMON/newparam[@sid='" + id + \
					"']/sampler2D/source").c_str()).node();
				id = type.text().get();
				type = effect.select_single_node(
					("profile_COMMON/newparam[@sid='" + id + \
					"']/surface/init_from").c_str()).node();
				id = type.text().get();
				type = doc.select_single_node(
					("//library_images/image[@id='" + id + \
					"']/init_from").c_str()).node();
				str = type.text().get();
				if (str.empty()) {
					throw Exception(
						"failed to get texture filename for " + cur->name);
				}
				elem->texture->filename =
					util::resource_path(util::unescape(str));
			}
			else { // This is a color
				if (sscanf(val, "%f %f %f %f",
					&elem->color.r, &elem->color.g, &elem->color.b, &elem->color.w) != 4)
				{
					throw Exception("failed to read material parameter " + \
						str + ", id " + cur->name);
				}
			}
		}
	}
}


void DaeModel::readObjectParams(const pugi::xml_node& node, Object* obj)
{
	std::string str;
	glm::vec3* v3;

	pugi::xml_node_iterator it = node.begin();
	for (; it != node.end(); ++it) {
		str = it->name();
		if (str == "rotate") {
			str = it->attribute("sid").value();
			glm::vec4* v4;

			if (str == "rotationX")
				v4 = &obj->rotation.x;
			else if (str == "rotationY")
				v4 = &obj->rotation.y;
			else if (str == "rotationZ")
				v4 = &obj->rotation.z;
			else continue;

			if (sscanf(it->text().get(), "%f %f %f %f",
				&v4->x, &v4->y, &v4->z, &v4->w) != 4)
			{
				throw Exception("missing parameters for camera in " + str);
			}
			continue;
		}
		if (str == "translate")
			v3 = &obj->position;
		else if (str == "scale")
			v3 = &obj->scaling;
		else continue;

		if (sscanf(it->text().get(), "%f %f %f", &v3->x, &v3->y, &v3->z) != 3)
			throw Exception("missing parameters for camera in " + str);
	}
}


void DaeModel::readCameraParams(
	const pugi::xml_document& doc, const std::string& ref, Camera* camera)
{
	pugi::xml_node node;
	std::string str;

	node = doc.select_single_node(("//camera[@id='" + ref + \
		"']/optics/technique_common/perspective").c_str()).node();
	if (node.empty())
		throw Exception("missing camera settings for id " + ref);

	camera->fov = node.child("xfov").text().as_float(45.f);
	camera->zNear = node.child("znear").text().as_float(0.1);
	camera->zFar = node.child("zfar").text().as_float(100);

	// Get the up axis
	camera->upAxis.x = camera->upAxis.y = camera->upAxis.z = 0.f;
	str = doc.select_single_node("//up_axis").node().text().get();
	if (str == "Z_UP")
		camera->upAxis.z = 1.f;
	else
		throw Exception("unknown camera up axes [" + str + "] for id " + ref);
}


Light* DaeModel::readLightParams(
	const pugi::xml_document& doc, const std::string& ref)
{
	pugi::xml_node node = doc.select_single_node(
		("//light[@id='" + ref + "']/technique_common").c_str()).node().first_child();
	if (node.empty())
		throw Exception("missing light settings for id " + ref);
	if (strcmp(node.name(), "point") != 0)
		throw Exception(util::format("light type [%s] is not supported yet", node.name()));

	PointLight point;

	if (sscanf(node.child("color").text().get(), "%f %f %f",
		&point.color.r, &point.color.g, &point.color.b) != 3)
	{
		throw Exception("failed to get light color, id " + ref);
	}
	if (sscanf(node.child("constant_attenuation").text().get(), "%f",
		&point.attenuation.constant) != 1)
	{
		throw Exception("missing constant attenuation for light " + ref);
	}
	if (sscanf(node.child("linear_attenuation").text().get(), "%f",
		&point.attenuation.linear) != 1)
	{
		throw Exception("missing linear attenuation for light " + ref);
	}
	if (sscanf(node.child("quadratic_attenuation").text().get(), "%f",
		&point.attenuation.quadratic) != 1)
	{
		throw Exception("missing quadratic attenuation for light " + ref);
	}

	return new PointLight(point);
}


void DaeModel::readGeometryParams(
	const pugi::xml_document& doc, const std::string& ref,
	Geometry* g, const Material::Map* lst)
{
	pugi::xml_node node, subnode, mesh;
	std::string id, str;
	std::vector<float> vertices, normals, texels;
	std::vector<std::string> tokens;

	mesh = doc.select_single_node(
		("//geometry[@id='" + ref + "']/mesh").c_str()).node();
	if (mesh.empty())
		throw Exception("missing geometry settings for id " + ref);

	node = mesh.child("polylist");
	// Select material
	id = node.attribute("material").value();
	subnode = doc.select_single_node(
		("//library_materials/material[@id='" + id + "']").c_str()).node();
	if (subnode) {
		str = subnode.attribute("name").value();
		Material::cIter i = lst->find(str);
		if (i == lst->end())
			throw Exception("can't find material [" + str + "] for geometry");
		g->material = i->second;
	}
	// Set geometries
	pugi::xml_node_iterator it = node.begin();
	for (; it != node.end(); ++it) {
		if (strcmp(it->name(), "p") == 0) {
			// Mapping of vertex/normal/texel
			util::tokenize(it->text().get(), ' ', &tokens);
			size_t n, i = 0, step = texels.size() ? 3 : 2;
			int vi, ni;
			for (; i < tokens.size(); i += step) {
				vi = util::strToInt(tokens.at(i)) * 3;
				ni = util::strToInt(tokens.at(i + 1)) * 3;
				for (n = 0; n < 3; n++) {
					g->vertices.push_back(vertices.at(vi + n));
					g->normals.push_back(normals.at(ni + n));
				}
				if (step == 3) {
					int idx = util::strToInt(tokens.at(i + 2)) * 2;
					g->texels.push_back(texels.at(idx));
					g->texels.push_back(texels.at(idx + 1));
				}
			}
		}
		if (strcmp(it->name(), "input") != 0)
			continue;
		// Input tag
		str = it->attribute("semantic").value();
		id = it->attribute("source").value() + 1;
		std::vector<float>* putIn;

		if (str == "VERTEX") {
			subnode = mesh.select_single_node(
				("vertices[@id='" + id + "']/input").c_str()).node();
			if (!subnode)
				throw Exception("missing geometry vertices info, id " + ref);
			id = subnode.attribute("source").value() + 1;
			subnode = mesh.select_single_node(
				("source[@id='" + id + "']/float_array").c_str()).node();
			if (subnode.empty())
				throw Exception("missing geometry vertices data, id " + ref);
			putIn = &vertices;
		}
		else if (str == "NORMAL") {
			subnode = mesh.select_single_node(
				("source[@id='" + id + "']/float_array").c_str()).node();
			if (subnode.empty())
				throw Exception("missing geometry normals data, id " + ref);
			putIn = &normals;
		}
		else if (str == "TEXCOORD") {
			subnode = mesh.select_single_node(
				("source[@id='" + id + "']/float_array").c_str()).node();
			if (subnode.empty())
				throw Exception(
					"missing geometry texture coordinates data, id " + ref);
			putIn = &texels;
		}
		else {
			printf("warning: skipping unknown geometry (id: %s) input [%s]\n",
				ref.c_str(), str.c_str());
			continue;
		}

		util::tokenize(subnode.text().get(), ' ', &tokens);
		std::vector<std::string>::const_iterator tokI = tokens.begin();
		for (; tokI != tokens.end(); ++tokI) {
			putIn->push_back(util::strToFloat(*tokI));
		}
	}
}

