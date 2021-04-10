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
	viewPos = block.viewPos;
	diffuseTexture = block.diffuseTexture;
	normalTexture = block.normalTexture;
	tNormalTexture = block.tNormalTexture;
	specTexture = block.specTexture;

	lightDir = (lightDir * (-1.0f)).normalize();
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

bool ModelShader::fragment(vec3 fragCoord, vec2 uvCoord, mat<3, 3> TBN, TGAColor& fragColor) {
	TGAColor nColor = normalTexture->get(normalTexture->get_width() * uvCoord.u,
		normalTexture->get_height() * uvCoord.v);
	TGAColor tnColor = tNormalTexture->get(tNormalTexture->get_width() * uvCoord.u,
		tNormalTexture->get_height() * uvCoord.v);
	TGAColor diffColor = diffuseTexture->get(diffuseTexture->get_width() * uvCoord.u,
		diffuseTexture->get_height() * uvCoord.v);
	TGAColor specColor = specTexture->get(specTexture->get_width() * uvCoord.u,
		specTexture->get_height() * uvCoord.v);

	vec3 worldNormal;
	vec3 tangentNormal;
	for (int i = 0; i < 3; i++) {
		worldNormal[i] = nColor[i] / 255. * 2 - 1;
		tangentNormal[2-i] = tnColor[i] / 255. * 2 - 1;
	}
	worldNormal.normalize();
	tangentNormal = (TBN * tangentNormal.normalize()).normalize();

	float diff = std::max(0.0, tangentNormal * lightDir);

	vec3 reflectDir = (tangentNormal * (tangentNormal * lightDir) * 2 - lightDir).normalize(); 
	vec3 viewDir = (viewPos - fragCoord).normalize();

	float spec = pow(std::max(viewDir*reflectDir, 0.0), specColor[0]);
	//float spec = pow(std::max(viewDir * reflectDir, 0.0), 32);

	fragColor = diffColor * (diff + 0.6 * spec);

	for (int i = 0; i < 3; i++) {
		fragColor.raw[i] = std::min(1 + fragColor.raw[i], 255);
	}

	return false;
}