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

Model* model = nullptr;

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

int main (int argc, char** argv) {

	model = new Model("obj/african_head.obj");

	TGAImage image (WIDTH, HEIGHT, TGAImage::RGB);

	Vector3f lightDir = Vector3f(0.0f, 0.0f, -1.0f);

    int facesCount = model->GetFacesCount();
	for (int f = 0; f < facesCount; ++f) {
		vector<int> face = model->GetFace( f );
		Vector2i screenPos[3];
		Vector3f worldPos[3];

		for (int i = 0; i < 3; ++i) {
			Vector3f v = model->GetVertex( face[i] );
			screenPos[i] = Vector2i((v.x + 1.0f) * WIDTH * 0.5f, (v.y + 1.0f) * HEIGHT * 0.5f);	
			worldPos[i] = v;		
		}

		Vector3f normal = (worldPos[2]-worldPos[0])^(worldPos[1]-worldPos[0]);
		normal.normalize();

		float intensity = normal * lightDir;

		if (intensity > 0) {
			draw_triangle(screenPos[0], screenPos[1], screenPos[2], image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
		}
		
	}

	image.flip_vertically ();
	image.write_tga_file ("output.tga");

	delete model;
	return 0;
}