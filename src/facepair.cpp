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


template <class T>
T max(T x, T y, T z) {
	return __max(__max(x, y), z);
}

template <class T>
T min(T x, T y, T z) {
	return __min(__min(x, y), z);
}

template <class T>
T max(T x, T y) {
	return __max(x, y);
}

template <class T>
T min(T x, T y) {
	return __min(x, y);
}


enum IntersectionStatus
{
	NoIntersection,
	PointIntersection,
	LineIntersection,
	VertexIntersection,
};

bool is_all_NoIntersection(IntersectionStatus *array, int size) {
	for (int i = 0; i < size; i++) {
		if (array[i]!= NoIntersection) {
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
IntersectionStatus getSegmentFaceIntersection(Edge edge, EleFace *tria) {
	Vector3f plane_point = tria->vertex0;
	Vector3f plane_norm = tria->normal;
	float edge_start_position = (edge.start - plane_point).Dot(plane_norm);
	float edge_end_position = (edge.end - plane_point).Dot(plane_norm);

	if ((edge_start_position * edge_end_position > 0)) {
		return NoIntersection;
	}
	else if (edge_start_position || edge_end_position) {
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
bool is_PointIntersection(Edge edge, EleFace *tria) {
	Vector3f v1_tria = tria->vertex0;
	Vector3f v2_tria = tria->vertex1;
	Vector3f v3_tria = tria->vertex2;
	Vector3f tmp1 = (v2_tria - v1_tria).Cross(edge.start - v1_tria);
	Vector3f tmp2 = (v3_tria - v2_tria).Cross(edge.start - v2_tria);
	Vector3f tmp3 = (v1_tria - v3_tria).Cross(edge.start - v3_tria);
	
	float g1 = (edge.end - v1_tria).Dot(tmp1) * (v3_tria - v1_tria).Dot(tmp1);
	float g2 = (edge.end - v2_tria).Dot(tmp2) * (v1_tria - v2_tria).Dot(tmp2);
	float g3 = (edge.end - v3_tria).Dot(tmp3) * (v2_tria - v3_tria).Dot(tmp3);

	if (g1 >= 0 && g2 >= 0 && g3 >= 0) 
		return true;
	else {
		return false;
	}
}

float cal_determinant(float matrix[3][3]) {
	float det = matrix[0][0] * matrix[1][1] * matrix[2][2] + matrix[1][0] * matrix[2][1] * matrix[0][2] + matrix[2][0] * matrix[0][1] * matrix[1][2];
	det = det - (matrix[0][2] * matrix[1][1] * matrix[2][0] + matrix[0][0] * matrix[1][2] * matrix[2][1] + matrix[0][1] * matrix[1][0] * matrix[2][2]);
	return det;
}

/*If the determinant of matrix_A is zero and array_b is non-zero
	return false.
  If the determinant of matrix_A is non-zero,
    return true, and return the answer in array_x.
*/
bool get_linear_equation_z(float matrix_A[3][3], float array_b[3], float &array_x3) {
	float base_D = cal_determinant(matrix_A);

	if (base_D == 0) {
		if (array_b[0] && array_b[1] && array_b[2]) {
			return false;
		}
		array_x3 = 0;
		return true;
	}
	float D[3];//derterminant of A1,A2,A3

	//only solve b
	for (int j = 0; j < 3; j++) {
		matrix_A[j][2] = array_b[j];//change col
	}
	D[2] = cal_determinant(matrix_A);
	array_x3 = D[2] / base_D;
	return true;
}

//calculate intersect point of edge p1p2 and face tria,put intersect point in answer
bool calculate_intersect_point(Edge edge, EleFace &tria, std::vector<Vector3f*> &answer) {
	Vector3f edge_p = edge.end - edge.start;
	Vector3f edge_q1 = tria.vertex[1] - tria.vertex[0];
	Vector3f edge_q2 = tria.vertex[2] - tria.vertex[0];
	Vector3f v_q = tria.vertex[0];
	Vector3f v_p = edge.start;
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
	float ratio = 0;
	if (get_linear_equation_z(mat_A, arr_b, ratio)) {
		answer.push_back(new Vector3f);
		answer.back()->x = v_p.x + ratio * edge_p.x;
		answer.back()->y = v_p.y + ratio * edge_p.y;
		answer.back()->z = v_p.z + ratio * edge_p.z;
		if (answer.size() == 2) {
			if ((*answer[1]-*answer[0]).Iszero()) {
				answer.pop_back();
				return false;
			}
		}
		return true;
	}
	return false;
}

/*two edges are collinear*/
bool cal_intersect_point_collinear(Edge edge1, Edge edge2, std::vector<Vector3f*> &res) {
	//fast judge separate
	for (int i = 0; i < 3; i++) {
		if (max(edge1.start[i], edge1.end[i]) < min(edge2.start[i], edge2.end[i])) {
			return 0;
		}
		if (min(edge1.start[i], edge1.end[i]) > max(edge2.start[i], edge2.end[i])) {
			return 0;
		}
	}

	Vector3f tmp = edge1.end - edge1.start;
	std::vector<Vector3f*> tmpv;
	tmpv.push_back(&edge1.start);
	tmpv.push_back(&edge1.end);
	tmpv.push_back(&edge2.start);
	tmpv.push_back(&edge2.end);
	float t[4];
	float tmax = 0;
	float tmin = 0;
	for (int i = 0; i < 4; i++) {
		t[i] = (*tmpv[i]- edge1.start).Dot(tmp);
		if (t[i] != 0) {
			t[i] = t[i] / abs(t[i])*(*tmpv[i] - edge1.start).L2Norm();
		}
		tmax = max(tmax, t[i]);
		tmin = min(tmin, t[i]);
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 2; j < 4; j++) {
			if (t[i] == t[j]) {
				Vector3f *intersect_points = new Vector3f(*tmpv[i]);
				res.push_back(intersect_points);
			}
		}
	}
	if (res.size() == 2) return true;
	for (int i = 0; i < 4; i++) {
		if (t[i]<tmax && t[i]>tmin) {
			Vector3f *intersect_points = new Vector3f(*tmpv[i]);
			res.push_back(intersect_points);
		}
		if (res.size() == 2) return true;
	}
	return false;
}

/*one edge and a triangle is colplanar*/
//judge whether two point of a segment are line at same side of another segment line
bool is_line_intersected(Edge edge1, Edge edge2) {
	float tmp = ((edge1.end-edge1.start).Cross(edge2.end-edge2.start)).Dot(
		(edge2.end-edge1.start).Cross(edge1.end - edge1.start));
	if (tmp > 0) {
		return false;
	}
	return true;
}


//judge whether two segment lines intersect
//0 for separate,1 for intersect,2 for same line,3 for intersect point is segment vertex
IntersectionStatus judge_intersect(Edge edge1, Edge edge2) {
	//fast judge separate
	for (int i = 0; i < 3; i++) {
		if (max(edge1.start[i], edge1.end[i]) < min(edge2.start[i], edge2.end[i])) {
			return NoIntersection;
		}
		if (min(edge1.start[i], edge1.end[i]) > max(edge2.start[i], edge2.end[i])) {
			return NoIntersection;
		}
	}

	Vector3f edge_p = edge1.end - edge1.start;
	Vector3f edge_q = edge2.end - edge2.start;
	if (edge_p.Cross(edge_q).Iszero()) {
		if ((edge2.start-edge1.start).Cross(edge2.end-edge1.end).Iszero()) {
			return LineIntersection;
		} // on the same line
		return NoIntersection;//parallel
	}

	if (is_line_intersected(edge1, edge2)) {
		return PointIntersection;
	}
	return NoIntersection;
}

//calculate determinant 2x2
float cal_determinant_2(float a, float b, float c, float d, float e, float f) {
	float det = a * d - b * c;
	if (det == 0) { return 2; }//2 for no answer
	else { return (e * d - b * f) / det; }
}

//calculate intersect point of two conlplanar segment lines
void cal_intersect_point_segment(Edge edge1, Edge edge2, std::vector<Vector3f*> &res) {
	Vector3f edge_p = edge1.end - edge1.start;
	Vector3f edge_q = edge2.end - edge2.start;
	float matrix_A[3][2];
	float array_b[3];
	array_b[0] = edge1.start.x - edge2.start.x;
	array_b[1] = edge1.start.y - edge2.start.y;
	array_b[2] = edge1.start.z - edge2.start.z;
	for (int i = 0; i < 3; i++) {
		matrix_A[i][0] = edge_p[i];
		matrix_A[i][1] = -edge_q[i];
	}
	float t;//answer
	t = cal_determinant_2(matrix_A[0][0], matrix_A[0][1], matrix_A[1][0], matrix_A[1][1], array_b[0], array_b[1]);
	if (t == 2) { t = cal_determinant_2(matrix_A[1][0], matrix_A[1][1], matrix_A[2][0], matrix_A[2][1], array_b[1], array_b[2]); }
	if (t == 2) { t = cal_determinant_2(matrix_A[2][0], matrix_A[2][1], matrix_A[0][0], matrix_A[0][1], array_b[2], array_b[0]); }

	Vector3f *intersect_points = new Vector3f(edge1.start.x + t * edge_p.x, edge1.start.y + t * edge_p.y, edge1.start.z + t * edge_p.z);
	res.push_back(intersect_points);
	return;
}



//calculate intersect point when edge p1p2 is on face tria
bool calculate_intersect_point_colplanar(Edge edge, EleFace &tria,std::vector<Vector3f*> &res) {
	int a = judge_intersect(edge, tria.edge[0]);
	int b = judge_intersect(edge, tria.edge[1]);
	int c = judge_intersect(edge, tria.edge[2]);
	if (a == 0 && b == 0 && c == 0) {
		return false;
	}
	//on the same line
	if (a == 2) {
		if (cal_intersect_point_collinear(edge, tria.edge[0], res)) {
			return true;
		}
	}	
	if (b == 2) {
		if (cal_intersect_point_collinear(edge, tria.edge[1], res)) {
			return true;
		}
	}	
	if (c == 2) {
		if (cal_intersect_point_collinear(edge, tria.edge[2], res)) {
			return true;
		}
	}
	//intersect on segment vertex
	if (a == 3 && c == 3) { res.push_back(&tria.vertex[0]); }
	if (a == 3 && b == 3) { res.push_back(&tria.vertex[1]); }
	if (b == 3 && c == 3) { res.push_back(&tria.vertex[2]); }
	// cross intersect
	if (a == 1) { cal_intersect_point_segment(edge, tria.edge[0], res); }
	if (b == 1) { cal_intersect_point_segment(edge, tria.edge[1], res); }
	if (c == 1) { cal_intersect_point_segment(edge, tria.edge[2], res); }

	return true;
}



/*
 * If Two triangles are parallel,
 *		return false.
 * If the AABB box of triangles are seperated,
 *		return false.
 * 
calculate intersection segment line of t1 and t2;
t1,t2 are two input triangles;
*/
bool cal_intersection(std::vector<EleFace*> t, std::vector<Vector3f*> &point) {

	EleFace tria[2] = { *t[0], *t[1] };
	//If the AABB box of triangles are seperated, return false.
	if (!isRegionIntersected(tria[0].region, tria[1].region)) {
		return false;
	}

	//ROUGHLY: judge whether each edge of tria[j] is at the same side of tria[j+1] plane
	IntersectionStatus inters_status[2][3] = {NoIntersection};
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 3; i++) {
			inters_status[j][i] = getSegmentFaceIntersection(
				tria[j].edge[i], &tria[(j+1)%2]);
		}
		if (is_all_NoIntersection(inters_status[j], 3)) {
			return false;
		}
	}

	//CAREFULLY: judge each edge of tria[j] intersect with tria[j+1]
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			if (inters_status[j][i] == PointIntersection) {
				if (!is_PointIntersection(tria[j].edge[i], &tria[(j + 1) % 2])) {
					inters_status[j][i] = NoIntersection;
				}
			}
		}
	}
	
	/*calculate the intersect point*/
	//situation 1
	bool situation_1_flag = true;
	int sum_point_intersection = 0;
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 3; i++) {
			if (inters_status[j][i] == PointIntersection) {
				sum_point_intersection++;
				if (!calculate_intersect_point(tria[j].edge[i], tria[(j + 1) % 2], point)) {
					std::cout << "Unknow bug in situation 1";
				}
				continue;
			}
			if (inters_status[j][i] == LineIntersection) {
				situation_1_flag = false;
				break;
			}
		}
		if (!situation_1_flag) break;
	}
	if (situation_1_flag) {
		if(sum_point_intersection == 2){
			return true;
		}
		if (sum_point_intersection == 3) {
			point[2] = point[3];
			return true;
		}
		return false;
	}

	//situation 2
	int sum_line_intersection = 0;
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 3; i++) {
			if (inters_status[j][i] == LineIntersection) {
				sum_line_intersection++;
				if (!calculate_intersect_point_colplanar(tria[j].edge[i], tria[(j+1)%2], point)) {
					std::cout << "2.0 Unknow bug in situation 2";
					return false;
				}
			}
		}
		if (j == 0 && sum_line_intersection > 1) {
			std::cout << "2.1 Unknow bug in situation 2";
			return false;
		}
		if (j == 1 && sum_line_intersection >= 2) {
			std::cout << "Two edge overlap";
			return true;
		}
	}

	std::cout << "SOME UNEXPECTED SITUATION!!!";
	return false;
}