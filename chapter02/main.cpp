#include <vector>
#include "tgaimage.h"
#include "geom.h"

using namespace std;

const int WIDTH = 200;
const int HEIGHT = 200;

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
		swap(v0.x, v1.x);
		swap(v0.y, v1.y);
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
		int secondPart = i >= partOneH;
		int segmentH = secondPart ? secondH : oneH;
		float ka = (float) i / totalH;
		float kb = (float) (i - ((secondPart) ? (v1.y - v0.y) : 0 )) / segmentH;
		Vector2i A = v0 + (v2-v0) * ka;
		Vector2i B = secondPart ? (v1 + (v2-v1) * kb) : (v0 + (v1-v0) * kb);
		if (A.x>B.x) swap(A, B);
		for (int x = A.x; x <= B.x; ++x) {
			image.set( x, v0.y + i, white );
		}
	}

	// int segmentH = v1.y - v0.y + 1;

	// for (int y = v0.y; y <= v1.y; ++y) {
	// 	float ka = (float) (y - v0.y) / totalH;
	// 	float kb = (float) (y - v0.y) / segmentH;
	// 	Vector2i A = v0 + (v2-v0) * ka;
	// 	Vector2i B = v0 + (v1-v0) * kb;
	// 	if (A.x>B.x) swap(A, B);
	// 	for (int x = A.x; x <= B.x; ++x) {
	// 		image.set( x, y, white );
	// 	}
	// 	image.set (A.x, y, red);
	// 	image.set (B.x, y, green);
	// }

	// segmentH = v2.y - v1.y + 1;

	// for (int y = v1.y; y <= v2.y; ++y) {
	// 	float ka = (float) (y - v0.y) / totalH;
	// 	float kb = (float) (y - v1.y) / segmentH;
	// 	Vector2i A = v0 + (v2-v0) * ka;
	// 	Vector2i B = v1 + (v2-v1) * kb;
	// 	if (A.x>B.x) swap(A, B);
	// 	for (int x = A.x; x <= B.x; ++x) {
	// 		image.set( x, y, white );
	// 	}
	// 	image.set (A.x, y, red);
	// 	image.set (B.x, y, green);
	// }
}

int main (int argc, char** argv) {

	TGAImage image (WIDTH, HEIGHT, TGAImage::RGB);

	Vector2i t0[3] = {Vector2i(10, 70),   Vector2i(50, 160),  Vector2i(70, 80)};
    Vector2i t1[3] = {Vector2i(180, 50),  Vector2i(150, 1),   Vector2i(70, 180)};
    Vector2i t2[3] = {Vector2i(180, 150), Vector2i(120, 160), Vector2i(130, 180)};

    draw_triangle (t0[0], t0[1], t0[2], image, red);
    draw_triangle (t1[0], t1[1], t1[2], image, blue);
    draw_triangle (t2[0], t2[1], t2[2], image, green);

	image.flip_vertically ();
	image.write_tga_file ("output.tga");

	return 0;
}