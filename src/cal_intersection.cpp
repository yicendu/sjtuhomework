#include "pch.h"
#include <iostream>
#include "octree.h"
#include "vector3.h"
#include <vector>
#include <algorithm>

//two vertex to vector3d
Vector3d edge(Vertex *a, Vertex *b) {
	Vector3d tmp;
	tmp[0] = b->x_value - a->x_value;
	tmp[1] = b->y_value - a->y_value;
	tmp[2] = b->z_value - a->z_value;
	return tmp;
}

//copy vertex1's value to vertex2
void v_assign(Vertex *v1, Vertex *v2) {
	v2->x_value = v1->x_value;
	v2->y_value = v1->y_value;
	v2->z_value = v1->z_value;
	return;
}

//judge whether one edge of triangle2 is at the same side of triangle1
//0 for at same side,1 for at different side(include one vertex on face),2 for 2 vertices on the face
int is_same_side(Vertex *va_2, Vertex *vb_2, EleFace *tria) {
	Vertex *va_1 = tria->vertex0;
	Vector3d nor = tria->normal;
	double tmp1 = edge(va_1, va_2).Dot(nor);
	double tmp2 = edge(va_1, vb_2).Dot(nor);

	if ((tmp1 * tmp2 > 0))
		return 0;
	else if (tmp1 == 0 && tmp2 == 0)
		return 2;
	else
		return 1;
}

//judege whether one edge of of triangle2 is separate from triangle1
//0 for separation(include one vertex on edge),1 for intersection,2 for on the face(not judeged in this function)
int is_intersection(Vertex *va_2, Vertex *vb_2, EleFace *tria) {
	Vertex *va_1 = tria->vertex0;
	Vertex *vb_1 = tria->vertex1;
	Vertex *vc_1 = tria->vertex2;
	Vector3d tmp1 = edge(va_1, vb_1).Cross(edge(va_1, va_2));
	Vector3d tmp2 = edge(vb_1, vc_1).Cross(edge(vb_1, va_2));
	Vector3d tmp3 = edge(vc_1, va_1).Cross(edge(vc_1, va_2));
	double g1 = (edge(va_1, vb_2).Dot(tmp1)) * (edge(va_1, vc_1).Dot(tmp1));
	double g2 = (edge(vb_1, vb_2).Dot(tmp2)) * (edge(vb_1, va_1).Dot(tmp2));
	double g3 = (edge(vc_1, vb_2).Dot(tmp3)) * (edge(vc_1, vb_1).Dot(tmp3));

	if (g1 > 0 && g2 > 0 && g3 > 0) { return 1; }
	else { return 0; }
}

//calculate Determinant |A|
double cal_determinant(double matrix[3][3]) {
	double det = matrix[0][0] * matrix[1][1] * matrix[2][2] + matrix[1][0] * matrix[2][1] * matrix[0][2] + matrix[2][0] * matrix[0][1] * matrix[1][2];
	det = det - (matrix[0][2] * matrix[1][1] * matrix[2][0] + matrix[0][0] * matrix[1][2] * matrix[2][1] + matrix[0][1] * matrix[1][0] * matrix[2][2]);
	return det;
}

//solve equation
bool solve_equation(double matrix_A[3][3], double array_b[3], double array_x[3]) {
	double base_D = cal_determinant(matrix_A);
	if (base_D != 0) {
		double D[3];//derterminant of A1,A2,A3

		//for (int i = 0; i < 3; i++) {
		//	double matrix_tmp[3][3];//copy A to tmp
		//	for (int j = 0; j < 3; j++) {
		//		for (int k = 0; k < 3; k++) { matrix_tmp[j][k] = matrix_A[j][k]; }
		//	}
		//	for (int j = 0; j < 3; j++) { matrix_tmp[j][i] = array_b[j]; }//change col
		//	D[i] = cal_determinant(matrix_tmp);
		//}
		//for (int i = 0; i < 3; i++) { array_x[i] = D[i] / base_D; }

		//only solve b
		for (int j = 0; j < 3; j++) {
			matrix_A[j][2] = array_b[j];//change col
		}
		D[2] = cal_determinant(matrix_A);
		array_x[2] = D[2] / base_D;
		return true;
	}
	else { return false; }//no answer
}

//calculate intersect point of edge p1p2 and face tria,put intersect point in answer
bool calculate_intersect_point(Vertex *p1, Vertex *p2, EleFace *tria, vector<Vertex*> &answer) {
	Vector3d edge_p = edge(p1, p2);
	Vector3d edge_q1 = edge(tria->vertex0, tria->vertex1);
	Vector3d edge_q2 = edge(tria->vertex0, tria->vertex2);
	Vertex *v_q = tria->vertex0;
	Vertex *v_p = p1;
	//b=vertex p -vertex q
	double arr_b[3];
	arr_b[0] = v_p->x_value - v_q->x_value;
	arr_b[1] = v_p->y_value - v_q->y_value;
	arr_b[2] = v_p->z_value - v_q->z_value;
	//A=[e_q1,e_q2,-e_p]
	double mat_A[3][3];
	for (int i = 0; i < 3; i++) {
		mat_A[i][0] = edge_q1[i];
		mat_A[i][1] = edge_q2[i];
		mat_A[i][2] = -edge_p[i];
	}
	//v_q0+ a1*e_q1+a2*e_q2=v_p+b*e_p
	//x=[a1,a2,b]
	double ans_x[3];
	if (solve_equation(mat_A, arr_b, ans_x) == true) {
		answer.push_back(new Vertex);
		answer.back()->x_value = v_p->x_value + ans_x[2] * edge_p.X();
		answer.back()->y_value = v_p->y_value + ans_x[2] * edge_p.Y();
		answer.back()->z_value = v_p->z_value + ans_x[2] * edge_p.Z();
		return true;
	}
	else { return false; }
}

/*two edges are collinear*/
bool cal_intersect_point_collinear(Vertex *p1, Vertex *p2, Vertex *q1, Vertex *q2, vector<Vertex*> &answer) {
	//fast judge separate
	if ((max(p1->x_value, p2->x_value) < min(p1->x_value, p2->x_value)) || (min(p1->x_value, p2->x_value) > max(p1->x_value, p2->x_value))) { return 0; }
	if ((max(p1->y_value, p2->y_value) < min(p1->y_value, p2->y_value)) || (min(p1->y_value, p2->y_value) > max(p1->y_value, p2->y_value))) { return 0; }
	if ((max(p1->z_value, p2->z_value) < min(p1->z_value, p2->z_value)) || (min(p1->z_value, p2->z_value) > max(p1->z_value, p2->z_value))) { return 0; }

	Vector3d tmp = edge(p1, p2);
	vector<Vertex*> tmpv;
	tmpv.push_back(p1);
	tmpv.push_back(p2);
	tmpv.push_back(q1);
	tmpv.push_back(q2);
	double t[4];
	double tmax = 0;
	double tmin = 0;
	for (int i = 0; i < 4; i++) {
		t[i] = edge(p1, tmpv[i]).Dot(tmp);
		if (t[i] != 0) { t[i] = t[i] / abs(t[i])*edge(p1, tmpv[i]).L2Norm(); }
		tmax = max(tmax, t[i]);
		tmin = min(tmin, t[i]);
	}
	for (int i = 0; i < 2; i++) {
		for (int j = i + 2; j < 4; j++) {
			if (t[i] == t[j]) {
				answer.push_back(new Vertex);
				v_assign(tmpv[i], answer.back());
			}
		}
	}
	if (answer.size() == 2) { return 1; }
	for (int i = 0; i < 4; i++) {
		if (t[i]<tmax && t[i]>tmin) {
			answer.push_back(new Vertex);
			v_assign(tmpv[i], answer.back());
		}
		if (answer.size() == 2) { return 1; }
	}

}

/*one edge and a triangle is colplanar*/
//judge whether two point of a segment are line at same side of another segment line
//0 for cross,1 for one side,2 for one vertex on segment
int judge_same_side(Vertex *p1, Vertex *p2, Vertex *q1, Vertex *q2) {
	double tmp = (edge(q1, p1).Cross(edge(q1, q2))).Dot(edge(q1, p2).Cross(edge(q1, q2)));
	if (tmp > 0) { return 1; }
	else if (tmp == 0) { return 2; }
	else { return 0; }
}

//judge whether two segment lines intersect
//0 for separate,1 for intersect,2 for same line,3 for intersect point is segment vertex
int judge_intersect(Vertex *p1, Vertex *p2, Vertex *q1, Vertex *q2) {
	//fast judge separate
	if ((max(p1->x_value, p2->x_value) < min(p1->x_value, p2->x_value)) || (min(p1->x_value, p2->x_value) > max(p1->x_value, p2->x_value))) { return 0; }
	if ((max(p1->y_value, p2->y_value) < min(p1->y_value, p2->y_value)) || (min(p1->y_value, p2->y_value) > max(p1->y_value, p2->y_value))) { return 0; }
	if ((max(p1->z_value, p2->z_value) < min(p1->z_value, p2->z_value)) || (min(p1->z_value, p2->z_value) > max(p1->z_value, p2->z_value))) { return 0; }

	Vector3d edge_p = edge(p1, p2);
	Vector3d edge_q = edge(q1, q2);
	if (edge_p.Cross(edge_q).Iszero() == true) {
		if ((edge(p1, q1).Cross(edge(p2, q2)).Iszero() == true)) { return 2; } // on the same line
		else { return 0; }//parallel
	}
	else {
		int tmp1 = judge_same_side(p1, p2, q1, q2);
		int tmp2 = judge_same_side(q1, q2, p1, p2);
		if (tmp1 == 2 && tmp2 == 2) { return 3; } //intersect point is segment vertex
		else if ((tmp1 == 0 && tmp2 == 0) || (tmp1 == 2 && tmp2 == 0) || (tmp2 == 2 && tmp2 == 0)) { return 1; }
		else { return 0; }
	}
}

//calculate determinant 2x2
double cal_determinant_2(double a, double b, double c, double d, double e, double f) {
	double det = a * d - b * c;
	if (det == 0) { return 2; }//2 for no answer
	else { return (e * d - b * f) / det; }
}

//calculate intersect point of two conlplanar segment lines
void cal_intersect_point_segment(Vertex *p1, Vertex *p2, Vertex *q1, Vertex *q2, vector<Vertex*> &answer) {
	Vector3d edge_p = edge(p1, p2);
	Vector3d edge_q = edge(q1, q2);
	double matrix_A[3][2];
	double array_b[3];
	array_b[0] = q1->x_value - p1->x_value;
	array_b[1] = q1->y_value - p1->y_value;
	array_b[2] = q1->z_value - p1->z_value;
	for (int i = 0; i < 3; i++) {
		matrix_A[i][0] = edge_p[i];
		matrix_A[i][1] = -edge_q[i];
	}
	double t;//answer
	t = cal_determinant_2(matrix_A[0][0], matrix_A[0][1], matrix_A[1][0], matrix_A[1][1], array_b[0], array_b[1]);
	if (t == 2) { t = cal_determinant_2(matrix_A[1][0], matrix_A[1][1], matrix_A[2][0], matrix_A[2][1], array_b[1], array_b[2]); }
	if (t == 2) { t = cal_determinant_2(matrix_A[2][0], matrix_A[2][1], matrix_A[0][0], matrix_A[0][1], array_b[2], array_b[0]); }

	answer.push_back(new Vertex);
	answer.back()->x_value = p1->x_value + t * edge_p[0];
	answer.back()->y_value = p1->y_value + t * edge_p[1];
	answer.back()->z_value = p1->z_value + t * edge_p[2];

	return;
}

//calculate intersect point when edge p1p2 is on face tria
bool calculate_intersect_point_colplanar(Vertex *p1, Vertex *p2, EleFace *tria, vector<Vertex*> &answer) {
	int a = judge_intersect(p1, p2, tria->vertex0, tria->vertex1);
	int b = judge_intersect(p1, p2, tria->vertex1, tria->vertex2);
	int c = judge_intersect(p1, p2, tria->vertex2, tria->vertex0);
	if (a == 0 && b == 0 && c == 0) { return false; }
	//intersect on segment vertex
	if (a == 3 && b == 3) { answer.push_back(new Vertex); v_assign(tria->vertex1, answer.back()); }
	if (b == 3 && c == 3) { answer.push_back(new Vertex); v_assign(tria->vertex2, answer.back()); }
	if (a == 3 && c == 3) { answer.push_back(new Vertex); v_assign(tria->vertex0, answer.back()); }
	// cross intersect
	if (a == 1) { cal_intersect_point_segment(p1, p2, tria->vertex0, tria->vertex1, answer); }
	if (b == 1) { cal_intersect_point_segment(p1, p2, tria->vertex1, tria->vertex2, answer); }
	if (c == 1) { cal_intersect_point_segment(p1, p2, tria->vertex2, tria->vertex0, answer); }

	return true;
}


/*
calculate intersection segment line of t1 and t2;
t1,t2 are two input triangles;
line_section is the vector storing the two point of the intersection segment line of t1 and t2;
*/
void cal_intersection(EleFace *t1, EleFace *t2, vector<vector<Vertex*>> &line_section) {

	Vector3d nor1 = t1->normal;//normal of triangle1
	Vector3d nor2 = t2->normal;
	Vertex *va1 = t1->vertex0;
	Vertex *vb1 = t1->vertex1;
	Vertex *vc1 = t1->vertex2;
	Vertex *va2 = t2->vertex0;
	Vertex *vb2 = t2->vertex1;
	Vertex *vc2 = t2->vertex2;
	vector<Vertex*> edgeset1;//use start vertex to denote each edge
	vector<Vertex*> edgeset2;
	edgeset1.push_back(va1);
	edgeset1.push_back(vb1);
	edgeset1.push_back(vc1);
	edgeset1.push_back(va1);
	edgeset2.push_back(va2);
	edgeset2.push_back(vb2);
	edgeset2.push_back(vc2);
	edgeset2.push_back(va2);

	/*judge whether t1 intersect with t2;if not,return;*/

	//judge whether each edge of t1 is at same side of t2
	int is_sd[6];
	for (int i = 0; i < 3; i++) { is_sd[i] = is_same_side(edgeset1[i], edgeset1[i + 1], t2); }
	if (is_sd[0] == 0 && is_sd[1] == 0 && is_sd[2] == 0) { return; }//three edges is at the same side,don't intersect with t2

	//judge whether each edge of t2 is at same side of t1
	for (int i = 0; i < 3; i++) { is_sd[i + 3] = is_same_side(edgeset2[i], edgeset2[i + 1], t1); }
	if (is_sd[3] == 0 && is_sd[4] == 0 && is_sd[5] == 0) { return; }

	//judge each edge of t1 intersect with t2
	int is_int[6];
	for (int i = 0; i < 6; i++) {
		if (is_sd[i] == 0) { is_int[i] = 0; }//at the same side, separate
		if (is_sd[i] == 2) { is_int[i] = 2; }//on the face
		if (is_sd[i] == 1) {
			if (i < 3) { is_int[i] = is_intersection(edgeset1[i], edgeset1[i + 1], t2); }//t1 edge
			else { is_int[i] = is_intersection(edgeset2[i - 3], edgeset2[i + 1 - 3], t1); }//t2 edge
		}
	}
	if (is_int[0] == 0 && is_int[1] == 0 && is_int[2] == 0 && is_int[3] == 0 && is_int[4] == 0 && is_int[5] == 0) { return; }//three edges don't intersect with t2

	//coplanor1r
	if ((nor1.Cross(nor2)).L2Norm() == 0) {return;}

	/*calculate the intersect point*/

	vector<Vertex*> point;
	bool find_answer = false;

	for (int i = 0; i < 6; i++) {

		if (is_int[i] == 1) {
			if (i < 3) {
				if (calculate_intersect_point(edgeset1[i], edgeset1[i + 1], t2, point) == true) { find_answer = true; }
			}
			else {
				if (calculate_intersect_point(edgeset2[i - 3], edgeset2[i + 1 - 3], t1, point) == true) { find_answer = true; }
			}
		}

		if (is_int[i] == 2) {
			if (i < 3) {
				for (int j = 3; j < 6; j++) {
					if (is_int[j] == 2) {//two edge overlap
						if (cal_intersect_point_collinear(edgeset1[i], edgeset1[i + 1], edgeset2[j - 3], edgeset2[j + 1 - 3], point) == true) { find_answer = true; }
					}
					is_int[i] == 0;
					is_int[j] == 0;
				}
			}
			if (i < 3) {
				if (calculate_intersect_point_colplanar(edgeset1[i], edgeset1[i + 1], t2, point) == true) { find_answer = true; }
			}
			else {
				if (calculate_intersect_point_colplanar(edgeset2[i - 3], edgeset2[i + 1 - 3], t1, point) == true) { find_answer = true; }
			}
		}
	}
	if (find_answer = true) {line_section.push_back(point);}
	return;
}
