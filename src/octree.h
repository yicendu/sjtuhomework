#ifndef OCT_TREE_H
#define OCT_TREE_H

#define MAX_ELE_NUM 5
#define NULL 0

#include <set>
#include "vector3.h"
//octree is a cubic tree. Each length of its eages is the same.
struct Region {
	double x;//x
	double y;
	double z;
	double length;
};

struct Vertex {
	double x_value;
	double y_value;
	double z_value;
};

struct EleFace {
	struct Vertex* vertex0;
	struct Vertex* vertex1;
	struct Vertex* vertex2;
	Vector3d* normal;
	double region_x;//x
	double region_y;
	double region_z;
	double region_length;
};

struct EleChain {
	struct EleChain* front;
	struct EleChain* rear;
	struct EleFace* data;

};


class Octree
{
public:
	Octree(int depth, Region region, double min_length, int max_ele_num);
	~Octree();

	void insertEle(EleFace* ele);
	std::set<EleFace*> queryEles(Region region);

private:
	void splitNode();

private:
	int		m_depth;	// 0 means root
	double	m_min_length;
	bool	m_is_leaf;
	Region	m_region;
	Octree*	m_sub_node[8];

	int		m_max_ele_num;
	std::set<EleFace*> m_eles;
};
#endif