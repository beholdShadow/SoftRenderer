#pragma once

#include "tgaimage.h"
#include "geometry.h"

#define uniform 
#define vary

typedef TGAImage* sample2D;

struct UniformBlock {
	mat<4,4> model;
	mat<4,4> view;
	mat<4, 4> projection;
	vec3  lightDir;
	vec3  viewPos;
	sample2D diffuseTexture;
	sample2D normalTexture;
	sample2D specTexture;
};

class RendererUtil {
public:
	static vec3 NDC(vec4 m) {
		vec3 ndc;
		ndc.x = m[0] / m[3];
		ndc.y = m[1] / m[3];
		ndc.z = (m[2] / m[3] + 1.0f) * 0.5f;
		return ndc;
	}

	static vec3 pixelCorrection(vec4 m) {
		return vec3(int(m.x + 0.5), int(m.y + 0.5), m.z);
	}

	static mat<4,4> lookat(vec3 eye, vec3 center, vec3 up) {
		vec3 z = (eye - center).normalize();
		vec3 x = vec3::cross(up, z).normalize();
		vec3 y = vec3::cross(z, x).normalize();
		mat<4,4> res = mat<4,4>::identity();
		for (int i = 0; i < 3; i++) {
			res[0][i] = x[i];
			res[1][i] = y[i];
			res[2][i] = z[i];
			res[i][3] = -eye[i];
			//res[i][3] = -center[i];
		}
		std::cerr << res << "\n";
		return res;
	}

	static mat<4,4> projection(float coeff) {
		mat<4,4> projection = mat<4,4>::identity();
		projection[2][2] = -1.f;
		projection[3][2] = -1.f / coeff;
		std::cerr << projection << "\n";
		return projection;
	}

};

class IShader
{
public:
	virtual ~IShader() = 0;
	virtual void setUniform(UniformBlock& block) = 0;

	virtual vec3 vertex(vec3 position) = 0;
	virtual bool fragment(vec2 uvCoord, vec3 normal, TGAColor& fragColor) = 0;
	virtual bool fragment(vec3 fragCoord, vec2 uvCoord, mat<3, 3> TBN, TGAColor& fragColor) = 0;
};

class ModelShader:public IShader
{
public:
	virtual ~ModelShader() override;
	virtual void setUniform(UniformBlock& block) override;

	virtual vec3 vertex(vec3 position) override;
	virtual bool fragment(vec2 uvCoord, vec3 normal, TGAColor& fragColor) override;
	virtual bool fragment(vec3 fragCoord, vec2 uvCoord, mat<3, 3> TBN, TGAColor& fragColor) override;

private:
	uniform mat<4,4>			model;
	uniform mat<4,4>			view;
	uniform mat<4,4>			projection;
	uniform sample2D			diffuseTexture;
	uniform sample2D			normalTexture;
	uniform sample2D			specTexture;
	uniform vec3				lightDir;
	uniform vec3				viewPos;
};

