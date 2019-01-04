#include "stlloading.h"
#include <string>
#include "Vector3.h"
#include <iostream>
#include <fstream>


Vector3f readVector3D(std::istream& is) {
	Vector3f v;
	std::string line;
	is >> line >> v.x >> v.y >>
		v.z;     // read vertex coords
	getline(is, line);
	return v;
}

// Warning!This method doesn't check the format of the stl file.
StlFile slt_read(const char* name) {

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

	int face_index = 0;
	while (true) {
		file >> line;
		if (line == "endsolid") {
			break;
		}
		Vector3f normal = readVector3D(file);
		getline(file, line);
		Vector3f A = readVector3D(file);
		Vector3f B = readVector3D(file);
		Vector3f C = readVector3D(file);
		EleFace face(normal, A, B, C, 
					 stl.getVector(A), stl.getVector(B), stl.getVector(C), face_index);
		face_index += 1;
		stl.faces.push_back(face);
		getline(file, line);
		getline(file, line);
	}

	std::cout << "Read successfully!";
	file.close();
	return stl;
}

// I am not sure whether we need the index of the vertexes in the stl instance.

EleFace::EleFace(Vector3f normal, Vector3f vertex0, Vector3f vertex1, Vector3f vertex2, int index) {
	EleFace::normal = normal;
	EleFace::vertex0 = vertex0;
	EleFace::vertex1 = vertex1;
	EleFace::vertex2 = vertex2;
	vertex[0] = vertex0;
	vertex[1] = vertex1;
	vertex[2] = vertex2;
	for (int i = 0; i < 3; i++) {
		edge[i].init(vertex[i], vertex[(i + 1) % 3]);
	}
	Vector3f region_min = min(vertex0, vertex1, vertex2);
	Vector3f region_max = max(vertex0, vertex1, vertex2);
	EleFace::region.x = region_min.x;
	EleFace::region.y = region_min.y;
	EleFace::region.z = region_min.z;
	EleFace::region.length = max(region_max - region_min);
	EleFace::region.min = Vector3f(EleFace::region.x, EleFace::region.y, EleFace::region.z);
	EleFace::region.max = Vector3f(EleFace::region.x + EleFace::region.length, EleFace::region.y + EleFace::region.length, EleFace::region.z + EleFace::region.length);
	EleFace::index = index;
	a = -1;
	b = -1;
	c = -1;
}

EleFace::EleFace(Vector3f normal, Vector3f vertex0, Vector3f vertex1, Vector3f vertex2, int index, int a, int b, int c) {
	EleFace::normal = normal;
	EleFace::vertex0 = vertex0;
	EleFace::vertex1 = vertex1;
	EleFace::vertex2 = vertex2;
	vertex[0] = vertex0;
	vertex[1] = vertex1;
	vertex[2] = vertex2;
	for (int i = 0; i < 3; i++) {
		edge[i].init(vertex[i], vertex[(i + 1) % 3]);
	}
	Vector3f region_min = min(vertex0, vertex1, vertex2);
	Vector3f region_max = max(vertex0, vertex1, vertex2);
	EleFace::region.x = region_min.x;
	EleFace::region.y = region_min.y;
	EleFace::region.z = region_min.z;
	EleFace::region.length = max(region_max - region_min);
	EleFace::region.min = Vector3f(EleFace::region.x, EleFace::region.y, EleFace::region.z);
	EleFace::region.max = Vector3f(EleFace::region.x + EleFace::region.length, EleFace::region.y + EleFace::region.length, EleFace::region.z + EleFace::region.length);
	EleFace::index = index;
	EleFace::a = a;
	EleFace::b = b;
	EleFace::c = c;
}
