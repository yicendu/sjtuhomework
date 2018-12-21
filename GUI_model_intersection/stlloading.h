#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include "Vector3.h"
#include <cmath>
 #include <algorithm>

#define NULL 0



class Region {
public:
	float x;
	float y;
	float z;
	float length;
};


namespace std {
	template <> struct hash<Vector3f> {
		std::size_t operator()(const Vector3f& v) const {
			return ((hash<double>()(v.x) ^ (hash<double>()(v.y) << 1)) >> 1) ^ (hash<double>()(v.z) << 1);
		}
	};
}


class EleFace {
public:
	EleFace(Vector3f normal, Vector3f vertex0, Vector3f vertex1, Vector3f vertex2);
	// I am not sure whether we need the index of the vertexes in the stl instance.
	EleFace(Vector3f normal, Vector3f vertex0, Vector3f vertex1, Vector3f vertex2, int a, int b, int c);
	EleFace(){}

public:
	Vector3f normal;
	Vector3f vertex0;
	Vector3f vertex1;
	Vector3f vertex2;
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
		Vector3f minCoord = Vector3f(0,0,0);
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



StlFile slt_read(char* name);