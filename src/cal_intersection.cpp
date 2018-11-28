#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include "octree.h"
#include <vector>

Vector3d edge(Vertex *a, Vertex *b) {
	Vector3d *tmp;
	tmp->X() = b->x_value - a->x_value;
	tmp->Y() = b->y_value - a->y_value;
	tmp->Z() = b->z_value - a->z_value;
	return tmp;
}

//judge whether one edge of triangle2 is at the same side of triangle1,true for at same side
bool is_same_side(Vertex *va2, Vertex *vb2, EleFace *t1) {
	Vertex *va1 = t1->vertex0;
	
	double t1 = edge(va1, va2).Dot(t1->normal);
	double t2 = edge(va2, vb2).Dot(t1->normal);

	if ((t1 > 0 && t2 > 0) || (t1 < 0 && t2 < 0)) { return true; }
	else { return false; }
}

//judege whether one edge of of triangle2 is separate from triangle1,true for intersection
bool is_intersection(Vertex *va2, Vertex *vb2, EleFace *t1) {
	Vertex *va1 = t1->vertex0;
	Vertex *vb1 = t1->vertex1;
	Vertex *vc1 = t1->vertex2;
	Vector3d tmp1 = edge(va1, vb1).Cross(edge(va1, va2));
	Vector3d tmp2 = edge(vb1, vc1).Cross(edge(vb1, va2));
	Vector3d tmp3 = edge(vc1, va1).Cross(edge(vc1, va2));
	double g1 = edge(va1, vb2).Dot(tmp1)*edge(va1, vc1).Dot(tmp1);
	double g2 = edge(vb1, vb2).Dot(tmp2)*edge(vb1, va1).Dot(tmp2);
	double g3 = edge(vc1, vb2).Dot(tmp3)*edge(vc1, vb1).Dot(tmp3);
	
	if (g1 > 0 && g2 > 0 || g3 > 0) { return true; }
	else { return false; }
}

/*************
calculate intersection segment line of t1 and t2;
t1,t2 are two input triangles;
line_section is the vector storing the two point of the intersection segment line of t1 and t2;
**************/
void cal_intersection(EleFace *t1, EleFace *t2, vector<Vertex> *line_section) {
	
	Vector3d *nor1 = t1->normal;//normal of triangle1
	Vector3d *nor2 = t2->normal;//normal of triangle2
	Vertex *va1 = t1->vertex0;
	Vertex *vb1 = t1->vertex1;
	Vertex *vc1 = t1->vertex2;
	Vertex *va2 = t2->vertex0;
	Vertex *vb2 = t2->vertex1;
	Vertex *vc2 = t2->vertex2;

	/*judge whether t1 intersect with t2;if not,return;*/
	//judge whether each edge of t1 is at same side of t2
	vector<bool> is_sd;
	is_sd[0] = is_same_side(va1, vb1, t2);
	is_sd[1] = is_same_side(vb1, vc1, t2);
	is_sd[2] = is_same_side(vc1, va1, t2);
	if (is_sd[0] == true && is_sd[1] == true && is_sd[2] == true) { return; }//three edges is at the same side,don't intersect with t2
	//judge whether each edge of t2 is at same side of t1
	is_sd[3] = is_same_side(va2, vb2, t1);
	is_sd[4] = is_same_side(vb2, vc2, t1);
	is_sd[5] = is_same_side(vc2, va2, t1);
	if (is_sd[3] == true && is_sd[4] == true && is_sd[5] == true) { return; }
	//judge each edge of t1 intersect with t2
	vector<bool> is_int;
	if (is_sd[0] = true) { is_int[0] = false; }
	else { is_int[0] = is_intersection(va1, vb1, t2); }
	if (is_sd[1] = true) { is_int[1] = false; }
	else { is_int[1] = is_intersection(vb1, vc1, t2); }
	if (is_sd[2] = true) { is_int[2] = false; }
	else { is_int[2] = is_intersection(vc1, va1, t2); }
	if (is_int[0] == false && is_int[1] == false && is_int[2] == false) { return; }//three edges don't intersect with t2
	//coplanor1r
	if (nor1->Cross(nor2).L2Norm()==0) {
		return;
	}
	

	/*calculate the intersect point*/

	dir = t1->normal->Cross(fb->normal);//direction of intersection line

}

