#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <fstream>
#include "geometry.h"

#pragma pack(push,1)
struct TGA_Header {
	char idlength;
	char colormaptype;
	char datatypecode;
	short colormaporigin;
	short colormaplength;
	char colormapdepth;
	short x_origin;
	short y_origin;
	short width;
	short height;
	char  bitsperpixel;
	char  imagedescriptor;
};
#pragma pack(pop)



struct TGAColor {
	union {
		struct {
			unsigned char b, g, r, a;
		};
		unsigned char raw[4];
		unsigned int val;
	};
	int bytespp;

	static TGAColor transform(vec3& vColor) {
		TGAColor tga;
		tga.r = std::min((vColor.r * 255), 255.0);
		tga.g = std::min((vColor.g * 255), 255.0);
		tga.b = std::min((vColor.b * 255), 255.0);

		return tga;
	}

	TGAColor() : val(0), bytespp(1) {
	}

	TGAColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A) : b(B), g(G), r(R), a(A), bytespp(4) {
	}

	TGAColor(int v, int bpp) : val(v), bytespp(bpp) {
	}

	TGAColor(const TGAColor &c) : val(c.val), bytespp(c.bytespp) {
	}

	TGAColor(const unsigned char *p, int bpp) : val(0), bytespp(bpp) {
		for (int i=0; i<bpp; i++) {
			raw[i] = p[i];
		}
	}

	TGAColor & operator =(const TGAColor &c) {
		if (this != &c) {
			bytespp = c.bytespp;
			val = c.val;
		}
		return *this;
	}

	TGAColor& operator *(float intensity) {
		b *= intensity;
		g *= intensity;
		r *= intensity;
		a *= intensity;

		return *this;
	}

	TGAColor& operator +(const TGAColor& c) {
		b += c.b;
		g += c.g;
		r += c.r;
		a += c.a;

		return *this;

	}

	unsigned char operator [](int idx) {
		return raw[idx];
	}

	//TGAColor& operator +(const TGAColor& c) {
	//	b += c.b;
	//	g += c.g;
	//	r += c.r;
	//	a += c.a;

	//	return *this;

	//}
};


class TGAImage {
protected:
	unsigned char* data;
	int width;
	int height;
	int bytespp;

	bool load_rle_data(std::ifstream &in);
	bool unload_rle_data(std::ofstream &out);
public:
	enum Format {
		GRAYSCALE=1, RGB=3, RGBA=4
	};

	TGAImage();
	TGAImage(int w, int h, int bpp);
	TGAImage(const TGAImage &img);
	bool read_tga_file(const char *filename);
	bool write_tga_file(const char *filename, bool rle=true);
	bool flip_horizontally();
	bool flip_vertically();
	bool scale(int w, int h);
	TGAColor get(int x, int y);
	vec3 texture(vec2 uv);
	vec3 texture(float u, float v);
	//TGAColor texture(float u, float v);
	bool set(int x, int y, TGAColor c);
	~TGAImage();
	TGAImage & operator =(const TGAImage &img);
	int get_width();
	int get_height();
	int get_bytespp();
	unsigned char *buffer();
	void clear();
};

#endif //__IMAGE_H__
