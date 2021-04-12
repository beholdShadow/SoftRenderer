#include "renderer_controller.h"

RendererController::RendererController(int width, int height, std::string path, TGAImage& outFrame) :
				_outFrame(outFrame),
					_width(width),
					_height(height),
					_depth(255),
					_direction_light(-1, -1, 0),
					_camera(1, 1, 4),
					_center(0, 0, 0) {
	_model = new Model(path);
	_modelShader = new ModelShader();
	_depthShader = new DepthShader();
	_rasterize = new Rasterization(_modelShader, _depthShader);
	_zbuffer = new float[width * height];
	_shadowBuffer = new float[width * height];
	for (int i = width * height; i--; _zbuffer[i] = std::numeric_limits<float>::max());
	for (int i = width * height; i--; _shadowBuffer[i] = std::numeric_limits<float>::max());
}

RendererController::~RendererController() {
	if (_model) {
		delete _model;
	}
	if (_modelShader) {
		delete _modelShader;
	}
	if (_depthShader) {
		delete _depthShader;
	}
	if (_rasterize) {
		delete _rasterize;
	}
	if (_zbuffer) {
		delete[] _zbuffer;
	}
	if (_shadowBuffer) {
		delete[] _shadowBuffer;
	}
}

void RendererController::run() {
	UniformBlock modelBlock;
	modelBlock.model = mat<4,4>::identity();
	//modelBlock.model[0][3] = 1.5;
	//modelBlock.model[1][3] = 1.6;
	//modelBlock.model[2][3] = 1.0;
	modelBlock.view = RendererUtil::lookat(_camera, _center, vec3(0.0, 1.0, 0.0));
	modelBlock.projection = RendererUtil::projection((_camera - _center).length());;

	modelBlock.diffuseTexture = _model->getDiffuseTexture();
	modelBlock.normalTexture = _model->getTangentNormalTexture();
	modelBlock.specTexture = _model->getSpecularTexture();
	modelBlock.glowTexture = _model->getGlowTexture();

	modelBlock.lightDir = _direction_light;
	modelBlock.viewPos = _camera;

	UniformBlock depthBlock;
	depthBlock.model = modelBlock.model;
	depthBlock.view = RendererUtil::lookat(_direction_light * (-1.0f), _center, vec3(0.0, 1.0, 0.0));
	depthBlock.projection = RendererUtil::projection(0);

	mat<4, 4> viewport = mat<4, 4>::identity();
	viewport[0][3] = _width / 2.f;
	viewport[1][3] = _height / 2.f;
	viewport[0][0] = _width / 2.f;
	viewport[1][1] = _height / 2.f;

	modelBlock.lightSpace = viewport * depthBlock.projection * depthBlock.view;
	modelBlock.shadowBuffer = _shadowBuffer;
	modelBlock.shadowW = _width;
	modelBlock.shadowH = _height;

	_modelShader->setUniform(modelBlock);
	_depthShader->setUniform(depthBlock);

	_rasterize->viewPort(0, 0, _width, _height);

	TGAImage depthTga(_width, _height, TGAImage::RGB);

	for (int i = 0; i < _model->nfaces(); i++) {
		std::vector<int> vertFace = _model->vertFace(i);
		vec4 light_clip_coords[3];
		for (int j = 0; j < 3; j++) {
			light_clip_coords[j] = _depthShader->vertex(_model->vert(vertFace[j]));
		}

		_rasterize->triangle(light_clip_coords, _shadowBuffer, depthTga);
	}

	depthTga.flip_vertically(); // to place the origin in the bottom left corner of the image 
	depthTga.write_tga_file("depth.tga");

	_rasterize->viewPort(0, 0, _width, _height);

	for (int i = 0; i < _model->nfaces(); i++) {
		std::vector<int> vertFace = _model->vertFace(i);
		std::vector<int> uvFace = _model->uvFace(i);
		std::vector<int> normalFace = _model->normalFace(i);
		vec3 world_coords[3];
		vec4 clip_coords[3];
		vec2 uv_coords[3];
		vec3 normal_coords[3];
		for (int j = 0; j < 3; j++) {
			//world_coords[j] = proj<3>(block.model * embed<4>(_model->vert(vertFace[j]), 1.0));
			world_coords[j] = _model->vert(vertFace[j]);
			clip_coords[j] = _modelShader->vertex(_model->vert(vertFace[j]));
			uv_coords[j] = _model->uvert(uvFace[j]);
			normal_coords[j] = _model->normalvert(normalFace[j]);
		}
		_rasterize->triangle(world_coords, clip_coords, uv_coords, normal_coords, _zbuffer, _outFrame);
	}
}

