#pragma once

#define MAX_ELE_NUM 5
#define NULL 0

#include <set>
#include "stlloading.h"
#include "facepair.h"
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

	//****寻找与储存与该结点相交的另一颗树上的结点***//Jian
	std::vector<Octree*> m_intersected_node;
	bool findIntersectedNode(Octree *nodeB);
	//***************//
	bool isLeaf() //方便外部判断是否为叶子结点
	{
		return m_is_leaf;
	};
	std::set<EleFace*> getEles() { return m_eles; }
	Octree* getSubNodes(int i) { return m_sub_node[i]; }
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

//储存两棵树之间可能相交的两组三角形
class might_intersected_faces
{
public:
	std::set<EleFace*> a_eles;
	std::set<EleFace*> b_eles;
};

//所有可能相交的两组三角形的列表
class might_intersected_faces_list
{
public:
	std::vector<might_intersected_faces*> m_i_f_list;
	might_intersected_faces_list(Octree *nodeA, Octree *nodeB);
	std::vector<vector<Vector3f*>> intersectLine_list;

private:
	bool getMightIntersectedFaces(Octree *nodeA);
	bool fillIintersectLineList();
};