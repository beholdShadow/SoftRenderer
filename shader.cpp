#include "shader.h"
#include <cstdlib>

IShader::~IShader() {
}

ModelShader::~ModelShader() {
}

void ModelShader::setUniform(UniformBlock& block) {
	model = block.model;
	view = block.view;
	projection = block.projection;
	lightDir = block.lightDir;
	diffuseTexture = block.diffuseTexture;
	normalTexture = block.normalTexture;
}

//vec3 ModelShader::vertex(vec3 position, vec3 normal,
//							float *intensity) {
//	vec3 gl_Position = RendererUtil::mTov3(viewport * RendererUtil::NDC(projection * view * model * RendererUtil::v2m(position)));
//	*intensity = normal.normalize() * lightDir;
//	return gl_Position; // transform it to screen coordinates
//}
//
//bool ModelShader::fragment(vec2 uvCoord, float intensity, TGAColor& fragColor) {
//	if (intensity < 0) {
//		fragColor = diffuseTexture->get(diffuseTexture->get_width() * uvCoord.u,
//			diffuseTexture->get_height() * uvCoord.v) * std::abs(intensity);
//		return false;
//	} else {
//		return true;
//	}
//}

vec3 ModelShader::vertex(vec3 position) {
	vec3 gl_Position = RendererUtil::NDC(projection * view * model * vec4(position));
	return gl_Position; // transform it to screen coordinates
}

bool ModelShader::fragment(vec2 uvCoord, vec3 normal, TGAColor& fragColor) {
	float intensity = std::max((double)0.0f, normal.normalize() * (lightDir * (-1.0f)).normalize());

	fragColor = diffuseTexture->get(diffuseTexture->get_width() * uvCoord.u,
		diffuseTexture->get_height() * uvCoord.v) * intensity;

	for (int i = 0; i < 3; i++) {
		fragColor.raw[i] = std::min(fragColor.raw[i], (unsigned char)255);
	}

	return false;
}

bool ModelShader::fragment(vec2 uvCoord, vec3 normal, mat<3, 3> TBN, TGAColor& fragColor) {
	TGAColor nColor = normalTexture->get(diffuseTexture->get_width() * uvCoord.u,
		diffuseTexture->get_height() * uvCoord.v);
	vec3 n = vec3(nColor.r, nColor.g, nColor.a) / 255 * 2 - 1.0;
	n = TBN * n.normalize();

	float intensity = std::max((double)0.0f, n.normalize() * (lightDir * (-1.0f)).normalize());

	fragColor = diffuseTexture->get(diffuseTexture->get_width() * uvCoord.u,
		diffuseTexture->get_height() * uvCoord.v) * intensity;

	for (int i = 0; i < 3; i++) {
		fragColor.raw[i] = std::min(fragColor.raw[i], (unsigned char)255);
	}

	return false;
}