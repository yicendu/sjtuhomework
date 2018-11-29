#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#define NULL 0

class Vector3D {
public:
	float x;
	float y;
	float z;

public:
	bool operator==(const Vector3D& other) const {
		return other.x == x && other.y == y && other.z == z;
	}
};

class Region {
public:
	float x;//x
	float y;
	float z;
	float length;
};


namespace std {
	template <> struct hash<Vector3D> {
		std::size_t operator()(const Vector3D& v) const {
			return ((hash<float>()(v.x) ^ (hash<float>()(v.y) << 1)) >> 1) ^ (hash<float>()(v.z) << 1);
		}
	};
}


class EleFace {
public:
	EleFace(Vector3D normal, Vector3D vertex0, Vector3D vertex1, Vector3D vertex2);
	// I am not sure whether we need the index of the vertexes in the stl instance.
	EleFace(Vector3D normal, Vector3D vertex0, Vector3D vertex1, Vector3D vertex2, int a, int b, int c);
	EleFace(){}

public:
	Vector3D normal;
	Vector3D vertex0;
	Vector3D vertex1;
	Vector3D vertex2;
	int a;
	int b;
	int c;
	Region region;
};

class StlFile {
public:
	std::string header;
	std::vector<Vector3D> vertices;
	std::unordered_map<Vector3D, int> indexMap;
	std::vector<EleFace> faces;

	int getVector(Vector3D v) {
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
