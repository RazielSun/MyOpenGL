#include <vector>
#include "tgaimage.h"
#include "model.h"

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 800;

const TGAColor white = TGAColor (255, 255, 255, 255);
const TGAColor red   = TGAColor (255, 0,   0,   255);

Model* model = nullptr;

void draw_line (int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color) {

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

	int dx = x1 - x0;
	int dy = y1 - y0;
	int delta = dx * 2;
	int derror = abs(dy) * 2;
	int error = 0;
	int y = y0;

	for (int x = x0; x <= x1; ++x) {
		if (inverted) {
			image.set (y, x, color);
		}
		else {
			image.set (x, y, color);
		}

		error += derror;

		if (error > dx) {
			y += (y0<y1) ? 1 : -1;
			error -= delta;
		}
	}
}

int main (int argc, char** argv) {

	model = new Model("obj/african_head.obj");

	TGAImage image (WIDTH, HEIGHT, TGAImage::RGB);

	int facesCount = model->GetFacesCount();
	for (int f = 0; f < facesCount; ++f) {
		vector<int> face = model->GetFace( f );
		for (int i = 0; i < 3; ++i) {
			Vector3f v0 = model->GetVertex( face[i] );
			Vector3f v1 = model->GetVertex( face[(i+1)%3] );
			int x0 = (v0.x + 1.0f) * WIDTH * 0.5f;
			int y0 = (v0.y + 1.0f) * HEIGHT * 0.5f;
			int x1 = (v1.x + 1.0f) * WIDTH * 0.5f;
			int y1 = (v1.y + 1.0f) * HEIGHT * 0.5f;
			draw_line (x0, y0, x1, y1, image, white);
		}
	}

	image.flip_vertically ();
	image.write_tga_file ("output.tga");

	delete model;
	return 0;
}