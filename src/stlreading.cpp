#include "stlreading.h"
#include <iostream>
#include <fstream>

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))


Vector3D readVector3D(std::istream& is) {
	Vector3D v;
	std::string line;
	is >> line >> v.x >> v.y >>
		v.z;     // read vertex coords
	getline(is, line);
	return v;
}

// Warning!This method doesn't check the format of the stl file.
StlFile slt_read(char* name) {

	StlFile stl;

	std::ifstream file;
	file.open(name);
	if (!file.is_open()) {
		std::cerr << "Unable to open " << name << std::endl;
		return stl;
	}
	std::cout << "Warning!This stl reading doesn't check the format of the stl file.";

	std::string line;
	getline(file, line);

	while (true) {
		file >> line;
		if (line == "endsolid") {
			break;
		}
		Vector3D normal = readVector3D(file);
		getline(file, line);
		Vector3D A = readVector3D(file);
		Vector3D B = readVector3D(file);
		Vector3D C = readVector3D(file);
		EleFace face(normal, A, B, C, 
					 stl.getVector(A), stl.getVector(B), stl.getVector(C));
		stl.faces.push_back(face);
		getline(file, line);
		getline(file, line);
	}

	std::cout << "Read successfully!";
	file.close();
	return stl;
}

// I am not sure whether we need the index of the vertexes in the stl instance.

inline EleFace::EleFace(Vector3D normal, Vector3D vertex0, Vector3D vertex1, Vector3D vertex2) {
	EleFace::normal = normal;
	EleFace::vertex0 = vertex0;
	EleFace::vertex1 = vertex1;
	EleFace::vertex2 = vertex2;
	region.x = min(min(vertex0.x, vertex1.x), vertex2.x);
	region.y = min(min(vertex0.y, vertex1.y), vertex2.y);
	region.z = min(min(vertex0.z, vertex1.z), vertex2.z);
	float x_length = max(max(vertex0.x, vertex1.x), vertex2.x) - region.x;
	float y_length = max(max(vertex0.y, vertex1.y), vertex2.y) - region.y;
	float z_length = max(max(vertex0.z, vertex1.z), vertex2.z) - region.z;
	region.length = max(max(x_length, y_length), z_length);
	a = -1;
	b = -1;
	c = -1;
}

inline EleFace::EleFace(Vector3D normal, Vector3D vertex0, Vector3D vertex1, Vector3D vertex2, int a, int b, int c) {
	EleFace::normal = normal;
	EleFace::vertex0 = vertex0;
	EleFace::vertex1 = vertex1;
	EleFace::vertex2 = vertex2;
	region.x = min(min(vertex0.x, vertex1.x), vertex2.x);
	region.y = min(min(vertex0.y, vertex1.y), vertex2.y);
	region.z = min(min(vertex0.z, vertex1.z), vertex2.z);
	float x_length = max(max(vertex0.x, vertex1.x), vertex2.x) - region.x;
	float y_length = max(max(vertex0.y, vertex1.y), vertex2.y) - region.y;
	float z_length = max(max(vertex0.z, vertex1.z), vertex2.z) - region.z;
	region.length = max(max(x_length, y_length), z_length);
	EleFace::a = a;
	EleFace::b = b;
	EleFace::c = c;
}
