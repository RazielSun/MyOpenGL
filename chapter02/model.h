#ifndef _MODEL_H
#define _MODEL_H

#include <vector>
#include "geom.h"

class Model {
private:
	std::vector<Vector3f> vertices;
	std::vector<std::vector<int> > faces;

public:
	Model(const char *filename);
	~Model();

	int GetVerticesCount();
	int GetFacesCount();

	Vector3f GetVertex(int idx);
	std::vector<int> GetFace(int idx);
};

#endif // _MODEL_H