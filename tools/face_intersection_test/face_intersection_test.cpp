#include <iostream>
#include "stlreading.h"
#include "faceintersect.h"

int main()
{
	Vector3f a_normal(0, 0, 1);
	Vector3f a_v1(0, 0, 0);
	Vector3f a_v2(1, 0, 0);
	Vector3f a_v3(0, 1, 0);
	EleFace a(a_normal, a_v1, a_v2, a_v3);

	Vector3f b_normal(1, -1, 0);
	Vector3f b_v1(0, 0, 0);
	Vector3f b_v2(1, 1, 1);
	Vector3f b_v3(1, 1, -1);
	EleFace b(b_normal, b_v1, b_v2, b_v3);

	Vector3f c_normal(0, 1, 0);
	Vector3f c_v1(.5, 0, 0);
	Vector3f c_v2(1.5, 0, 0);
	Vector3f c_v3(0, 0, 1);
	EleFace c(c_normal, c_v1, c_v2, c_v3);

	vector<vector<Vector3f*>> ans;
	cal_intersection(&a, &c, ans);
	cout << *ans[0][0] << endl << *ans[0][1] << endl;

	return 0;
}
