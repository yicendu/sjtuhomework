#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include "octree.h"
#include <vector>

//two vertex to edge
Vector3d edge(Vertex *a, Vertex *b) {
	Vector3d *tmp;
	tmp->X() = b->x_value - a->x_value;
	tmp->Y() = b->y_value - a->y_value;
	tmp->Z() = b->z_value - a->z_value;
	return tmp;
}

//judge whether one edge of triangle2 is at the same side of triangle1
//0 for at same side,1 for at different side,2 for on the face
int is_same_side(Vertex *va2, Vertex *vb2, EleFace *t1) {
	Vertex *va1 = t1->vertex0;

	double tmp1 = edge(va1, va2).Dot(t1->normal);
	double tmp2 = edge(va2, vb2).Dot(t1->normal);

	if ((tmp1 * tmp2 > 0) 
		return 0; 
	else if(tmp1==0 && tmp2==0)
		return 2;
	else  
		return 1; 
}

//judege whether one edge of of triangle2 is separate from triangle1
//0 for separation,1 for intersection,2 for on the face
int is_intersection(Vertex *va2, Vertex *vb2, EleFace *t1) {
	Vertex *va1 = t1->vertex0;
	Vertex *vb1 = t1->vertex1;
	Vertex *vc1 = t1->vertex2;
	Vector3d tmp1 = edge(va1, vb1).Cross(edge(va1, va2));
	Vector3d tmp2 = edge(vb1, vc1).Cross(edge(vb1, va2));
	Vector3d tmp3 = edge(vc1, va1).Cross(edge(vc1, va2));
	double g1 = edge(va1, vb2).Dot(tmp1)*edge(va1, vc1).Dot(tmp1);
	double g2 = edge(vb1, vb2).Dot(tmp2)*edge(vb1, va1).Dot(tmp2);
	double g3 = edge(vc1, vb2).Dot(tmp3)*edge(vc1, vb1).Dot(tmp3);
	
	if (g1 > 0 && g2 > 0 && g3 > 0) { return 1; }
	else { return 0; }
}

/*
calculate intersection segment line of t1 and t2;
t1,t2 are two input triangles;
line_section is the vector storing the two point of the intersection segment line of t1 and t2;
*/
void cal_intersection(EleFace *t1, EleFace *t2, vector<Vertex> *line_section) {
	
	Vector3d *nor1 = t1->normal;//normal of triangle1
	Vector3d *nor2 = t2->normal;
	Vertex *va1 = t1->vertex0;
	Vertex *vb1 = t1->vertex1;
	Vertex *vc1 = t1->vertex2;
	Vertex *va2 = t2->vertex0;
	Vertex *vb2 = t2->vertex1;
	Vertex *vc2 = t2->vertex2;
	vector<Vertex> edgeset1;//use start vertex to denote each edge
	vector<Vertex> edgeset2;
	edgeset1[0] = va1;
	edgeset1[1] = vb1;
	edgeset1[2] = vc1;
	edgeset1[3] = va1;
	edgeset2[0] = va2;
	edgeset2[1] = vb2;
	edgeset2[2] = vc2;
	edgeset2[3] = va2;


	/*judge whether t1 intersect with t2;if not,return;*/

	//judge whether each edge of t1 is at same side of t2
	vector<int> is_sd;
	for (int i = 0; i < 3; i++) { is_sd[i] = is_same_side(edgeset1[i], edgeset1[i + 1], t2); }
	if (is_sd[0] == 0 && is_sd[1] == 0 && is_sd[2] == 0) { return; }//three edges is at the same side,don't intersect with t2

	//judge whether each edge of t2 is at same side of t1
	for (int i = 0; i < 3; i++) { is_sd[i+3] = is_same_side(edgeset2[i], edgeset2[i + 1], t1); }
	if (is_sd[3] == 0 && is_sd[4] == 0 && is_sd[5] == 0) { return; }

	//judge each edge of t1 intersect with t2
	vector<int> is_int;
	for (int i = 0; i < 5; i++) {
		if (is_sd[i] == 0) { is_int[i] = 0; }//at the same side, separate
		if (is_sd[i] == 2) { is_int[i] = 2; }//on the face
		if (is_sd[i] == 1) {
			if (i < 3) { is_int[i] = is_intersection(edgeset1[i], edgeset1[i + 1], t2); }//t1 edge
			else { is_int[i] = is_intersection(edgeset2[i], edgeset2[i + 1], t1); }//t2 edge
		}
	}
	if (is_int[0] == 0 && is_int[1] == 0 && is_int[2] == 0) { return; }//three edges don't intersect with t2

	//coplanor1r
	if (nor1->Cross(nor2).L2Norm()==0) {
		return;
	}
	

	/*calculate the intersect point*/
	

}

