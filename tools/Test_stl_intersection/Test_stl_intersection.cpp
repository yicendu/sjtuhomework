// Test_stl_intersection.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include"facepair.h"
#include"octree.h"
#include"stlloading.h"
#include"vector3.h"

#include <iostream>

int main(int argc, char** argv)
{
	//if (argc < 2) {
	//	std::cerr << "program requires at least one argument of filename\n";
	//	return 1;
	//}

	StlFile stl_1;
	StlFile stl_2;
	stl_1.stl_read("../models/rabbit.stl");
	stl_2.stl_read("../models/yellowcat.stl");
	Octree root1(&stl_1, 5, 5);
	Octree root2(&stl_2, 5, 5);

	
	std::cout << search_inter_lines(&root1, &root2).size();
	//for (int i = 0; i < a_b.intersectLine_list.size(); i++) {
	//	std::cout << *(a_b.intersectLine_list[i][0]) << endl << *(a_b.intersectLine_list[i][1]) << endl;
	//}
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
