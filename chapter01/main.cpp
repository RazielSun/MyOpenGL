#include "tgaimage.h"

const TGAColor white = TGAColor (255, 255, 255, 255);
const TGAColor red   = TGAColor (255, 0,   0,   255);

void draw_line (int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	
	for (float t = 0.0f; t < 1.0f; t += 0.01f) {
		int x = (1.0f - t) * x0 + t * x1;
		int y = (1.0f - t) * y0 + t * y1;
		image.set (x, y, color);
	}
}

int main (int argc, char** argv) {

	TGAImage image (100, 100, TGAImage::RGB);
	draw_line ( 13, 20, 80, 40, image, white );
	image.flip_vertically ();
	image.write_tga_file ("output.tga");
	return 0;
}