#pragma once
#define NULL 0

#include "Vector3.h"
#include <vector>
#include <algorithm>
#include <unordered_map>
class vector;

class Region {
public:
	float x;
	float y;
	float z;
	float length;
	Vector3f min;
	Vector3f max;
};


namespace std {
	template <> struct hash<Vector3f> {
		std::size_t operator()(const Vector3f& v) const {
			return ((hash<double>()(v.x) ^ (hash<double>()(v.y) << 1)) >> 1) ^ (hash<double>()(v.z) << 1);
		}
	};
}

class Edge {
public:
	Edge() {}
	Edge(Vector3f start_point, Vector3f end_point) {
		start = start_point;
		end = end_point;
	}
	void init(Vector3f start_point, Vector3f end_point) {
		start = start_point;
		end = end_point;
	}
public:
	Vector3f start;
	Vector3f end;
};

class EleFace {
public:
	EleFace(Vector3f normal, Vector3f vertex0, Vector3f vertex1, Vector3f vertex2, int index=0);
	// I am not sure whether we need the index of the vertexes in the stl instance.
	EleFace(Vector3f normal, Vector3f vertex0, Vector3f vertex1, Vector3f vertex2, int index, int a, int b, int c);
	EleFace(){}

public:
	Vector3f normal;
	Vector3f vertex0;
	Vector3f vertex1;
	Vector3f vertex2;
	Vector3f vertex[3];
	Edge edge[3];
	int index;
	int a;
	int b;
	int c;
	Region region;
};


class StlFile {
public:
	std::string header;
	std::vector<Vector3f> vertices;
	std::unordered_map<Vector3f, int> indexMap;
	std::vector<EleFace> faces;
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
	inline Vector3f minVector3f(Vector3f a, Vector3f b)
	{
		return Vector3f(std::min(a.x, b.x),
			std::min(a.y, b.y),
			std::min(a.z, b.z));
	}

	inline Vector3f maxVector3f(Vector3f a, Vector3f b)
	{
		return Vector3f(std::max(a.x, b.x),
			std::max(a.y, b.y),
			std::max(a.z, b.z));
	}

	Vector3f MinCoord() {
		Vector3f minCoord = Vector3f(0, 0, 0);
		for (size_t i = 0; i < vertices.size(); i++)
			minCoord = minVector3f(vertices[i], minCoord);
		return minCoord;
	}

	Vector3f MaxCoord() {
		Vector3f maxCoord = Vector3f(0, 0, 0);
		for (size_t i = 0; i < vertices.size(); i++)
			maxCoord = maxVector3f(vertices[i], maxCoord);
		return maxCoord;
	}



	int getVector(Vector3f v) {
		if (indexMap.count(v)) {
			return indexMap.at(v);
		}
		else {
			int index = (int)vertices.size();
			indexMap.insert({ v, index });
			vertices.push_back(v);
			return index;
		}
	}
};

