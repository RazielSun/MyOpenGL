#include <vector>
#include "tgaimage.h"
#include "model.h"
#include <cstdlib> // for rand

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 800;

const TGAColor white = TGAColor (255, 255, 255, 255);
const TGAColor red   = TGAColor (255, 0,   0,   255);
const TGAColor blue  = TGAColor (0, 0,   255,   255);
const TGAColor green = TGAColor (0, 255,   0,   255);

void draw_line (Vector2i v0, Vector2i v1, TGAImage& image, const TGAColor& color) {

	bool inverted = false;
	if (abs(v1.x-v0.x) < abs(v1.y-v0.y)) {
		inverted = true;
		swap(v0.x, v0.y);
		swap(v1.x, v1.y);
	}

	if (v0.x > v1.x) {
		swap(v0, v1);
	}

	int dx = v1.x - v0.x;
	int dy = v1.y - v0.y;
	int delta = dx * 2;
	int derror = abs(dy) * 2;
	int error = 0;
	int y = v0.y;

	for (int x = v0.x; x <= v1.x; ++x) {
		if (inverted) {
			image.set (y, x, color);
		}
		else {
			image.set (x, y, color);
		}

		error += derror;

		if (error > dx) {
			y += (v0.y<v1.y) ? 1 : -1;
			error -= delta;
		}
	}
}

void draw_triangle (Vector2i v0, Vector2i v1, Vector2i v2, TGAImage& image, const TGAColor& color) {

	if (v0.y == v1.y && v0.y == v2.y) return;

	if (v0.y > v1.y) swap(v0, v1);
	if (v0.y > v2.y) swap(v0, v2);
	if (v1.y > v2.y) swap(v1, v2);

	int totalH = v2.y - v0.y;
	int partOneH = v1.y - v0.y;
	int oneH = partOneH + 1;
	int secondH = v2.y - v1.y + 1;

	for (int i = 0; i <= totalH; ++i) {
		int secondPart = i > partOneH || v0.y == v1.y;
		int segmentH = secondPart ? secondH : oneH;
		float ka = (float) i / totalH;
		float kb = (float) (i - ((secondPart) ? (v1.y - v0.y) : 0 )) / segmentH;
		Vector2i A = v0 + (v2-v0) * ka;
		Vector2i B = secondPart ? (v1 + (v2-v1) * kb) : (v0 + (v1-v0) * kb);
		if (A.x>B.x) swap(A, B);
		for (int x = A.x; x <= B.x; ++x) {
			image.set( x, v0.y + i, color );
		}
	}
}

void rasterize (Vector2i v0, Vector2i v1, TGAImage& image, const TGAColor& color, int ybuffer[]) {

	if (v0.x > v1.x) {
		swap(v0, v1);
	}

	for (int x = v0.x; x <= v1.x; x++) {
		float t = (x - v0.x) / (float)(v1.x - v0.x);
		int y = v0.y * (1.0f-t) + v1.y * t;
		if (ybuffer[x] < y) {
			ybuffer[x] = y;
			for (int j = 0; j < 16; j++) {
				image.set (x, j, color);
			}
		}
	}
}

Vector2i l1[2] = { Vector2i(100,360), Vector2i(320,400) };
Vector2i l2[2] = { Vector2i(20,200), Vector2i(780,400) };
Vector2i l3[2] = { Vector2i(160,400), Vector2i(600,200) };

int main (int argc, char** argv) {
	{
		TGAImage scene (WIDTH, HEIGHT, TGAImage::RGB);

		draw_line(l1[0], l1[1], scene, red);
		draw_line(l2[0], l2[1], scene, green);
		draw_line(l3[0], l3[1], scene, blue);

		draw_line(Vector2i(10,10), Vector2i(790,10), scene, white); // screen line

		scene.flip_vertically ();
		scene.write_tga_file ("scene.tga");
	}
	{
		TGAImage render (WIDTH, 16, TGAImage::RGB);
		int ybuffer[WIDTH];
		for (int i = 0; i < WIDTH; ++i) {
			ybuffer[i] = numeric_limits<int>::min();
		}

		rasterize(l1[0], l1[1], render, red, ybuffer);
		rasterize(l2[0], l2[1], render, green, ybuffer);
		rasterize(l3[0], l3[1], render, blue, ybuffer);

		render.flip_vertically ();
		render.write_tga_file ("render.tga");
	}
	
	return 0;
}