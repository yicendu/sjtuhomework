#include "stlloading.h"
#include <iostream>
#include "octree.h"
#include "vector3.h"
#include <vector>
#define epsilon 1e-9

#ifndef __min
#define __min(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef __max
#define __max(a,b) ((a)>(b)?(a):(b))
#endif

float max(float x, float y, float z) {
	return __max(__max(x, y), z);
}

float min(float x, float y, float z) {
	return __min(__min(x, y), z);
}

float max(float x, float y) {
	return __max(x, y);
}

float min(float x, float y) {
	return __min(x, y);
}


Vector3f edge(Vector3f a, Vector3f b) {
	return Vector3f(b.x - a.x, b.y - a.y, b.z - a.z);
}

enum IntersectionStatus
{
	NoIntersection,
	PointIntersection,
	LineIntersection,
};

bool is_all_zero(IntersectionStatus *array, int size) {
	for (int i = 0; i < size; i++) {
		if (array[i]) {
			return false;
		}
	}
	return true;
}


/*
 * If both two vertexes of va_2, va_b are on the plane that tria face defined (with one-line intersection), 
 *		return LineIntersection;
 * If both two vertexes of va_2, va_b are on the same side of the plane (with no intersection),
 *      return PointIntersection 
 * If the segment defined by two vertexes of va_2, va_b intersected with the plane (with one-point intersection),
 *      return NoIntersection
*/
IntersectionStatus getSegmentFaceIntersection(Vector3f v1, Vector3f v2, EleFace *tria) {
	Vector3f plane_point = tria->vertex0;
	Vector3f plane_norm = tria->normal;
	float v1_position = edge(plane_point, v1).Dot(plane_norm);
	float v2_position = edge(plane_point, v2).Dot(plane_norm);

	if ((v1_position * v2_position > 0)) {
		return NoIntersection;
	}
	else if (v1_position || v2_position) {
		return PointIntersection;
	}
	else {
		return LineIntersection;
	}
}

//judege whether one edge of of triangle2 is separate from triangle1
//0 for separation(include one vertex on edge),1 for intersection,2 for on the face(not judeged in this function)
/*
 * v1_e the first vertex of the edge
 * v2_e the second vertex of the edge
 * tria the triangle
 */
IntersectionStatus is_intersection(Vector3f v1_edge, Vector3f v2_edge, EleFace *tria) {
	Vector3f v1_tria = tria->vertex0;
	Vector3f v2_tria = tria->vertex1;
	Vector3f v3_tria = tria->vertex2;
	Vector3f tmp1 = edge(v1_tria, v2_tria).Cross(edge(v1_tria, v1_edge));
	//if ((tmp1.operator-(tria->normal).Iszero() == 1)) {//va_1 is on the face,change to vb_2
	//	va_2 = vb2;
	//	vb_2 = va2;
	//	tmp1 = edge(va_1, vb_1).Cross(edge(va_1, va_2));
	//}
	Vector3f tmp2 = edge(v2_tria, v3_tria).Cross(edge(v2_tria, v1_edge));
	Vector3f tmp3 = edge(v3_tria, v1_tria).Cross(edge(v3_tria, v1_edge));
	
	float g1 = (edge(v1_tria, v2_edge).Dot(tmp1)) * (edge(v1_tria, v3_tria).Dot(tmp1));
	float g2 = (edge(v2_tria, v2_edge).Dot(tmp2)) * (edge(v2_tria, v1_tria).Dot(tmp2));
	float g3 = (edge(v3_tria, v2_edge).Dot(tmp3)) * (edge(v3_tria, v2_tria).Dot(tmp3));

	if (g1 >= 0 && g2 >= 0 && g3 >= 0) 
		return PointIntersection;
	//else if ((g1 == 0 && g2 == 0) || (g2 == 0 && g3 == 0) || (g3 == 0 && g1 == 0)) { return 1; }
	else {
		return NoIntersection;
	}
}

float cal_determinant(float matrix[3][3]) {
	float det = matrix[0][0] * matrix[1][1] * matrix[2][2] + matrix[1][0] * matrix[2][1] * matrix[0][2] + matrix[2][0] * matrix[0][1] * matrix[1][2];
	det = det - (matrix[0][2] * matrix[1][1] * matrix[2][0] + matrix[0][0] * matrix[1][2] * matrix[2][1] + matrix[0][1] * matrix[1][0] * matrix[2][2]);
	return det;
}

bool solve_equation(float matrix_A[3][3], float array_b[3], float array_x[3]) {
	float base_D = cal_determinant(matrix_A);
	if (base_D != 0) {
		float D[3];//derterminant of A1,A2,A3

		//only solve b
		for (int j = 0; j < 3; j++) {
			matrix_A[j][2] = array_b[j];//change col
		}
		D[2] = cal_determinant(matrix_A);
		array_x[2] = D[2] / base_D;
		return true;
	}
	else {
		return false;
	}//no answer
}

//calculate intersect point of edge p1p2 and face tria,put intersect point in answer
bool calculate_intersect_point(Vector3f p1, Vector3f p2, EleFace *tria, vector<Vector3f*> &answer) {
	Vector3f edge_p = edge(p1, p2);
	Vector3f edge_q1 = edge(tria->vertex0, tria->vertex1);
	Vector3f edge_q2 = edge(tria->vertex0, tria->vertex2);
	Vector3f v_q = tria->vertex0;
	Vector3f v_p = p1;
	//b=vertex p -vertex q
	float arr_b[3];
	arr_b[0] = v_p.x - v_q.x;
	arr_b[1] = v_p.y - v_q.y;
	arr_b[2] = v_p.z - v_q.z;
	//A=[e_q1,e_q2,-e_p]
	float mat_A[3][3];
	for (int i = 0; i < 3; i++) {
		mat_A[i][0] = edge_q1[i];
		mat_A[i][1] = edge_q2[i];
		mat_A[i][2] = -edge_p[i];
	}
	//v_q0+ a1*e_q1+a2*e_q2=v_p+b*e_p
	//x=[a1,a2,b]
	float ans_x[3];
	if (solve_equation(mat_A, arr_b, ans_x)) {
		answer.push_back(new Vector3f);
		answer.back()->x = v_p.x + ans_x[2] * edge_p.x;
		answer.back()->y = v_p.y + ans_x[2] * edge_p.y;
		answer.back()->z = v_p.z + ans_x[2] * edge_p.z;
		if (answer.size() == 2) {
			if (answer[0]->operator-(*answer[1]).Iszero() == 1) {
				answer.pop_back();
				return false;
			}
		}
		return true;
	}
	else {
		return false;
	}
}

/*two edges are collinear*/
bool cal_intersect_point_collinear(Vector3f p1, Vector3f p2, Vector3f q1, Vector3f q2, vector<Vector3f*> &answer) {
	//fast judge separate
	for (int i = 0; i < 3; i++) {
		if (max(p1[i], p2[i]) < min(q1[i], q2[i])) { 
			return false;
		}
		else if(min(p1[i], p2[i]) > max(q1[i], q2[i])) {
			return false;
		}
	}

	Vector3f tmp = edge(p1, p2);
	vector<Vector3f*> tmpv;
	tmpv.push_back(&p1);
	tmpv.push_back(&p2);
	tmpv.push_back(&q1);
	tmpv.push_back(&q2);
	float t[4];
	float tmax = 0;
	float tmin = 0;
	for (int i = 0; i < 4; i++) {
		t[i] = edge(p1, *tmpv[i]).Dot(tmp);
		if (t[i] != 0) { t[i] = t[i] / abs(t[i])*edge(p1, *tmpv[i]).L2Norm(); }
		tmax = max(tmax, t[i]);
		tmin = min(tmin, t[i]);
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 2; j < 4; j++) {
			if (t[i] == t[j]) {
				answer.push_back(new Vector3f(tmpv[i]->x, tmpv[i]->x, tmpv[i]->x));
			}
		}
	}
	if (answer.size() == 2) return true;
	for (int i = 0; i < 4; i++) {
		if (t[i]<tmax && t[i]>tmin) {
			answer.push_back(new Vector3f);
			answer.back()->x = tmpv[i]->x;
			answer.back()->y = tmpv[i]->y;
			answer.back()->z = tmpv[i]->z;
		}
		if (answer.size() == 2) return true;
	}
	return false;
}

/*one edge and a triangle is colplanar*/
//judge whether two point of a segment are line at same side of another segment line
//0 for cross,1 for one side,2 for one vertex on segment
int judge_same_side(Vector3f p1, Vector3f p2, Vector3f q1, Vector3f q2) {
	float tmp = (edge(q1, p1).Cross(edge(q1, q2))).Dot(edge(q1, p2).Cross(edge(q1, q2)));
	if (tmp > 0) { return 1; }
	else if (tmp == 0) { return 2; }
	else { return 0; }
}

//judge whether two segment lines intersect
//0 for separate,1 for intersect,2 for same line,3 for intersect point is segment vertex
int judge_intersect(Vector3f p1, Vector3f p2, Vector3f q1, Vector3f q2) {
	//fast judge separate
	for (int i = 0; i < 3; i++) {
		if ((max(p1[i], p2[i]) < min(q1[i], q2[i]))) {
			return 0;
		}
		else if (min(p1[i], p2[i]) > max(q1[i], q2[i])) {
			return 0;
		}
	}

	Vector3f edge_p = edge(p1, p2);
	Vector3f edge_q = edge(q1, q2);
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
float cal_determinant_2(float a, float b, float c, float d, float e, float f) {
	float det = a * d - b * c;
	if (det == 0) { return 2; }//2 for no answer
	else { return (e * d - b * f) / det; }
}

//calculate intersect point of two conlplanar segment lines
void cal_intersect_point_segment(Vector3f p1, Vector3f p2, Vector3f q1, Vector3f q2, vector<Vector3f*> &res) {
	Vector3f edge_p = edge(p1, p2);
	Vector3f edge_q = edge(q1, q2);
	float matrix_A[3][2];
	float array_b[3];
	array_b[0] = q1.x - p1.x;
	array_b[1] = q1.y - p1.y;
	array_b[2] = q1.z - p1.z;
	for (int i = 0; i < 3; i++) {
		matrix_A[i][0] = edge_p[i];
		matrix_A[i][1] = -edge_q[i];
	}
	float t;//answer
	t = cal_determinant_2(matrix_A[0][0], matrix_A[0][1], matrix_A[1][0], matrix_A[1][1], array_b[0], array_b[1]);
	if (t == 2) { t = cal_determinant_2(matrix_A[1][0], matrix_A[1][1], matrix_A[2][0], matrix_A[2][1], array_b[1], array_b[2]); }
	if (t == 2) { t = cal_determinant_2(matrix_A[2][0], matrix_A[2][1], matrix_A[0][0], matrix_A[0][1], array_b[2], array_b[0]); }

	Vector3f *intersect_points = new Vector3f(p1.x + t * edge_p.x, p1.y + t * edge_p.y, p1.z + t * edge_p.z);
	res.push_back(new Vector3f);
	return;
}

//calculate intersect point when edge p1p2 is on face tria
bool calculate_intersect_point_colplanar(Vector3f p1, Vector3f p2, EleFace *tria, vector<Vector3f*> &answer) {
	int a = judge_intersect(p1, p2, tria->vertex0, tria->vertex1);
	int b = judge_intersect(p1, p2, tria->vertex1, tria->vertex2);
	int c = judge_intersect(p1, p2, tria->vertex2, tria->vertex0);
	if (a == 0 && b == 0 && c == 0) { return false; }
	//intersect on segment vertex
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
*/
bool cal_intersection(EleFace *tria1, EleFace *tria2,vector<Vector3f*> &point) {

	//If Two triangles are parallel, return false.
	if (cross(tria2->normal, tria1->normal).L2Norm() < epsilon) {
		return false;
	}

	Vector3f va1 = tria1->vertex0;
	Vector3f vb1 = tria1->vertex1;
	Vector3f vc1 = tria1->vertex2;

	Vector3f va2 = tria2->vertex0;
	Vector3f vb2 = tria2->vertex1;
	Vector3f vc2 = tria2->vertex2;

	for (int i = 0; i < 3; i++) {
		if (max(va1[i], vb1[i], vc1[i]) < min(va2[i], vb2[i], vc2[i])) {
			return false;
		}
		else if(min(va1[0], vb1[0], vc1[0]) > max(va2[0], vb2[0], vc2[0])){
			return false;
		}
	}

	vector<Vector3f> edgevector1;//use start vertex to denote each edge
	vector<Vector3f> edgevector2;

	edgevector1.push_back(va1);
	edgevector1.push_back(vb1);
	edgevector1.push_back(vc1);

	edgevector2.push_back(va2);
	edgevector2.push_back(vb2);
	edgevector2.push_back(vc2);

	/*judge whether tria1 intersect with tria2;if not,return false;*/

	//ROUGHLY judge whether each edge of tria1 is at same side of tria2 plane
	IntersectionStatus inters_status[6];

	for (int i = 0; i < 3; i++) {
		inters_status[i] = getSegmentFaceIntersection(
			edgevector1[i % 3], edgevector1[(i + 1) % 3], tria2);
	}
	if (is_all_zero(inters_status,3)) {
		return false;
	}//three edges is at the same side,don't intersect with tria2

	//judge whether each edge of tria2 is at same side of tria1
	for (int i = 0; i < 3; i++) {
		inters_status[i + 3] = getSegmentFaceIntersection(
			edgevector2[i % 3], edgevector2[(i + 1) % 3], tria1);
	}
	if (is_all_zero(inters_status + 3, 3)) {
		return false;
	}

	//CAREFULLY judge each edge of tria1 intersect with tria2
	for (int i = 0; i < 6; i++) {
		if (inters_status[i] == 1) {
			if (i < 3) {
				inters_status[i] = is_intersection(
					edgevector1[i%3], edgevector1[(i + 1)%3], tria2);
			}//tria1 edge
			else {
				inters_status[i] = is_intersection(
					edgevector2[i % 3], edgevector2[(i + 1) % 3], tria1);
			}//tria2 edge
		}
	}

	if (is_all_zero(inters_status, 6)) {
		return false;
	}

	/*calculate the intersect point*/

	bool find_answer = false;

	for (int i = 0; i < 6; i++) {
		if (inters_status[i] == 2) {
			if (i < 3) {
				for (int j = 3; j < 6; j++) {
					if (inters_status[j] == 2) {//two edge overlap
						if (cal_intersect_point_collinear(edgevector1[i%3], edgevector1[(i + 1)%3], edgevector2[(j - 3)%3], edgevector2[(j + 1 - 3)%3], point)) {
							find_answer = true;
						}
					}
					inters_status[i] = NoIntersection;
					inters_status[j] = NoIntersection;
				}
			}
			if (i < 3) {
				if (calculate_intersect_point_colplanar(edgevector1[i%3], edgevector1[(i + 1)%3], tria2, point)) { find_answer = true; }
			}
			else {
				if (calculate_intersect_point_colplanar(edgevector2[(i - 3)%3], edgevector2[(i + 1 - 3)%3], tria1, point)) { find_answer = true; }
			}
		}
		if (point.size() == 2) { break; }

		if (inters_status[i] == 1) {
			if (i < 3) {
				if (calculate_intersect_point(edgevector1[i%3], edgevector1[(i + 1)%3], tria2, point)) { find_answer = true; }
			}
			else {
				if (calculate_intersect_point(edgevector2[(i - 3)%3], edgevector2[(i + 1 - 3)%3], tria1, point)) { find_answer = true; }
			}
		}
	}
	if (find_answer) { 

		return true; 
	}
	return false;
}