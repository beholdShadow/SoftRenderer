#pragma once

#include <vector>
#include "geometry.h"
#include "tgaimage.h"

class Model {
public:
	Model(std::string filename);
	~Model();
	int nverts();
	int nfaces();

	std::vector<int> vertFace(int idx);
	vec3 vert(int i);

	std::vector<int> uvFace(int idx);
	vec2 uvert(int i);

	std::vector<int> normalFace(int idx);
	vec3 normalvert(int i);

	TGAColor diffuse(vec2 uv);

	TGAImage* getDiffuseTexture();
	TGAImage* getNormalTexture();

private:
	std::vector<vec3> verts;
	std::vector<std::vector<int> > vertFaces;

	std::vector<vec2> uverts;
	std::vector<std::vector<int> > uvFaces;
	
	std::vector<vec3> normalverts;
	std::vector<std::vector<int> > normalFaces;

	TGAImage diffuseTexture;
	TGAImage normalTexture;
};
