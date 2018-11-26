#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include "octree.h"




Octree::Octree(int depth, Region region, double min_length, int max_ele_num)
{
	m_depth = depth;
	m_region = region;
	m_min_length = min_length;
	m_is_leaf = true;
	memset(m_sub_node, NULL, sizeof(m_sub_node));
	m_max_ele_num = max_ele_num;
}


Octree::~Octree()
{
}

void Octree::splitNode()
{
	m_is_leaf = false;

	double child_lenght = m_region.length / 2;
	double x[2] = { m_region.x, m_region.x + child_lenght };
	double y[2] = { m_region.y, m_region.y + child_lenght };
	double z[2] = { m_region.z, m_region.z + child_lenght };

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

	double child_lenght = m_region.length / 2;
	double mid_x = m_region.x + child_lenght;
	double mid_y = m_region.y + child_lenght;
	double mid_z = m_region.z + child_lenght;
	double x1 = ele->region_x;
	double x2 = x1 + ele->region_length / 2;
	double y1 = ele->region_y;
	double y2 = y1 + ele->region_length / 2;
	double z1 = ele->region_z;
	double z2 = z1 + ele->region_length / 2;

	if (x1 < mid_x) {
		if (y1 < mid_y) {
			if (z1 < mid_z) {
				m_sub_node[0]->insertEle(ele);
			}
			if (z2 > mid_z) {
				m_sub_node[1]->insertEle(ele);
			}
			if (y2 > mid_y) {
				if (z1 < mid_z) {
					m_sub_node[2]->insertEle(ele);
				}
				if (z2 > mid_z) {
					m_sub_node[3]->insertEle(ele);
				}
			}
		}
		if (x2 > mid_x) {
			if (y1 < mid_y) {
				if (z1 < mid_z) {
					m_sub_node[4]->insertEle(ele);
				}
				if (z2 > mid_z) {
					m_sub_node[5]->insertEle(ele);
				}
			}
			if (y2 > mid_y) {
				if (z1 < mid_z) {
					m_sub_node[6]->insertEle(ele);
				}
				if (z2 > mid_z) {
					m_sub_node[7]->insertEle(ele);
				}
			}
		}
	}
}

std::set<EleFace*> Octree::queryEles(Region region) {
	if (m_is_leaf) {
		return m_eles;
	}

	double child_lenght = m_region.length / 2;
	double mid_x = m_region.x + child_lenght;
	double mid_y = m_region.y + child_lenght;
	double mid_z = m_region.z + child_lenght;
	double x1 = region.x;
	double x2 = x1 + region.length;
	double y1 = region.y;
	double y2 = y1 + region.length;
	double z1 = region.z;
	double z2 = z1 + region.length;

	std::set<EleFace*> eles;
	std::set<EleFace*> ele[8];
	if (x1 < mid_x) {
		if (y1 < mid_y) {
			if (z1 < mid_z) {
				ele[0] = m_sub_node[0]->queryEles(region);
			}
			if (z2 > mid_z) {
				ele[1] = m_sub_node[1]->queryEles(region);
			}
		}
		if (y2 > mid_y) {
			if (z1 < mid_z) {
				ele[2] = m_sub_node[2]->queryEles(region);
			}
			if (z2 > mid_z) {
				ele[3] = m_sub_node[3]->queryEles(region);
			}
		}
	}
	if (x2 > mid_x) {
		if (y1 < mid_y) {
			if (z1 < mid_z) {
				ele[4] = m_sub_node[4]->queryEles(region);
			}
			if (z2 > mid_z) {
				ele[5] = m_sub_node[5]->queryEles(region);
			}
		}
		if (y2 > mid_y) {
			if (z1 < mid_z) {
				ele[6] = m_sub_node[6]->queryEles(region);
			}
			if (z2 > mid_z) {
				ele[7] = m_sub_node[7]->queryEles(region);
			}
		}
	}
	std::set<EleFace*> temp;
	for (int i = 0; i < 8; i++) {
		std::set_union(ele[i].begin(), ele[i].end(), eles.begin(), eles.end(), std::inserter(temp,temp.begin()));
		eles = temp;
	}
	return eles;
}