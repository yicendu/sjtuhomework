#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include "octree.h"


#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

Octree::Octree(int depth, Region region, float min_length, int max_ele_num)
{
	m_count = 0;
	m_depth = depth;
	m_region = region;
	m_min_length = min_length;
	m_is_leaf = true;
	memset(m_sub_node, NULL, sizeof(m_sub_node));
	m_max_ele_num = max_ele_num;
}

Octree::Octree(StlFile *stl, float min_length, int max_ele_num)
{
	size_t vcount = stl->vertices.size();
	float min_x = stl->vertices[0].x;
	float min_y = stl->vertices[0].y;
	float min_z = stl->vertices[0].z;
	float max_x = min_x;
	float max_y = min_y;
	float max_z = min_z;
	for (int i = 0; i < vcount; i++) {
		if (stl->vertices[i].x < min_x) min_x = stl->vertices[i].x;
		else if (stl->vertices[i].x > max_x) max_x = stl->vertices[i].x;
		if (stl->vertices[i].y < min_y) min_y = stl->vertices[i].y;
		else if (stl->vertices[i].y > max_y) max_y = stl->vertices[i].y;
		if (stl->vertices[i].z < min_z) min_z = stl->vertices[i].z;
		else if (stl->vertices[i].z > max_z) max_z = stl->vertices[i].z;
	}
	float region_length = max(max(max_x - min_x, max_y - min_y), max_z - min_z);
	m_count = 0;
	m_is_leaf = true;
	memset(m_sub_node, NULL, sizeof(m_sub_node));
	m_depth = 0;
	m_min_length = min_length;
	m_max_ele_num = max_ele_num;
	m_region = Region{ min_x, min_y, min_z, region_length };
	size_t fcount = stl->faces.size();
	for (int i = 0; i < fcount; i++) {
		insertEle(&stl->faces[i]);
	}
}


Octree::~Octree()
{
}

void Octree::splitNode()
{
	m_is_leaf = false;

	float child_lenght = m_region.length / 2;
	float x[2] = { m_region.x, m_region.x + child_lenght };
	float y[2] = { m_region.y, m_region.y + child_lenght };
	float z[2] = { m_region.z, m_region.z + child_lenght };

	int child_depth = m_depth + 1;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				m_sub_node[4 * i + 2 * j + k] = new Octree(
					child_depth,
					Region{ x[i], y[j], z[k], child_lenght },
					m_min_length,
					m_max_ele_num);
			}
		}
	}

	std::set<EleFace*>::iterator it;
	for (it = m_eles.begin(); it != m_eles.end(); it++) {
		insertEle(*it);
	}
	m_eles.clear();
}

bool Octree::isIntersected(Region a, Region b)
{
	if ((a.x + a.length) < b.x) return false;
	if ((b.x + b.length) < a.x) return false;
	if ((a.y + a.length) < b.y) return false;
	if ((b.y + b.length) < a.y) return false;
	if ((a.z + a.length) < b.z) return false;
	if ((b.z + b.length) < a.z) return false;
	return true;
}

void Octree::insertEle(EleFace* ele)
{
	if (m_is_leaf) {
		if (m_eles.size() + 1 > m_max_ele_num && m_region.length > m_min_length) {
			splitNode();
			insertEle(ele);
		}
		else {
			m_eles.insert(ele);
		}
		return;
	}

	for (int i = 0; i < 8; i++) {
		if (isIntersected(m_sub_node[i]->m_region, ele->region)) {
			m_sub_node[i]->insertEle(ele);
		}
	}
}

std::set<EleFace*> Octree::queryEles(Region region) {
	if (m_is_leaf) {
		std::set<EleFace*> eles;
		std::set<EleFace*>::iterator ele;
		for (ele = m_eles.begin(); ele != m_eles.end(); ele++) {
			EleFace* face = *ele;
			if (isIntersected(face->region, region)) {
				eles.insert(face);
			}
		}
		return eles;
	}

	std::set<EleFace*> eles;
	std::set<EleFace*> ele[8];

	for (int i = 0; i < 8; i++) {
		if (isIntersected(m_sub_node[i]->m_region, region)) {
			ele[i] = m_sub_node[i]->queryEles(region);
		}
	}

	std::set<EleFace*> temp;
	for (int i = 0; i < 8; i++) {
		std::set_union(ele[i].begin(), ele[i].end(), eles.begin(), eles.end(), std::inserter(temp, temp.begin()));
		eles = temp;
	}
	return eles;
}

bool Octree::findIntersectedNode(Octree *nodeB)
{
	if (!isIntersected(this->m_region, nodeB->m_region))
		return false;
	else
		m_intersected_node.push_back(nodeB);

	if (m_is_leaf && nodeB->m_is_leaf)
		return true;

	if (m_is_leaf && !nodeB->m_is_leaf)
	{
		for (int j = 0; j < 8; j++)
		{
			findIntersectedNode(nodeB->m_sub_node[j]);
		}
	}

	if (!m_is_leaf && nodeB->m_is_leaf)
	{
		for (int i = 0; i < 8; i++)
		{
			m_sub_node[i]->findIntersectedNode(nodeB);
		}
	}

	if (!m_is_leaf && !nodeB->m_is_leaf)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				m_sub_node[i]->findIntersectedNode(nodeB->m_sub_node[j]);
			}
		}
	}
	return true;
}


might_intersected_faces_list::might_intersected_faces_list(Octree *nodeA, Octree *nodeB)
{
	nodeA->findIntersectedNode(nodeB);
	getMightIntersectedFaces(nodeA);
	fillIintersectLineList();
}

bool might_intersected_faces_list::getMightIntersectedFaces(Octree *nodeA)
{

	if (nodeA->isLeaf())
	{
		if (nodeA->getEles().empty())
		{
			return false;
		}
		might_intersected_faces *tmp = new might_intersected_faces();
		tmp->a_eles = nodeA->getEles();
		size_t i = 0;
		for (i = 0; i < nodeA->m_intersected_node.size(); i++)
		{
			if (nodeA->m_intersected_node[i]->isLeaf())
			{
				std::set<EleFace*> tmp_b_eles = nodeA->m_intersected_node[i]->getEles();
				tmp->b_eles.insert(tmp_b_eles.begin(), tmp_b_eles.end());
			}
		}
		m_i_f_list.push_back(tmp);
		return true;
	}
	else
	{
		for (int i = 0; i < 8; i++)
			getMightIntersectedFaces(nodeA->getSubNodes(i));
	}
	return true;
}

bool might_intersected_faces_list::fillIintersectLineList()
{
	size_t i = 0;
	for (i = 0; i < m_i_f_list.size(); i++)
	{
		/*auto j = m_i_f_list[i]->a_eles.begin();*/
		for (auto j = m_i_f_list[i]->a_eles.begin(); j != m_i_f_list[i]->a_eles.end(); j++)
		{
			/*size_t k = 0;*/
			for (auto k = m_i_f_list[i]->b_eles.begin(); k != m_i_f_list[i]->b_eles.end(); k++)
			{
				std::vector<Vector3f*> tmpLine;
				if (cal_intersection(*j, *k, tmpLine))
				intersectLine_list.push_back(tmpLine);
			}
		}
	}
	if (intersectLine_list.empty())
		return false;
	else
		return true;
}