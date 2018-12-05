#include "stlreading.h"
#include <iostream>
#include "octree.h"
#include "vector3.h"
#include <vector>
#include <algorithm>

//two vertex to vector3d
Vector3d edge(Vector3d a, Vector3d b) {
	Vector3d tmp;
	tmp[0] = b.X() - a.X();
	tmp[1] = b.Y() - a.Y();
	tmp[2] = b.Z() - a.Z();
	return tmp;
}

//copy vertex1's value to vertex2
void v_assign(Vector3d v1, Vector3d v2) {
	v2.X() = v1.X();
	v2.Y() = v1.Y();
	v2.Z() = v1.Z();
	return;
}

//judge whether one edge of triangle2 is at the same side of triangle1
//0 for at same side,1 for at different side(include one vertex on face),2 for 2 vertices on the face
int is_same_side(Vector3d va_2, Vector3d vb_2, EleFace *tria) {
	Vector3d va_1 = tria->vertex0;
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
int is_intersection(Vector3d va2, Vector3d vb2, EleFace *tria) {
	Vector3d &va_2 = va2;
	Vector3d &vb_2 = vb2;
	Vector3d va_1 = tria->vertex0;
	Vector3d vb_1 = tria->vertex1;
	Vector3d vc_1 = tria->vertex2;
	Vector3d tmp1 = edge(va_1, vb_1).Cross(edge(va_1, va_2));
	//if ((tmp1.operator-(tria->normal).Iszero() == 1)) {//va_1 is on the face,change to vb_2
	//	va_2 = vb2;
	//	vb_2 = va2;
	//	tmp1 = edge(va_1, vb_1).Cross(edge(va_1, va_2));
	//}
	Vector3d tmp2 = edge(vb_1, vc_1).Cross(edge(vb_1, va_2));
	Vector3d tmp3 = edge(vc_1, va_1).Cross(edge(vc_1, va_2));
	
	double g1 = (edge(va_1, vb_2).Dot(tmp1)) * (edge(va_1, vc_1).Dot(tmp1));
	double g2 = (edge(vb_1, vb_2).Dot(tmp2)) * (edge(vb_1, va_1).Dot(tmp2));
	double g3 = (edge(vc_1, vb_2).Dot(tmp3)) * (edge(vc_1, vb_1).Dot(tmp3));

	if (g1 >= 0 && g2 >= 0 && g3 >= 0) { return 1; }
	//else if ((g1 == 0 && g2 == 0) || (g2 == 0 && g3 == 0) || (g3 == 0 && g1 == 0)) { return 1; }
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
bool calculate_intersect_point(Vector3d p1, Vector3d p2, EleFace *tria, vector<Vector3d*> &answer) {
	Vector3d edge_p = edge(p1, p2);
	Vector3d edge_q1 = edge(tria->vertex0, tria->vertex1);
	Vector3d edge_q2 = edge(tria->vertex0, tria->vertex2);
	Vector3d v_q = tria->vertex0;
	Vector3d v_p = p1;
	//b=vertex p -vertex q
	double arr_b[3];
	arr_b[0] = v_p.X() - v_q.X();
	arr_b[1] = v_p.Y() - v_q.Y();
	arr_b[2] = v_p.Z() - v_q.Z();
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
	if (solve_equation(mat_A, arr_b, ans_x)) {
		answer.push_back(new Vector3d);
		answer.back()->X() = v_p.X() + ans_x[2] * edge_p.X();
		answer.back()->Y() = v_p.Y() + ans_x[2] * edge_p.Y();
		answer.back()->Z() = v_p.Z() + ans_x[2] * edge_p.Z();
		if (answer.size() == 2) {
			if (answer[0]->operator-(*answer[1]).Iszero() == 1) {
				answer.pop_back();
				return false;
			}
		}
		return true;
	}
	else { return false; }
}

/*two edges are collinear*/
bool cal_intersect_point_collinear(Vector3d p1, Vector3d p2, Vector3d q1, Vector3d q2, vector<Vector3d*> &answer) {
	//fast judge separate
	if ((max(p1.X(), p2.X()) < min(p1.X(), p2.X())) || (min(p1.X(), p2.X()) > max(p1.X(), p2.X()))) { return 0; }
	if ((max(p1.Y(), p2.Y()) < min(p1.Y(), p2.Y())) || (min(p1.Y(), p2.Y()) > max(p1.Y(), p2.Y()))) { return 0; }
	if ((max(p1.Z(), p2.Z()) < min(p1.Z(), p2.Z())) || (min(p1.Z(), p2.Z()) > max(p1.Z(), p2.Z()))) { return 0; }

	Vector3d tmp = edge(p1, p2);
	vector<Vector3d*> tmpv;
	tmpv.push_back(&p1);
	tmpv.push_back(&p2);
	tmpv.push_back(&q1);
	tmpv.push_back(&q2);
	double t[4];
	double tmax = 0;
	double tmin = 0;
	for (int i = 0; i < 4; i++) {
		t[i] = edge(p1, *tmpv[i]).Dot(tmp);
		if (t[i] != 0) { t[i] = t[i] / abs(t[i])*edge(p1, *tmpv[i]).L2Norm(); }
		tmax = max(tmax, t[i]);
		tmin = min(tmin, t[i]);
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 2; j < 4; j++) {
			if (t[i] == t[j]) {
				answer.push_back(new Vector3d);
				answer.back()->X() = tmpv[i]->X();
				answer.back()->Y() = tmpv[i]->Y();
				answer.back()->Z() = tmpv[i]->Z();
			}
		}
	}
	if (answer.size() == 2) { return 1; }
	for (int i = 0; i < 4; i++) {
		if (t[i]<tmax && t[i]>tmin) {
			answer.push_back(new Vector3d);
			answer.back()->X() = tmpv[i]->X();
			answer.back()->Y() = tmpv[i]->Y();
			answer.back()->Z() = tmpv[i]->Z();
		}
		if (answer.size() == 2) { return 1; }
	}

}

/*one edge and a triangle is colplanar*/
//judge whether two point of a segment are line at same side of another segment line
//0 for cross,1 for one side,2 for one vertex on segment
int judge_same_side(Vector3d p1, Vector3d p2, Vector3d q1, Vector3d q2) {
	double tmp = (edge(q1, p1).Cross(edge(q1, q2))).Dot(edge(q1, p2).Cross(edge(q1, q2)));
	if (tmp > 0) { return 1; }
	else if (tmp == 0) { return 2; }
	else { return 0; }
}

//judge whether two segment lines intersect
//0 for separate,1 for intersect,2 for same line,3 for intersect point is segment vertex
int judge_intersect(Vector3d p1, Vector3d p2, Vector3d q1, Vector3d q2) {
	//fast judge separate
	if ((max(p1.X(), p2.X()) < min(p1.X(), p2.X())) || (min(p1.X(), p2.X()) > max(p1.X(), p2.X()))) { return 0; }
	if ((max(p1.Y(), p2.Y()) < min(p1.Y(), p2.Y())) || (min(p1.Y(), p2.Y()) > max(p1.Y(), p2.Y()))) { return 0; }
	if ((max(p1.Z(), p2.Z()) < min(p1.Z(), p2.Z())) || (min(p1.Z(), p2.Z()) > max(p1.Z(), p2.Z()))) { return 0; }

	Vector3d edge_p = edge(p1, p2);
	Vector3d edge_q = edge(q1, q2);
	if (edge_p.Cross(edge_q).Iszero()) {
		if ((edge(p1, q1).Cross(edge(p2, q2)).Iszero())) { return 2; } // on the same line
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
void cal_intersect_point_segment(Vector3d p1, Vector3d p2, Vector3d q1, Vector3d q2, vector<Vector3d*> &answer) {
	Vector3d edge_p = edge(p1, p2);
	Vector3d edge_q = edge(q1, q2);
	double matrix_A[3][2];
	double array_b[3];
	array_b[0] = q1.X() - p1.X();
	array_b[1] = q1.Y() - p1.Y();
	array_b[2] = q1.Z() - p1.Z();
	for (int i = 0; i < 3; i++) {
		matrix_A[i][0] = edge_p[i];
		matrix_A[i][1] = -edge_q[i];
	}
	double t;//answer
	t = cal_determinant_2(matrix_A[0][0], matrix_A[0][1], matrix_A[1][0], matrix_A[1][1], array_b[0], array_b[1]);
	if (t == 2) { t = cal_determinant_2(matrix_A[1][0], matrix_A[1][1], matrix_A[2][0], matrix_A[2][1], array_b[1], array_b[2]); }
	if (t == 2) { t = cal_determinant_2(matrix_A[2][0], matrix_A[2][1], matrix_A[0][0], matrix_A[0][1], array_b[2], array_b[0]); }

	answer.push_back(new Vector3d);
	answer.back()->X() = p1.X() + t * edge_p[0];
	answer.back()->Y() = p1.Y() + t * edge_p[1];
	answer.back()->Z() = p1.Z() + t * edge_p[2];
	
	return;
}

//calculate intersect point when edge p1p2 is on face tria
bool calculate_intersect_point_colplanar(Vector3d p1, Vector3d p2, EleFace *tria, vector<Vector3d*> &answer) {
	int a = judge_intersect(p1, p2, tria->vertex0, tria->vertex1);
	int b = judge_intersect(p1, p2, tria->vertex1, tria->vertex2);
	int c = judge_intersect(p1, p2, tria->vertex2, tria->vertex0);
	if (a == 0 && b == 0 && c == 0) { return false; }
	//intersect on segment vertex
	/*if (a == 3 && b == 3) { answer.push_back(new Vector3d); v_assign(tria->vertex1, *answer.back()); }
	if (b == 3 && c == 3) { answer.push_back(new Vector3d); v_assign(tria->vertex2, *answer.back()); }
	if (a == 3 && c == 3) { answer.push_back(new Vector3d); v_assign(tria->vertex0, *answer.back()); }*/
	if (a == 3 && b == 3) { answer.push_back(&tria->vertex1); }
	if (b == 3 && c == 3) { answer.push_back(&tria->vertex2); }
	if (a == 3 && c == 3) { answer.push_back(&tria->vertex0); }
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
void cal_intersection(EleFace *t1, EleFace *t2, vector<vector<Vector3d*>> &line_section) {

	//coplanor1r
	if ((t1->normal.Cross(t2->normal)).L2Norm() == 0) { return; }

	Vector3d va1 = t1->vertex0;
	Vector3d vb1 = t1->vertex1;
	Vector3d vc1 = t1->vertex2;
	Vector3d va2 = t2->vertex0;
	Vector3d vb2 = t2->vertex1;
	Vector3d vc2 = t2->vertex2;
	vector<Vector3d> edgeset1;//use start vertex to denote each edge
	vector<Vector3d> edgeset2;
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

	

	/*calculate the intersect point*/

	vector<Vector3d*> point;
	bool find_answer = false;

	for (int i = 0; i < 6; i++) {

		if (is_int[i] == 2) {
			if (i < 3) {
				for (int j = 3; j < 6; j++) {
					if (is_int[j] == 2) {//two edge overlap
						if (cal_intersect_point_collinear(edgeset1[i], edgeset1[i + 1], edgeset2[j - 3], edgeset2[j + 1 - 3], point)) { find_answer = true; }
					}
					is_int[i] = 0;
					is_int[j] = 0;
				}
			}
			if (i < 3) {
				if (calculate_intersect_point_colplanar(edgeset1[i], edgeset1[i + 1], t2, point)) { find_answer = true; }
			}
			else {
				if (calculate_intersect_point_colplanar(edgeset2[i - 3], edgeset2[i + 1 - 3], t1, point)) { find_answer = true; }
			}
		}
		if (point.size() == 2) { break; }

		if (is_int[i] == 1) {
			if (i < 3) {
				if (calculate_intersect_point(edgeset1[i], edgeset1[i + 1], t2, point)) { find_answer = true; }
			}
			else {
				if (calculate_intersect_point(edgeset2[i - 3], edgeset2[i + 1 - 3], t1, point)) { find_answer = true; }
			}
		}

		
	}
	if (find_answer) {line_section.push_back(point);}
	return;
}

void StlFile::cal_int(EleFace *t1, EleFace *t2) {
	cal_intersection(t1, t2, intersection);
}