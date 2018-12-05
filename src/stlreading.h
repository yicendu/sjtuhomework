#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include "Vector3.h"

#define NULL 0



class Region {
public:
	float x;//x
	float y;
	float z;
	float length;
};


namespace std {
	template <> struct hash<Vector3d> {
		std::size_t operator()(const Vector3d& v) const {
			return ((hash<double>()(v.X()) ^ (hash<double>()(v.Y()) << 1)) >> 1) ^ (hash<double>()(v.Z()) << 1);
		}
	};
}


class EleFace {
public:
	EleFace(Vector3d normal, Vector3d vertex0, Vector3d vertex1, Vector3d vertex2);
	// I am not sure whether we need the index of the vertexes in the stl instance.
	EleFace(Vector3d normal, Vector3d vertex0, Vector3d vertex1, Vector3d vertex2, int a, int b, int c);
	EleFace(){}

public:
	Vector3d normal;
	Vector3d vertex0;
	Vector3d vertex1;
	Vector3d vertex2;
	int a;
	int b;
	int c;
	Region region;
};

class StlFile {
public:
	std::string header;
	std::vector<Vector3d> vertices;
	std::unordered_map<Vector3d, int> indexMap;
	std::vector<EleFace> faces;
	std::vector<vector<Vector3d*>> intersection;//set of the intersect segment,each intersect segment has two vertices

	int getVector(Vector3d v) {
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
