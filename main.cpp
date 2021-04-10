#include <cmath>
#include <algorithm>
#include "renderer_controller.h"

const int WIDTH = 800;
const int HEIGHT = 800;

int main(int argc, char** argv) {
	TGAImage _outFrame(WIDTH, HEIGHT, TGAImage::RGB);

	RendererController* controller[3] = {NULL};
	if (2 == argc) {
		controller[0] = new RendererController(WIDTH, HEIGHT, argv[1], _outFrame);
		controller[0]->run();
	}
	else {
		controller[0] = new RendererController(WIDTH, HEIGHT, "obj/african_head/african_head", _outFrame);
		controller[0]->run();
		//controller[1] = new RendererController(WIDTH, HEIGHT, "obj/african_head/african_head_eye_inner", _outFrame);
		//controller[2] = new RendererController(WIDTH, HEIGHT, "obj/african_head/african_head_eye_outer", _outFrame);
		//controller[1]->run();
		//controller[2]->run();
	}

	for (int i = 0; i < 3; i++)
	{
		if (controller[i])
			delete controller[i];
	}

	_outFrame.flip_vertically(); // to place the origin in the bottom left corner of the image 
	_outFrame.write_tga_file("framebuffer.tga");

	//getchar();

	return 0;

}