
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "model.h"

Model::Model(const char *filename) : vertices(), uvs(), normals(), faces() {
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
		else if (line.compare(0, 3, "vt ") == 0) {
			iss >> trash >> trash;
			Vector3f vec;
			for (int i = 0; i < 3; i++) iss >> vec.raw[i];
			uvs.push_back(vec);
		}
		else if (line.compare(0, 3, "vn ") == 0) {
			iss >> trash >> trash;
			Vector3f vec;
			for (int i = 0; i < 3; i++) iss >> vec.raw[i];
			normals.push_back(vec);
		}
		else if (line.compare(0, 2, "f ") == 0) {
			iss >> trash;
			int idx, uvid, nid;
			std::vector<Vector3i> face;
			while(iss >> idx >> trash >> uvid >> trash >> nid) {
				idx --;
				uvid --;
				nid --;
				Vector3i vec (idx, uvid, nid);
				face.push_back(vec);
			}
			faces.push_back(face);
		}
	}

	std::cerr << "Model created | v: " << vertices.size() << " f: " << faces.size() << " vt: " << uvs.size() << " vn: " << normals.size() << "|" << std::endl;

	load_texture(filename, "_diffuse.tga", diffusemap);
}

Model::~Model() {
	//
}

void Model::load_texture(std::string filename, const char* suffix, TGAImage& image) {
	std::string textureFile(filename);
	size_t dot = textureFile.find_last_of(".");
	if (dot != std::string::npos) {
		textureFile = textureFile.substr(0, dot) + std::string(suffix);
		std::cerr << "texture " << textureFile << " loading " << (image.read_tga_file(textureFile.c_str()) ? "ok" : "failed") << std::endl;
		image.flip_vertically();
	}
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

Vector2i Model::GetUV(int idx, int iuv) {
	int id = faces[idx][iuv].iui;
	return Vector2i(uvs[id].x * diffusemap.get_width(), uvs[id].y * diffusemap.get_height());
}

TGAColor Model::GetDiffuse(Vector2i uv) {
	return diffusemap.get(uv.x, uv.y);
}

Vector3f Model::GetNormal(int idx) {
	return normals[idx];
}

std::vector<Vector3i> Model::GetFace(int idx) {
	return faces[idx];
}