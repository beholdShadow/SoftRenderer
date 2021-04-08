#include "renderer_controller.h"

RendererController::RendererController(int width, int height, std::string path, TGAImage& outFrame) :
				_outFrame(outFrame),
					_width(width),
					_height(height),
					_depth(255),
					_light_dir(-1, -1, -1),
					_camera(1, 1, 3),
					_center(0, 0, 0) {
	_model = new Model(path);
	_zbuffer = new float[_width * _height];
	_shader = new ModelShader();
	_rasterize = new Rasterization(width, height, _shader);
	for (int i = _width * _height; i--; _zbuffer[i] = -std::numeric_limits<float>::max());
}

RendererController::~RendererController() {
	if (_model) {
		delete _model;
	}
	if (_zbuffer) {
		delete[] _zbuffer;
	}
	if (_shader) {
		delete _shader;
	}
	if (_rasterize) {
		delete _rasterize;
	}
}

void RendererController::run() {
	UniformBlock block;
	block.model = mat<4,4>::identity();
	block.model[0][3] = 1.5;
	block.model[1][3] = 1.6;
	block.model[2][3] = 1.0;
	block.view = RendererUtil::lookat(_camera, _center, vec3(0.0, 1.0, 0.0));
	block.projection = RendererUtil::projection((_camera - _center).length());
	block.diffuseTexture = _model->getDiffuseTexture();
	block.normalTexture = _model->getNormalTexture();
	block.lightDir = _light_dir;

	_rasterize->viewPort(0, 0, _width, _height);

	_shader->setUniform(block);

	for (int i = 0; i < _model->nfaces(); i++) {
		std::vector<int> vertFace = _model->vertFace(i);
		std::vector<int> uvFace = _model->uvFace(i);
		std::vector<int> normalFace = _model->normalFace(i);
		vec3 vert_coords[3];
		vec2 uv_coords[3];
		vec3 normal_coords[3];
		for (int j = 0; j < 3; j++) {
			uv_coords[j] = _model->uvert(uvFace[j]);
			normal_coords[j] = _model->normalvert(normalFace[j]);
			vert_coords[j] = _shader->vertex(_model->vert(vertFace[j]));
		}
		_rasterize->triangle(vert_coords, uv_coords, normal_coords, _zbuffer, _outFrame);
	}
}

