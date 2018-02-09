#include <vector>
#include "tgaimage.h"
#include "model.h"
#include <iostream>
#include <cstdlib> // for rand

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 800;
const int DEPTH = 255;

const TGAColor white = TGAColor (255, 255, 255, 255);
const TGAColor red   = TGAColor (255, 0,   0,   255);
const TGAColor blue  = TGAColor (0, 0,   255,   255);
const TGAColor green = TGAColor (0, 255,   0,   255);

void draw_triangle (Vector3i v0, Vector3i v1, Vector3i v2, TGAImage& image, const TGAColor& color, int* zbuffer) {

	if (v0.y == v1.y && v0.y == v2.y) return;

	if (v0.y > v1.y) swap(v0, v1);
	if (v0.y > v2.y) swap(v0, v2);
	if (v1.y > v2.y) swap(v1, v2);

	int totalH = v2.y - v0.y;
	int oneH = v1.y - v0.y;
	int secondH = v2.y - v1.y;

	for (int i = 0; i < totalH; ++i) {
		int secondPart = i > oneH || v0.y == v1.y;
		int segmentH = secondPart ? secondH : oneH;
		float ka = (float) i / totalH;
		float kb = (float) (i - ((secondPart) ? oneH : 0 )) / segmentH;
		Vector3i A = v0 + Vector3f(v2-v0) * ka;
		Vector3i B = secondPart ? (v1 + Vector3f(v2-v1) * kb) : (v0 + Vector3f(v1-v0) * kb);
		if (A.x>B.x) swap(A, B);

		for (int j = A.x; j <= B.x; j++) {
			float t = A.x == B.x ? 1.0f : (float)(j-A.x)/(float)(B.x-A.x);
			Vector3i p = Vector3f(A) + Vector3f(B-A) * t;
			int idx = p.x + p.y * WIDTH;
			if (idx < WIDTH*HEIGHT && zbuffer[idx] < p.z) {
				zbuffer[idx] = p.z;
				image.set( p.x, p.y, color );
			}
		}
	}
}

int main (int argc, char** argv) {

	Model* model = new Model("obj/african_head.obj");

	int totalZB = WIDTH * HEIGHT;
	int* zbuffer = new int[totalZB];
	for (int i = 0; i < totalZB; i++) {
		zbuffer[i] = numeric_limits<int>::min();
	}

	{	
		TGAImage image (WIDTH, HEIGHT, TGAImage::RGB);

		Vector3f lightDir = Vector3f(0.0f, 0.0f, -1.0f);

	    int facesCount = model->GetFacesCount();
		for (int f = 0; f < facesCount; ++f) {
			vector<int> face = model->GetFace( f );
			Vector3i screenPos[3];
			Vector3f worldPos[3];

			for (int i = 0; i < 3; ++i) {
				Vector3f v = model->GetVertex( face[i] );
				screenPos[i] = Vector3i((v.x+1.0f)*WIDTH*0.5f, (v.y+1.0f)*HEIGHT*0.5f, (v.z+1.0f)*DEPTH*0.5f);	
				worldPos[i] = v;		
			}

			Vector3f normal = (worldPos[2]-worldPos[0])^(worldPos[1]-worldPos[0]);
			normal.normalize();

			float intensity = normal * lightDir;

			if (intensity > 0) {
				draw_triangle(screenPos[0], screenPos[1], screenPos[2], image, TGAColor(intensity*255, intensity*255, intensity*255, 255), zbuffer);
			}		
		}

		image.flip_vertically ();
		image.write_tga_file ("output.tga");
	}

	{
		TGAImage image (WIDTH, HEIGHT, TGAImage::GRAYSCALE);
		for (int i = 0; i < WIDTH; i++) {
			for (int j = 0; j < HEIGHT; j++) {
				image.set(i, j, TGAColor(zbuffer[i + j*WIDTH], 1));
			}
		}
		image.flip_vertically ();
		image.write_tga_file ("zbuffer.tga");
	}

	delete model;
	delete[] zbuffer;
	return 0;
}