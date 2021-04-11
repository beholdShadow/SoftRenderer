#pragma once

#include "rasterization.h"

#define MAX(a, b)  a > b ? a : b

class RendererController {
public:
	RendererController(int width, int height, std::string path, TGAImage& _outFrame);
	~RendererController();

	void run();
private:
	Model		   *_model = NULL;
	Rasterization  *_rasterize;
	IShader		   * _modelShader = NULL;
	IShader		   *_depthShader = NULL;

	TGAImage&		_outFrame;

	const int		_width;
	const int		_height;
	const int		_depth;

	vec3		    _direction_light;
	vec3		    _camera;
	vec3		    _center;

	float			* _zbuffer = NULL;
	float			* _shadowBuffer = NULL;
};