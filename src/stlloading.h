#pragma once
#define NULL 0

#include "Vector3.h"
#include <vector>
#include <algorithm>
#include <unordered_map>
class vector;

// The basic element of EleFace, Octree.
class Region {
public:
	Region(float x, float y, float z, float length) {
		this->m_x = x;
		this->m_y = y;
		this->m_z = z;
		this->m_length = length;
		m_min = Vector3f(x, y, z);
		m_max = Vector3f(x + length, y + length, z + length);
	}
	Region() {}
	float m_x;
	float m_y;
	float m_z;
	float m_length;
	Vector3f m_min;
	Vector3f m_max;
};


namespace std {
	template <> struct hash<Vector3f> {
		std::size_t operator()(const Vector3f& v) const {
			return ((hash<double>()(v.x) ^ (hash<double>()(v.y) << 1)) >> 1) ^ (hash<double>()(v.z) << 1);
		}
	};
}

// The basic element of EleFace.
// It's the half edge of the triangle.
class Edge {
public:
	Edge() {}
	Edge(Vector3f start_point, Vector3f end_point) {
		m_start = start_point;
		m_end = end_point;
	}

public:
	Vector3f m_start;
	Vector3f m_end;
};

// The basic element of Octree
class EleFace {
public:
	EleFace(Vector3f normal, Vector3f vertex0, Vector3f vertex1, Vector3f vertex2);

	Vector3f m_normal;
	Vector3f m_vertex0;
	Vector3f m_vertex1;
	Vector3f m_vertex2;
	Vector3f m_vertex[3];
	Edge m_edge[3];
	int m_index;
	float m_color[4] = { 0.4f,0.4f,1.0f,0.8f };
	Region m_region;
};


// StlFile is used for reading Ascii-format 3d object. 
// Not fully developed.
// Example:
//    StlFile stl;
//	  if(stl.read("../models/squarehead.stl")==0) {
//		//file was successfully readed
//    }
class StlFile {
public:
	std::string m_header;
	std::vector<Vector3f> m_vertices;
	std::vector<EleFace> m_faces;
	~StlFile();
private:
	bool _isopen;
public:
	/* If the file is not stl standard file in ASCII format,
	*  it will cause error in open a file.
    *  return 1, if everything is right.
	*  return 0, if there is no such a file.
	*  return -1, if the format is not right.
	*/
	int stl_read(const char* name);
	StlFile() {
		_isopen = false;
	}

	bool isopen() {
		return _isopen;
	}
};