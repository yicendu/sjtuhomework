#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include "Vector3.h"

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