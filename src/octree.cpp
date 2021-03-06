#include "octree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <iterator>


#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

bool is_region_intersected(Region a, Region b)
{
	if ((a.m_x + a.m_length) <= b.m_x) return false;
	if ((b.m_x + b.m_length) <= a.m_x) return false;
	if ((a.m_y + a.m_length) <= b.m_y) return false;
	if ((b.m_y + b.m_length) <= a.m_y) return false;
	if ((a.m_z + a.m_length) <= b.m_z) return false;
	if ((b.m_z + b.m_length) <= a.m_z) return false;
	return true;
}

Octree::Octree(int depth, Region region, float min_length, int max_ele_num)
{
	m_depth = depth;
	m_region = region;
	m_min_length = min_length;
	m_is_leaf = true;
	memset(m_sub_node, NULL, sizeof(m_sub_node));
	m_max_ele_num = max_ele_num;
}

Octree::Octree(StlFile *stl, float min_length, int max_ele_num)
{
	size_t vcount = stl->m_vertices.size();
	float min_x = stl->m_vertices[0].x;
	float min_y = stl->m_vertices[0].y;
	float min_z = stl->m_vertices[0].z;
	float max_x = min_x;
	float max_y = min_y;
	float max_z = min_z;
	for (int i = 0; i < vcount; i++) {
		if (stl->m_vertices[i].x < min_x) min_x = stl->m_vertices[i].x;
		else if (stl->m_vertices[i].x > max_x) max_x = stl->m_vertices[i].x;
		if (stl->m_vertices[i].y < min_y) min_y = stl->m_vertices[i].y;
		else if (stl->m_vertices[i].y > max_y) max_y = stl->m_vertices[i].y;
		if (stl->m_vertices[i].z < min_z) min_z = stl->m_vertices[i].z;
		else if (stl->m_vertices[i].z > max_z) max_z = stl->m_vertices[i].z;
	}
	float region_length = max(max(max_x - min_x, max_y - min_y), max_z - min_z);

	m_is_leaf = true;
	memset(m_sub_node, NULL, sizeof(m_sub_node));
	m_depth = 0;
	m_min_length = min_length;
	m_max_ele_num = max_ele_num;
	m_region = Region(min_x, min_y, min_z, region_length);
	size_t fcount = stl->m_faces.size();
	for (int i = 0; i < fcount; i++) {
		insert_ele(&stl->m_faces[i]);
	}
}


Octree::~Octree()
{
	if (m_is_leaf) {
		return;
	}
	else {
		for (int i = 0; i < 8; i++) {
			delete m_sub_node[i];
		}
	}
}

void Octree::split_node()
{
	m_is_leaf = false;

	float child_lenght = m_region.m_length / 2;
	float x[2] = { m_region.m_x, m_region.m_x + child_lenght };
	float y[2] = { m_region.m_y, m_region.m_y + child_lenght };
	float z[2] = { m_region.m_z, m_region.m_z + child_lenght };

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
		insert_ele(*it);
	}
	m_eles.clear();
}



void Octree::insert_ele(EleFace* ele)
{
	if (m_is_leaf) {
		if (m_eles.size() + 1 > m_max_ele_num && m_region.m_length > m_min_length) {
			split_node();
			insert_ele(ele);
		}
		else {
			m_eles.insert(ele);
		}
		return;
	}

	for (int i = 0; i < 8; i++) {
		if (is_region_intersected(m_sub_node[i]->m_region, ele->m_region)) {
			m_sub_node[i]->insert_ele(ele);
		}
	}
}

std::set<EleFace*> Octree::query_eles(Region region) {
	if (m_is_leaf) {
		std::set<EleFace*> eles;
		std::set<EleFace*>::iterator ele;
		for (ele = m_eles.begin(); ele != m_eles.end(); ele++) {
			EleFace* face = *ele;
			if (is_region_intersected(face->m_region, region)) {
				eles.insert(face);
			}
		}
		return eles;
	}

	std::set<EleFace*> eles;
	std::set<EleFace*> ele[8];

	for (int i = 0; i < 8; i++) {
		if (is_region_intersected(m_sub_node[i]->m_region, region)) {
			ele[i] = m_sub_node[i]->query_eles(region);
		}
	}

	std::set<EleFace*> temp;
	for (int i = 0; i < 8; i++) {
		std::set_union(ele[i].begin(), ele[i].end(), eles.begin(), eles.end(), std::inserter(temp, temp.begin()));
		eles = temp;
	}
	return eles;
}
