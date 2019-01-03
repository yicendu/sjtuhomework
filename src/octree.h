#pragma once

#define MAX_ELE_NUM 5
#define NULL 0

#include <set>
#include "stlloading.h"
#include "facepair.h"
//octree is a cubic tree. Each length of its eages is the same.

bool isRegionIntersected(Region a, Region b);

class Octree
{
public:
	Octree() {};
	Octree(int depth, Region region, float min_length, int max_ele_num);
	Octree(StlFile *stl, float min_length, int max_ele_num);
	~Octree();

	void insertEle(EleFace* ele);
	std::set<EleFace*> queryEles(Region region);
	Region	m_region;
	bool	m_is_leaf;
	Octree*	m_sub_node[8];
	int		m_depth;	// 0 means root
	float	m_min_length;
	int		m_max_ele_num;
	std::set<EleFace*> m_eles;

	std::set<EleFace*> getEles() { return m_eles; }
	Octree* getSubNodes(int i) { return m_sub_node[i]; }
private:
	void splitNode();
};

//储存两棵树之间可能相交的两组三角形

class pair_octree
{
public:
	pair_octree(Octree* A, Octree* B) {
		a = A;
		b = B;
	}
	Octree* a;
	Octree* b;
};