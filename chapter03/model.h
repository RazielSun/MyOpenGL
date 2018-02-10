#ifndef _MODEL_H
#define _MODEL_H

#include <vector>
#include <string>
#include "tgaimage.h"
#include "geom.h"

class Model {
private:
	std::vector<Vector3f> vertices;
	std::vector<Vector3f> uvs;
	std::vector<Vector3f> normals;
	std::vector<std::vector<Vector3i> > faces;

	TGAImage diffusemap;

public:
	Model(const char *filename);
	~Model();

	int GetVerticesCount();
	int GetFacesCount();

	Vector3f GetVertex(int idx);
	Vector2i GetUV(int idx, int iuv);
	Vector3f GetNormal(int idx);
	std::vector<Vector3i> GetFace(int idx);
	TGAColor GetDiffuse(Vector2i uv);

	void load_texture(std::string filename, const char* suffix, TGAImage& image);
};

#endif // _MODEL_H