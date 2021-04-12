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
	sample2D glowTexture;

	float* shadowBuffer;
	mat<4, 4> lightSpace;
	int	   shadowW;
	int	   shadowH;
};

class RendererUtil {
public:
	static mat<4,4> lookat(vec3 eye, vec3 center, vec3 up) {
		vec3 z = (eye - center).normalize();
		vec3 x = vec3::cross(up, z).normalize();
		vec3 y = vec3::cross(z, x).normalize();
		mat<4,4> res = mat<4,4>::identity();
		for (int i = 0; i < 3; i++) {
			res[0][i] = x[i];
			res[1][i] = y[i];
			res[2][i] = z[i];
			//res[i][3] = -eye[i];
			res[i][3] = -center[i];
		}
		std::cerr << res << "\n";
		return res;
	}

	static mat<4,4> projection(float coeff) {
		mat<4,4> projection = mat<4,4>::identity();
		projection[2][2] = -1.f;
		if(coeff > 1e-6f)
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

	virtual vec4 vertex(vec3 position) = 0;
	virtual bool fragment() = 0;
	virtual bool fragment(vec2 uvCoord, vec3 normal, TGAColor& fragColor) = 0;
	virtual bool fragment(vec3 worldCoord, vec2 uvCoord, mat<3, 3> TBN, TGAColor& fragColor) = 0;
};

class ModelShader:public IShader
{
public:
	virtual ~ModelShader() override;
	virtual void setUniform(UniformBlock& block) override;

	virtual vec4 vertex(vec3 position) override;
	virtual bool fragment() override { return false; }
	virtual bool fragment(vec2 uvCoord, vec3 normal, TGAColor& fragColor) override;
	virtual bool fragment(vec3 worldCoord, vec2 uvCoord, mat<3, 3> TBN, TGAColor& fragColor) override;

protected:
	float calculateShadow(vec4 fragPosLightSpace);

private:
	uniform mat<4,4>			model;
	uniform mat<4,4>			view;
	uniform mat<4,4>			projection;

	uniform sample2D			diffuseTexture;
	uniform sample2D			normalTexture;
	uniform sample2D			specTexture; 
	uniform sample2D			glowTexture;

	uniform vec3				lightDir;
	uniform vec3				viewPos;

	uniform	mat<4, 4>			lightSpace;
	uniform	float			  * shadowBuffer;
	int						    shadowW;
	int						    shadowH;
};

class DepthShader :public IShader
{
public:
	virtual ~DepthShader() override;
	virtual void setUniform(UniformBlock& block) override;

	virtual vec4 vertex(vec3 position) override;
	virtual bool fragment() override { return false; }
	virtual bool fragment(vec2 uvCoord, vec3 normal, TGAColor& fragColor) override { return false; }
	virtual bool fragment(vec3 worldCoord, vec2 uvCoord, mat<3, 3> TBN, TGAColor& fragColor) override { return false; }

private:
	uniform mat<4, 4>			model;
	uniform mat<4, 4>			view;
	uniform mat<4, 4>			projection;
};