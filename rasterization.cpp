#include "rasterization.h"
#include "shader.h"

Rasterization::Rasterization(int width, int height, IShader* shader) {
	this->width = width;
	this->height = height;
	this->shader = shader;
}

Rasterization::~Rasterization() {
}

void Rasterization::viewPort(int x, int y, int w, int h) {
	viewport = mat<4, 4>::identity();
	viewport[0][3] = x + w / 2.f;
	viewport[1][3] = y + h / 2.f;

	viewport[0][0] = w / 2.f;
	viewport[1][1] = h / 2.f;

	std::cerr << viewport << "\n";
}

void Rasterization::line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

vec3 Rasterization::baryCentric(vec3* vertPts, vec3 P) {
	vec3 u = vec3::cross(vec3(vertPts[2][0] - vertPts[0][0], vertPts[1][0] - vertPts[0][0], vertPts[0][0] - P[0]), vec3(vertPts[2][1] - vertPts[0][1], vertPts[1][1] - vertPts[0][1], vertPts[0][1] - P[1]));
	/* `vertPts` and `P` has integer value as coordinates
	   so `abs(u[2])` < 1 means `u[2]` is 0, that means
	   triangle is degenerate, in this case return something with negative coordinates */
	if (std::abs(u[2]) < 1e-5) return vec3(-1, 1, 1);
	return vec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

void Rasterization::triangle(vec2 t0, vec2 t1, vec2 t2, TGAImage& image, TGAColor color) {
	if (t1.y > t2.y) { std::swap(t1, t2); }
	if (t0.y > t2.y) { std::swap(t0, t2); }
	if (t0.y > t1.y) { std::swap(t0, t1); }

	float totalHeight = t2.y - t0.y + 1;
	float segmentHeight1 = t1.y - t0.y;
	float segmentHeight2 = t2.y - t1.y;
	float alpha;
	vec2 A;
	for (int i = t0.y; i < t2.y; i++)
	{
		if (i < t1.y) {
			alpha = (i - t0.y) / segmentHeight1;
			A = t0 + (t1 - t0) * alpha;
		}
		else {
			alpha = (i - t1.y) / segmentHeight2;
			A = t1 + (t2 - t1) * alpha;
		}

		float beta = (i - t0.y) / totalHeight;
		vec2 B = t0 + (t2 - t0) * beta;

		if (A.x > B.x)
			std::swap(A, B);

		for (int x = A.x; x < B.x; x++) {
			image.set(x, i, color);
		}
	}
}

void Rasterization::triangle(vec3* vertPts, float* zbuffer, TGAImage& image, TGAColor color) {
	if (vertPts[0].y == vertPts[1].y && vertPts[0].y == vertPts[2].y) return;
	vec2 bboxmin(image.get_width() - 1, image.get_height() - 1);
	vec2 bboxmax(0, 0);
	vec2 clamp(image.get_width() - 1, image.get_height() - 1);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			bboxmin[j] = std::max(0.0, std::min(bboxmin[j], vertPts[i][j]));
			bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], vertPts[i][j]));
		}
	}
	vec3 P;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
			vec3 bc_screen = baryCentric(vertPts, P);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
			P.z = 0;
			for (int i = 0; i < 3; i++) P.z += vertPts[i][2] * bc_screen[i];
			if (zbuffer[int(P.x + P.y * width)] < P.z) {
				zbuffer[int(P.x + P.y * width)] = P.z;
				image.set(P.x, P.y, color);
			}
		}
	}
}

void Rasterization::triangle(Model* model, vec3* vertPts, vec2* uvPts, float* zbuffer, TGAImage& image, float intensity) {
	if (vertPts[0].y == vertPts[1].y && vertPts[0].y == vertPts[2].y) return;
	vec2 bboxmin(image.get_width() - 1, image.get_height() - 1);
	vec2 bboxmax(0, 0);
	vec2 clamp(image.get_width() - 1, image.get_height() - 1);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			bboxmin[j] = std::max(0.0, std::min(bboxmin[j], vertPts[i][j]));
			bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], vertPts[i][j]));
		}
	}
	vec3 P;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
			vec3 bc_screen = baryCentric(vertPts, P);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
			P.z = 0;
			for (int i = 0; i < 3; i++) P.z += vertPts[i][2] * bc_screen[i];
			vec2 sample(0, 0);
			for (int i = 0; i < 3; i++) {
				sample.x += (uvPts[i] * bc_screen[i]).x;
				sample.y += (uvPts[i] * bc_screen[i]).y;
			}
			if (zbuffer[int(P.x + P.y * width)] < P.z) {
				zbuffer[int(P.x + P.y * width)] = P.z;
				image.set(P.x, P.y, model->diffuse(sample) * intensity);
			}
		}
	}
}

//void Rasterization::triangle(Model* model,
//							vec3* vertPts, vec2* uvPts,
//							float* zbuffer,
//							TGAImage& image, float* intensityPt) {
//	if (vertPts[0].y == vertPts[1].y && vertPts[0].y == vertPts[2].y) 
//		return;
//
//	vec2 bboxmin(image.get_width() - 1, image.get_height() - 1);
//	vec2 bboxmax(0, 0);
//	vec2 clamp(image.get_width() - 1, image.get_height() - 1);
//
//	for (int i = 0; i < 3; i++) {
//		for (int j = 0; j < 2; j++) {
//			bboxmin[j] = std::max((float)0.0, std::min(bboxmin[j], vertPts[i][j]));
//			bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], vertPts[i][j]));
//		}
//	}
//
//	vec3 P;
//	TGAColor fragColor;
//	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
//		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
//			vec3 bc_screen = baryCentric(vertPts, P);
//			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
//			P.z = 0;
//			vec2 sample;
//			float intensity = 0;
//			for (int i = 0; i < 3; i++) {
//				P.z += vertPts[i][2] * bc_screen[i];
//				intensity += intensityPt[i] * bc_screen[i];
//				sample.x += (uvPts[i] * bc_screen[i]).x;
//				sample.y += (uvPts[i] * bc_screen[i]).y;
//			} 
//
//			bool discard = shader->fragment(sample, intensity, fragColor);
//			if (discard)
//				continue;
//
//			if (zbuffer[int(P.x + P.y * width)] < P.z) {
//				zbuffer[int(P.x + P.y * width)] = P.z;
//				image.set(P.x, P.y, fragColor);
//			}
//		}
//	}
//}

void Rasterization::triangle(vec3* worldPts, vec3* vertPts, vec2* uvPts, vec3* normalPts, mat<3, 3>& TBN, float* zbuffer, TGAImage& image) {
	if (vertPts[0].y == vertPts[1].y && vertPts[0].y == vertPts[2].y)
		return;

	for (int i =0; i<3; i++)
		vertPts[i] = RendererUtil::pixelCorrection(viewport * vec4(vertPts[i]));

	vec2 bboxmin(image.get_width() - 1, image.get_height() - 1);
	vec2 bboxmax(0, 0);
	vec2 clamp(image.get_width() - 1, image.get_height() - 1);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			bboxmin[j] = std::max(0.0, std::min(bboxmin[j], vertPts[i][j]));
			bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], vertPts[i][j]));
		}
	}

	vec3 P;
	TGAColor fragColor;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
			vec3 bc_screen = baryCentric(vertPts, P);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
				continue;

			P.z = 0;
			vec2 uvCoord;
			vec3 bn;
			mat<1, 3> z;
			mat<2, 3> sample_uv;
			mat<3, 3> normal;
			for (int i = 0; i < 3; i++) {
				z[0][i] = vertPts[i][2];
				sample_uv.set_col(i, uvPts[i]);
				normal.set_col(i, normalPts[i]);
			}
			P.z = (z * bc_screen)[0];
			uvCoord = sample_uv * bc_screen;
			bn = normal * bc_screen;

			bn.normalize();
			TBN[0] = (TBN[0] - bn *(bn * TBN[0])).normalize();
			TBN[1] = vec3::cross(bn, TBN[0]).normalize();
			TBN[2] = bn;

			mat<3, 3> AI = mat<3, 3>{ {worldPts[1] - worldPts[0], worldPts[2] - worldPts[0], bn} }.invert();
			vec3 i = AI * vec3((uvPts[1] - uvPts[0]).u, (uvPts[2] - uvPts[0]).u, 0);
			vec3 j = AI * vec3((uvPts[1] - uvPts[0]).v, (uvPts[2] - uvPts[0]).v, 0);

			mat<3, 3> B = mat<3, 3>{ {i.normalize(), j.normalize(), bn} }.transpose();

			shader->fragment(uvCoord, bn, B, fragColor);

			//post process
			if (zbuffer[int(P.x + P.y * width)] < P.z) {
				zbuffer[int(P.x + P.y * width)] = P.z;
				image.set(P.x, P.y, fragColor);
			}
		}
	}
}