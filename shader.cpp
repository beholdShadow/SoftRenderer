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
	glowTexture = block.glowTexture;

	lightSpace = block.lightSpace;
	shadowBuffer = block.shadowBuffer;
	shadowW = block.shadowW;
	shadowH = block.shadowH;

	lightDir = (lightDir * (-1.0f)).normalize();
}

vec4 ModelShader::vertex(vec3 position) {
	vec4 gl_Position = projection * view * model * vec4(position);
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
	vec3 nColor = normalTexture->texture(uvCoord.u, uvCoord.v);
	vec3 diffColor = diffuseTexture->texture(uvCoord.u, uvCoord.v);
	vec3 specColor = specTexture->texture(uvCoord.u, uvCoord.v); 
	vec3 glowColor = glowTexture->texture(uvCoord.u, uvCoord.v);

	vec3 tangentNormal(
		nColor.r  * 2 - 1,
		nColor.g  * 2 - 1,
		nColor.b  * 2 - 1
	);

	tangentNormal = (TBN * tangentNormal.normalize()).normalize();

	//float shadowFactor = 1.0f;
	float shadowFactor = .3 + .7 * (1.0 -calculateShadow(lightSpace * embed<4>(fragCoord, 1.0f)));

	float ambient = 5 / 255.0;

	double diff = std::max(0.0, tangentNormal * lightDir);

	vec3 reflectDir = (tangentNormal * (tangentNormal * lightDir) * 2 - lightDir).normalize(); 
	vec3 viewDir = (viewPos - fragCoord).normalize();
	vec3 halfway = (viewDir + lightDir).normalize();

	//double spec = pow(std::max(viewDir*reflectDir, 0.0), 5 + specColor[0] * 255);
	double spec = pow(std::max(halfway * tangentNormal, 0.0), 5 + specColor.r * 255);

	vec3 vfragColor = diffColor * (ambient + shadowFactor * (1.2 * diff + 0.6 * spec))  + glowColor * 10;

	vec3 result = vec3(1.0, 1.0, 1.0) - vec3(std::exp(-vfragColor.x * 1.0), std::exp(-vfragColor.y * 1.0), std::exp(-vfragColor.z * 1.0));

	fragColor = TGAColor::transform(result);

	return false;
}

float ModelShader::calculateShadow(vec4 fragPos) {
	vec3 projCoords = proj<3>(fragPos / fragPos.w);
	// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	projCoords.x = std::max(0.0, std::min(projCoords.x, 1.0));
	projCoords.y = std::max(0.0, std::min(projCoords.y, 1.0));
	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = shadowBuffer[int(shadowW * projCoords.x) + int(shadowW * (shadowH * projCoords.y))];
	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	// calculate bias (based on depth map resolution and slope)
	//vec3 normal = normalize(fs_in.Normal);
	//vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	//float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

	return currentDepth > 0.2 + closestDepth;
}

DepthShader::~DepthShader() {
}

void DepthShader::setUniform(UniformBlock& block) {
	model = block.model;
	view = block.view;
	projection = block.projection;
}

vec4 DepthShader::vertex(vec3 position) {
	vec4 gl_Position = projection * view * model * vec4(position);
	return gl_Position; // transform it to screen coordinates
}