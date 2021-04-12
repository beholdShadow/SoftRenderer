#pragma once

#include "model.h"
#include "shader.h"

class Rasterization
{
public:
	Rasterization(IShader* modelShader, IShader* depthShader);
	~Rasterization();

	void viewPort(int x, int y, int w, int h);

	void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);

	//fisrt of all, implement triangle rasterization with simple color
	void triangle(vec2 t0, vec2 t1, vec2 t2, TGAImage& image, TGAColor color);

	//1. add z-buffer to implement depth test
	//2. add diffuse texture
	void triangle(vec3* clipPts, float* zbuffer, TGAImage& image, TGAColor color);

	// add same light intensity used in single triangle
	void triangle(Model* model, vec3* clipPts, vec2* uvPts, float* zbuffer, TGAImage& image, float intensity);

	// add normalPts to calculate different light intensity
	void triangle(vec3* worldPts, vec4* clipPts, vec2* uvPts, vec3* normalPts,
		float* zbuffer, TGAImage& image);

	// calculate shadow Buffer
	void triangle(vec4* clipPts, float* shadowBuffer, TGAImage& depth);

protected:
	vec3 baryCentric(vec3* clipPts, vec3 P);

private:
	IShader	  * modelShader;
	IShader	  * depthShader;

	double			x;
	double			y;
	double			width;
	double			height;
	mat<4, 4>		viewport;
};

