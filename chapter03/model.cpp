
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "model.h"

Model::Model(const char *filename) : vertices(), faces() {
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;

	std::string line;

	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;

		if (line.compare(0, 2, "v ") == 0) {
			iss >> trash;
			Vector3f vec;
			for (int i = 0; i < 3; i++) iss >> vec.raw[i];
			vertices.push_back(vec);
		}
		else if (line.compare(0, 2, "f ") == 0) {
			iss >> trash;
			int idx, itrash;
			std::vector<int> face;
			while(iss >> idx >> trash >> itrash >> trash >> itrash) {
				idx --;
				face.push_back(idx);
			}
			faces.push_back(face);
		}
	}

	std::cerr << "Model created | v: " << vertices.size() << " f: " << faces.size() << "|" << std::endl; 
}

Model::~Model() {
	//
}

int Model::GetVerticesCount() {
	return static_cast<int>(vertices.size());
}

int Model::GetFacesCount() {
	return static_cast<int>(faces.size());
}

Vector3f Model::GetVertex(int idx) {
	return vertices[idx];
}

std::vector<int> Model::GetFace(int idx) {
	return faces[idx];
}