#pragma once

#include "model.h"
#include "shader.h"

class Rasterization
{
public:
	Rasterization(int width, int height, IShader* shader);
	~Rasterization();

	void viewPort(int x, int y, int w, int h);

	void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);

	//fisrt of all, implement triangle rasterization with simple color
	void triangle(vec2 t0, vec2 t1, vec2 t2, TGAImage& image, TGAColor color);

	//1. add z-buffer to implement depth test
	//2. add diffuse texture
	void triangle(vec3* vertPts, float* zbuffer, TGAImage& image, TGAColor color);

	// add same light intensity used in single triangle
	void triangle(Model* model, vec3* vertPts, vec2* uvPts, float* zbuffer, TGAImage& image, float intensity);

	// add normalPts to calculate different light intensity
	//void triangle(Model* model, 
	//			vec3* vertPts, vec2* uvPts,
	//			float* zbuffer, 
	//			TGAImage& image, float* intensityPt);
	void triangle(vec3* vertPts, vec2* uvPts, vec3* normalPts, 
				mat<3, 3>& TBN,
				float* zbuffer, TGAImage& image);

protected:
	vec3 baryCentric(vec3* vertPts, vec3 P);

private:
	int			width;
	int			height;
	IShader	  * shader;

	mat<4, 4>   viewport;
};

