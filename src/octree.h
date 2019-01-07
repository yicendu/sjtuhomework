#pragma once

#define MAX_ELE_NUM 5
#define NULL 0

#include <set>
#include "stlloading.h"
#include "facepair.h"

bool is_region_intersected(Region a, Region b);

// OcTree is a cubic tree. Each length of its eages is the same.
// Example:
//    StlFile stl = StlFile("../models/squarehead.stl");
//    Octree* stl_octree = new Octree(stl, 2.f, 10);
//    ...
//    delete Octree;
class Octree
{
public:
	Octree(int depth, Region region, float min_length, int max_ele_num);
	Octree(StlFile *stl, float min_length, int max_ele_num);
	~Octree();

	void insert_ele(EleFace* ele);
	std::set<EleFace*> query_eles(Region region);
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
	void split_node();
};