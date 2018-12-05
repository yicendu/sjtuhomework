
#include <iostream>
#include "stlreading.h"
#include "octree.h"

void cal_intersection(EleFace *t1, EleFace *t2, vector<vector<Vector3d*>> &line_section);

void main()
{
	EleFace a;
	a.vertex0[0] = 0.0;
	a.vertex0[1] = 0.0;
	a.vertex0[2] = 0.0;
	a.vertex1[0] = 1.0;
	a.vertex1[1] = 0.0;
	a.vertex1[2] = 0.0;
	a.vertex2[0] = 0.0;
	a.vertex2[1] = 1.0;
	a.vertex2[2] = 0.0;
	a.normal[0] = 0.0;
	a.normal[1] = 0.0;
	a.normal[2] = 1.0;
	EleFace b;
	b.vertex0[0] = 0.0;
	b.vertex0[1] = 0.0;
	b.vertex0[2] = 0.0;
	b.vertex1[0] = 1.0;
	b.vertex1[1] = 1.0;
	b.vertex1[2] = 1.0;
	b.vertex2[0] = 1.0;
	b.vertex2[1] = 1.0;
	b.vertex2[2] = -1.0;
	b.normal[0] = 1.0;
	b.normal[1] = -1.0;
	b.normal[2] = 0.0;
	EleFace c;
	c.vertex0[0] = 0.5;
	c.vertex0[1] = 0.0;
	c.vertex0[2] = 0.0;
	c.vertex1[0] = 1.5;
	c.vertex1[1] = 0.0;
	c.vertex1[2] = 0.0;
	c.vertex2[0] = 0.0;
	c.vertex2[1] = 0.0;
	c.vertex2[2] = 1.0;
	c.normal[0] = 0.0;
	c.normal[1] = 1.0;
	c.normal[2] = 0.0;
	vector<vector<Vector3d*>> ans;
	cal_intersection(&a, &c, ans);
	cout << ans[0][0]->X() << endl;
	cout << ans[0][0]->Y() << endl;
	cout << ans[0][0]->Z() << endl;
	cout << ans[0][1]->X() << endl;
	cout << ans[0][1]->Y() << endl;
	cout << ans[0][1]->Z() << endl;
	
	return;
}

