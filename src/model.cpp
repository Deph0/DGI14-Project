#include "model.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <SOIL.h>


void Model::cleanup()
{
	Object::Iter oi = objects.begin();
	for (; oi != objects.end(); ++oi)
		delete *oi;
	objects.clear();

	Material::Iter mi = materials.begin();
	for (; mi != materials.end(); ++mi)
		delete *mi;
	materials.clear();

	Texture::Iter ti = textures.begin();
	for (; ti != textures.end(); ++ti)
		delete *ti;
	textures.clear();
}


void Model::load(const std::string& fname)// throw (ModelException)
{
	std::vector<float> vertices, normals, texels;
	std::ifstream file;
	std::string path, line;
	float a, b, c;
	int lineCnt = 1;
	Object* obj = NULL;

	size_t pos = fname.find_last_of('/');
	path.assign(fname.c_str(), pos + 1);

	file.open(fname.c_str());
	if (!file.is_open()) {
		throw ModelException("could not open " + fname);
	}

	printf("reading %s\n", fname.c_str());

	// Start reading file data
	for (; std::getline(file, line); lineCnt++) {
		const char* cline = line.c_str();

		// Skip comments
		if (cline[0] == '#')
			continue;

		// Object with a name
		if (cline[0] == 'o' && cline[1] == ' ') {
			obj = new Object(cline + 2);
			objects.push_back(obj);
		}
		// Material library
		else if (strncmp(cline, "mtllib ", 7) == 0) {
			loadMaterials(path + (cline + 7));
		}
		// Material to use
		else if (strncmp(cline, "usemtl ", 7) == 0) {
			// Find the material, should be loaded already
			Material::Iter i = materials.begin();
			for (; i != materials.end(); ++i) {
				if ((*i)->name == (cline + 7)) {
					obj->material = *i;
					break;
				}
			}
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
			// The texture is fliiped upside down, fix it by
			// insert at the begining
			texels.insert(texels.begin(), b);
			texels.insert(texels.begin(), a);
		}
		// Normal in (x, y, z) form; normals might not be unit
		else if (sscanf(cline, "vn %f %f %f", &a, &b, &c) >= 2) {
			normals.push_back(a);
			normals.push_back(b);
			normals.push_back(c);
		}
		// Face in form (vertex1/texture1/normal1 ... vn/tn/nn)
		else if (cline[0] == 'f' && cline[1] == ' ') {
			std::list<std::string> parts, tokens;
			int i, idx;

			tokenize(cline + 2, ' ', &parts);
			std::list<std::string>::const_iterator pi = parts.begin();
			for (; pi != parts.end(); ++pi) {
				tokenize(*pi, '/', &tokens);
				std::list<std::string>::const_iterator ti = tokens.begin();
				// Vertex index (mandatory)
				if (ti == tokens.end() || ti->length() == 0) {
					char buf[100];
					sprintf(buf, "expecting vertex on line %d", lineCnt);
					throw ModelException(buf);
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
		// Smooth shading across polygons
		else if (cline[0] == 's' && cline[1] == ' ') {
			obj->smoothShading = (strcmp(cline + 2, "off") != 0);
		}
		else {
			printf("warning: skipping line %d [%s]\n", lineCnt, cline);
		}
	}

	file.close();
	printf("done reading %s\n", fname.c_str());
}


void Model::loadMaterials(const std::string& fname) //throw (ModelException)
{
	std::ifstream file;
	std::string line;
	Material* mat = NULL;
	float r, g, b;
	int n, cnt = 1;

	file.open(fname.c_str());
	if (!file.is_open()) {
		throw ModelException(std::string("could not open ") + fname);
	}

	printf("reading %s\n", fname.c_str());

	// Start reading file data
	for (; std::getline(file, line); cnt++) {
		const char* cline = line.c_str();
		// Skip comments and empty lines
		if (*cline == '#' || *cline == 0)
			continue;

		if (strncmp(cline, "newmtl ", 7) == 0) {
			mat = new Material(cline + 7);
			materials.push_back(mat);
		}
		else if (strncmp(cline, "map_Kd ", 7) == 0) {
			// Remove path and look in resources instead
			const char* img = rindex(cline + 7, '/');
			if (img == NULL) {
				img = cline + 7;
			}
			Texture* t = new Texture();
			t->load(std::string("../../res/") + img);
			textures.push_back(t);
			mat->diffuseTexture = t;
		}
		else if (sscanf(cline, "Ka %f %f %f", &r, &g, &b) == 3) {
			mat->ambient[0] = r;
			mat->ambient[1] = g;
			mat->ambient[2] = b;
		}
		else if (sscanf(cline, "Kd %f %f %f", &r, &g, &b) == 3) {
			mat->diffuse[0] = r;
			mat->diffuse[1] = g;
			mat->diffuse[2] = b;
		}
		else if (sscanf(cline, "Ks %f %f %f", &r, &g, &b) == 3) {
			mat->specular[0] = r;
			mat->specular[1] = g;
			mat->specular[2] = b;
		}
		else if (sscanf(cline, "Ns %f", &r) == 1) {
			mat->specularWeight = r;
		}
		else if (sscanf(cline, "Ni %f", &r) == 1) {
			mat->opticalDensity = r;
		}
		else if (sscanf(cline, "illum %d", &n) == 1) {
			mat->illuminationModel = n;
		}
		else if (sscanf(cline, "d %f", &r) == 1) {
			mat->setTransparency(r);
		}
		else {
			printf("warning: skipping line %d [%s]\n", cnt, cline);
		}
	}
	printf("done reading %s\n", fname.c_str());
}


void Model::tokenize(
	const std::string& str, char delim, std::list<std::string>* tokens)
{
	std::istringstream is(str);
	std::string token;

	tokens->clear();
	while (getline(is, token, delim)) {
		tokens->push_back(token);
	}
}


void Model::draw() const
{
	Material* m;

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	Object::cIter i = objects.begin();
	for (; i != objects.end(); ++i) {
		m = (*i)->material;

		printf("drawing %s\n", (*i)->name.c_str());
//		if ((*i)->name != "BkgndPlane_Plane.002")
//			continue;

		if (m) {
			m->apply();
/*			float s[] = {
				m->specular[0], m->specular[1], m->specular[2], m->transparency
			};
			float d[] = {
				m->diffuse[0], m->diffuse[1], m->diffuse[2], m->transparency
			};
			float a[] = {
				m->ambient[0], m->ambient[1], m->ambient[2], m->transparency
			};
			// Apply material for this material group
			glMaterialfv(GL_FRONT, GL_SPECULAR, s);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
			glMaterialfv(GL_FRONT, GL_AMBIENT, a);
			glMaterialf(GL_FRONT, GL_SHININESS, m->specularWeight);*/
		}

		if (m && m->diffuseTexture) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, (*i)->texels.data());
		}
		else {
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		glVertexPointer(3, GL_FLOAT, 0, (*i)->vertices.data());
		glNormalPointer(GL_FLOAT, 0, (*i)->normals.data());
		glDrawArrays(GL_TRIANGLES, 0, (*i)->vertices.size() / 3);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

