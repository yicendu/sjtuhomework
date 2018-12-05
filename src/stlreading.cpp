#include "stlreading.h"
#include <vector>
#include <iostream>
#include <fstream>

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))


Vector3d readVector3d(std::istream& is) {
	Vector3d v;
	std::string line;
	is >> line >> v.X() >> v.Y() >>
		v.Z();     // read vertex coords
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
		Vector3d normal = readVector3d(file);
		getline(file, line);
		Vector3d A = readVector3d(file);
		Vector3d B = readVector3d(file);
		Vector3d C = readVector3d(file);
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

inline EleFace::EleFace(Vector3d normal, Vector3d vertex0, Vector3d vertex1, Vector3d vertex2) {
	EleFace::normal = normal;
	EleFace::vertex0 = vertex0;
	EleFace::vertex1 = vertex1;
	EleFace::vertex2 = vertex2;
	region.x = min(min(vertex0.X(), vertex1.X()), vertex2.X());
	region.y = min(min(vertex0.Y(), vertex1.Y()), vertex2.Y());
	region.z = min(min(vertex0.Z(), vertex1.Z()), vertex2.Z());
	float x_length = max(max(vertex0.X(), vertex1.X()), vertex2.X()) - region.x;
	float y_length = max(max(vertex0.Y(), vertex1.Y()), vertex2.Y()) - region.y;
	float z_length = max(max(vertex0.Z(), vertex1.Z()), vertex2.Z()) - region.z;
	region.length = max(max(x_length, y_length), z_length);
	a = -1;
	b = -1;
	c = -1;
}

inline EleFace::EleFace(Vector3d normal, Vector3d vertex0, Vector3d vertex1, Vector3d vertex2, int a, int b, int c) {
	EleFace::normal = normal;
	EleFace::vertex0 = vertex0;
	EleFace::vertex1 = vertex1;
	EleFace::vertex2 = vertex2;
	region.x = min(min(vertex0.X(), vertex1.X()), vertex2.X());
	region.y = min(min(vertex0.Y(), vertex1.Y()), vertex2.Y());
	region.z = min(min(vertex0.Z(), vertex1.Z()), vertex2.Z());
	float x_length = max(max(vertex0.X(), vertex1.X()), vertex2.X()) - region.x;
	float y_length = max(max(vertex0.Y(), vertex1.Y()), vertex2.Y()) - region.y;
	float z_length = max(max(vertex0.Z(), vertex1.Z()), vertex2.Z()) - region.z;
	region.length = max(max(x_length, y_length), z_length);
	EleFace::a = a;
	EleFace::b = b;
	EleFace::c = c;
}
