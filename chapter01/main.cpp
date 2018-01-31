#include "tgaimage.h"

using namespace std;

const TGAColor white = TGAColor (255, 255, 255, 255);
const TGAColor red   = TGAColor (255, 0,   0,   255);

void draw_line (int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {

	bool inverted = false;

	if (abs(x1-x0) < abs(y1-y0)) {
		inverted = true;
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	for (int x = x0; x <= x1; ++x) {
		float t = (x - x0) / (float) (x1 - x0);
		int y = (1.0f - t) * y0 + t * y1;
		if (inverted) {
			image.set (y, x, color);
		}
		else {
			image.set (x, y, color);
		}
	}
}

int main (int argc, char** argv) {

	TGAImage image (100, 100, TGAImage::RGB);
	for (int i = 0; i < 1000000; ++i) {
		draw_line (13, 20, 80, 40, image, white);
		draw_line (20, 13, 40, 80, image, red);
		draw_line (80, 40, 13, 20, image, red);
	}
	image.flip_vertically ();
	image.write_tga_file ("output.tga");
	return 0;
}