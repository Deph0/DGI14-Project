#include "obj_model.h"
#include "util.h"
#include "exception.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string.h>


void ObjModel::load(const std::string& fname, Scene* scene, Camera*)
{
	std::vector<float> vertices, normals, texels;
	std::ifstream file;
	std::string line;
	float a, b, c;
	int lineCnt = 1;
	Geometry* obj = NULL;

	file.open(fname.c_str());
	if (!file.is_open()) {
		throw Exception("could not open " + fname);
	}

	printf("loading %s\n", fname.c_str());

	// Start reading file data
	for (; std::getline(file, line); lineCnt++) {
		const char* cline = line.c_str();

		// Skip comments
		if (cline[0] == '#')
			continue;

		// Object with a name
		if (cline[0] == 'o' && cline[1] == ' ') {
			obj = new Geometry();
			scene->geometries.push_back(obj);
			obj->name = cline + 2;
		}
		// Material library
		else if (strncmp(cline, "mtllib ", 7) == 0) {
			loadMaterials(
				util::resource_path(cline + 7), &scene->materials);
		}
		// Material to use
		else if (strncmp(cline, "usemtl ", 7) == 0) {
			// Find the material, should be loaded already
			Material::cIter i = scene->materials.find(cline + 7);
			if (i == scene->materials.end())
				throw Exception(util::format("material [%s] not found", cline + 7));
			obj->material = i->second;
		}
		// Vertex with (x,y,z) coordinates
		else if (sscanf(cline, "v %f %f %f", &a, &b, &c) == 3) {
			vertices.push_back(a);
			vertices.push_back(b);
			vertices.push_back(c);
		}
		// Texture coordinate, in (u ,v) coordinates, these will vary
		// between 0 and 1
		else if (sscanf(cline, "vt %f %f", &a, &b) == 2) {
			texels.push_back(a);
			texels.push_back(b);
		}
		// Normal in (x, y, z) form; normals might not be unit
		else if (sscanf(cline, "vn %f %f %f", &a, &b, &c) >= 2) {
			normals.push_back(a);
			normals.push_back(b);
			normals.push_back(c);
		}
		// Face in form (vertex1/texture1/normal1 ... vn/tn/nn)
		else if (cline[0] == 'f' && cline[1] == ' ') {
			std::vector<std::string> parts, tokens;
			int i, idx;

			util::tokenize(cline + 2, ' ', &parts);
			std::vector<std::string>::const_iterator pi = parts.begin();
			for (; pi != parts.end(); ++pi) {
				util::tokenize(*pi, '/', &tokens);
				std::vector<std::string>::const_iterator ti = tokens.begin();
				// Vertex index (mandatory)
				if (ti == tokens.end() || ti->length() == 0) {
					char buf[100];
					sprintf(buf, "expecting vertex on line %d", lineCnt);
					throw Exception(buf);
				}
				idx = (atoi(ti->c_str()) - 1) * 3;
				for (i = 0; i < 3; i++)
					obj->vertices.push_back(vertices.at(i + idx));
				++ti;
				// Texture coordinates index (optional)
				if (ti == tokens.end()) {
					continue;
				}
				if (ti->length()) {
					idx = (atoi(ti->c_str()) - 1) * 2;
					for (i = 0; i < 2; i++)
						obj->texels.push_back(texels.at(i + idx));
				}
				++ti;
				// Normal index (optional)
				if (ti == tokens.end()) {
					continue;
				}
				if (ti->length()) {
					idx = (atoi(ti->c_str()) - 1) * 3;
					for (i = 0; i < 3; i++)
						obj->normals.push_back(normals.at(i + idx));
				}
			}
		}
//		// Smooth shading across polygons
//		else if (cline[0] == 's' && cline[1] == ' ') {
//			obj->smoothShading = (strcmp(cline + 2, "off") != 0);
//		}
		else {
			printf("warning: skipping line %d [%s]\n", lineCnt, cline);
		}
	}

	file.close();
}


void ObjModel::loadMaterials(const std::string& fname, Material::Map* lst)
{
	std::ifstream file;
	std::string line;
	Material* cur = NULL;
	float r, g, b;
	int cnt = 1;
	char type;

	file.open(fname.c_str());
	if (!file.is_open()) {
		throw Exception("could not open " + fname);
	}

	printf("loading %s\n", fname.c_str());

	// Start reading file data
	for (; std::getline(file, line); cnt++) {
		const char* cline = line.c_str();
		// Skip comments and empty lines
		if (*cline == '#' || *cline == 0)
			continue;

		if (strncmp(cline, "newmtl ", 7) == 0) {
			cur = new Material();
			cur->name = cline + 7;
			(*lst)[cur->name] = cur;
		}
		else if (strncmp(cline, "map_K", 5) == 0 && cline[6] == ' ') {
			type = cline[5];
			Material::Element** el;
			if (type == 'a')
				el = &cur->ambient;
			else if (type == 'd')
				el = &cur->diffuse;
			else if (type == 's')
				el = &cur->specular;
			else {
				printf("warning: unknown material texture type [%c] on line %d\n", type, cnt);
				continue;
			}
			if (*el == NULL)
				*el = new Material::Element();
			// Remove path and look in resources instead
			const char* img = rindex(cline + 7, '/');
			if (img == NULL) {
				img = cline + 7;
			}
			Texture* t = new Texture();
			t->filename = util::resource_path(img);
			(*el)->texture = t;
		}
		else if (sscanf(cline, "K%c %f %f %f", &type, &r, &g, &b) == 4) {
			Material::Element** el;
			if (type == 'a')
				el = &cur->ambient;
			else if (type == 'd')
				el = &cur->diffuse;
			else if (type == 's')
				el = &cur->specular;
			else {
				printf("warning: unknown material color type [%c] on line %d\n", type, cnt);
				continue;
			}
			if (*el == NULL)
				*el = new Material::Element();
			(*el)->color = glm::vec4(r, g, b, 1.f);
		}
		else if (sscanf(cline, "Ns %f", &r) == 1) {
			cur->shininess = new float(r);
		}
//		else if (sscanf(cline, "Ni %f", &r) == 1) {
//			cur->opticalDensity = r;
//		}
//		else if (sscanf(cline, "illum %d", &n) == 1) {
//			cur->illuminationModel = n;
//		}
		else if (sscanf(cline, "d %f", &r) == 1) {
			cur->transparency = new float(r);
		}
		else {
			printf("warning: skipping line %d [%s]\n", cnt, cline);
		}
	}
	printf("done reading %s\n", fname.c_str());
}

