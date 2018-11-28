#pragma once

#define MAX_ELE_NUM 5
#define NULL 0

#include <set>
#include "stlreading.h"
//octree is a cubic tree. Each length of its eages is the same.

class Octree
{
public:
	Octree(int depth, Region region, float min_length, int max_ele_num);
	Octree(StlFile *stl, float min_length, int max_ele_num);
	~Octree();

	void insertEle(EleFace* ele);
	std::set<EleFace*> queryEles(Region region);
	Region	m_region;
private:
	void splitNode();
	bool isIntersected(Region a, Region b);

private:
	int		m_depth;	// 0 means root
	float	m_min_length;
	bool	m_is_leaf;

	Octree*	m_sub_node[8];
	int		m_count;

	int		m_max_ele_num;
	std::set<EleFace*> m_eles;
};
