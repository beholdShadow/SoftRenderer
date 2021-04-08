#include "model.h"

#include <iostream>
#include <fstream>
#include <sstream>

Model::Model(std::string filename) : verts(), vertFaces() {
	std::string objName = filename +  ".obj";
	std::string diffuseName = filename + "_diffuse.tga";
	std::string normalMapName = filename + "_nm.tga";
	std::string tNormalMapName = filename + "_nm_tangent.tga";
	std::ifstream in;
	in.open(objName, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			vec3 v;
			for (int i = 0; i < 3; i++) iss >> v[i];
			verts.push_back(v);
		}
		else if (!line.compare(0, 3, "vt ")) {
			iss >> trash >> trash;
			vec2 vt;
			for (int i = 0; i < 2; i++) iss >> vt[i];
			uverts.push_back(vt);
		}
		else if (!line.compare(0, 3, "vn ")) {
			iss >> trash >> trash;
			vec3 n;
			for (int i = 0; i < 3; i++) iss >> n[i];
			normalverts.push_back(n);
		}
		else if (!line.compare(0, 2, "f ")) {
			std::vector<int> f;
			std::vector<int> uvFace;
			std::vector<int> normalFace;
			int itrash, idx, texIdx, normalIdx;
			iss >> trash;
			while (iss >> idx >> trash >> texIdx >> trash >> normalIdx) {
				idx--; // in wavefront obj all indices start at 1, not zero
				texIdx--;
				normalIdx--;
				f.push_back(idx);
				uvFace.push_back(texIdx);
				normalFace.push_back(normalIdx);
			}
			vertFaces.push_back(f);
			uvFaces.push_back(uvFace);
			normalFaces.push_back(normalFace);
		}
	}

	diffuseTexture.read_tga_file(diffuseName.c_str());
	diffuseTexture.flip_vertically();

	normalTexture.read_tga_file(normalMapName.c_str());
	normalTexture.flip_vertically();

	tNormalTexture.read_tga_file(tNormalMapName.c_str());
	tNormalTexture.flip_vertically();
	std::cerr << "# v# " << verts.size() << " f# " << vertFaces.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
	return (int)verts.size();
}

int Model::nfaces() {
	return (int)vertFaces.size();
}

std::vector<int> Model::vertFace(int idx) {
	return vertFaces[idx];
}

std::vector<int> Model::uvFace(int idx) {
	return uvFaces[idx];
}

std::vector<int> Model::normalFace(int idx) {
	return normalFaces[idx];

}

vec3 Model::vert(int i) {
	return verts[i];
}

vec2 Model::uvert(int i) {
	return uverts[i];
}

vec3 Model::normalvert(int i) {
	return normalverts[i];

}

TGAColor Model::diffuse(vec2 uv) {
	return diffuseTexture.get(
		diffuseTexture.get_width() * uv.u,
		diffuseTexture.get_height() * uv.v);
}

TGAImage* Model::getDiffuseTexture() {
	return &diffuseTexture;
}

TGAImage* Model::getNormalTexture() {
	return &normalTexture;
}

TGAImage* Model::getTangentNormalTexture() {
	return &tNormalTexture;
}
