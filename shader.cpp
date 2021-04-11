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
	specTexture = block.specTexture;

	lightDir = (lightDir * (-1.0f)).normalize();
}

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
	TGAColor nColor = normalTexture->texture(uvCoord.u, uvCoord.v);
	TGAColor diffColor = diffuseTexture->texture(uvCoord.u, uvCoord.v);
	TGAColor specColor = specTexture->texture(uvCoord.u, uvCoord.v);

	vec3 tangentNormal(
		nColor.r / 255. * 2 - 1,
		nColor.g / 255. * 2 - 1,
		nColor.b / 255. * 2 - 1
	);

	tangentNormal = (TBN * tangentNormal.normalize()).normalize();

	float diff = std::max(0.0, tangentNormal * lightDir);

	vec3 reflectDir = (tangentNormal * (tangentNormal * lightDir) * 2 - lightDir).normalize(); 
	vec3 viewDir = (viewPos - fragCoord).normalize();
	vec3 halfway = (viewDir + lightDir).normalize();

	//float spec = pow(std::max(viewDir*reflectDir, 0.0), specColor[0]);
	float spec = pow(std::max(halfway * tangentNormal, 0.0), specColor[0]);

	fragColor = diffColor * (diff + 0.6 * spec);

	for (int i = 0; i < 3; i++) {
		fragColor.raw[i] = std::min(1 + fragColor.raw[i], 255);
	}

	return false;
}