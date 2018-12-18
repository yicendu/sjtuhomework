#include "octree.h"
#include "stlloading.h"
#include <iostream>

int main()
{
	Region root_region_a = { 0.f, 0.f, -0.1f, 8.f };
	Region root_region_b = { 1.f, 1.f, -0.1f, 8.f };
	Octree tree_a(0, root_region_a, 0.1f, 5);
	Octree tree_b(0, root_region_b, 0.1f, 5);

	for (int j = 0; j < 8; j++)
	{
		Vector3f normal(0.f, 0.f, 1.f);
		Vector3f vertex[3];
		float i = (float)j;
		float k = i + 0.5f;
		vertex[0] = { i,i,0 };
		vertex[1] = { i,k,0 };
		vertex[2] = { k,i,0 };
		EleFace *tmpEle=new EleFace(normal, vertex[0], vertex[1], vertex[2]);
		tree_a.insertEle(tmpEle);
	}

	for (int j = 1; j < 9; j++)
	{
		Vector3f normal(0, 0, 1);
		Vector3f vertex[3];
		float i = (float)j;
		float k = i + 0.5f;
		vertex[0] = { i,i,0 };
		vertex[1] = { i,k,0 };
		vertex[2] = { k,i,0 };
		EleFace *tmpEle = new EleFace(normal, vertex[0], vertex[1], vertex[2]);
		tree_b.insertEle(tmpEle);
	}
	
	might_intersected_faces_list a_b(&tree_a, &tree_b);
	
	return 0;
}
