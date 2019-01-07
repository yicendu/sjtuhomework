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

StlFile::~StlFile() {
	m_vertices.clear();
	m_faces.clear();
}


// Warning!This method doesn't check the format of the stl file.
int StlFile::stl_read(const char* name) {

	std::ifstream file;
	file.open(name);
	if (!file.is_open()) {
		std::cerr << "Unable to open " << name << std::endl;
		file.close();
		return 0;
	}
	std::string line;
	getline(file, line);
	if (line.compare ("solid 0")!=0) {
		std::cerr << "The file \"" << name << "\" format is wrong!" << std::endl;
		file.close();
		return -1;
	}
	std::cout << line;
	while (true) {
		file >> line;

		if (line.compare("endsolid")==0) {
			break;
		}
		Vector3f normal = readVector3D(file);
		getline(file, line);
		Vector3f A = readVector3D(file);
		Vector3f B = readVector3D(file);
		Vector3f C = readVector3D(file);
		m_vertices.push_back(A);
		m_vertices.push_back(B);
		m_vertices.push_back(C);
		EleFace face(normal, A, B, C);
		m_faces.push_back(face);
		getline(file, line);
		getline(file, line);
	}

	std::cout << "Read successfully!" << std::endl;
	file.close();
	_isopen = true;
	return 1;
}

// I am not sure whether we need the index of the vertexes in the stl instance.

EleFace::EleFace(Vector3f normal, Vector3f vertex0, Vector3f vertex1, Vector3f vertex2) {
	EleFace::m_normal = normal;
	EleFace::m_vertex0 = vertex0;
	EleFace::m_vertex1 = vertex1;
	EleFace::m_vertex2 = vertex2;
	m_vertex[0] = vertex0;
	m_vertex[1] = vertex1;
	m_vertex[2] = vertex2;
	for (int i = 0; i < 3; i++) {
		m_edge[i]= Edge(m_vertex[i], m_vertex[(i + 1) % 3]);
	}
	Vector3f region_min = min(vertex0, vertex1, vertex2);
	Vector3f region_max = max(vertex0, vertex1, vertex2);
	EleFace::m_region.m_x = region_min.x;
	EleFace::m_region.m_y = region_min.y;
	EleFace::m_region.m_z = region_min.z;
	EleFace::m_region.m_length = max(region_max - region_min);
	EleFace::m_region.m_min = Vector3f(EleFace::m_region.m_x, EleFace::m_region.m_y, EleFace::m_region.m_z);
	EleFace::m_region.m_max = Vector3f(EleFace::m_region.m_x + EleFace::m_region.m_length, EleFace::m_region.m_y + EleFace::m_region.m_length, EleFace::m_region.m_z + EleFace::m_region.m_length);
}