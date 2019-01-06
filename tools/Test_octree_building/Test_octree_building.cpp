// Tree_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "octree.h"
#include "stlloading.h"
#include <iostream>
#include <time.h>

EleFace ele[1000000];
Vector3f vertex[3];

int main(int argc, char** argv)
{
	{
	
	StlFile stl;
	stl.stl_read("../models/squarehead.stl");

	Octree root0(&stl, 5, 5);
	{
		Region test;
		test.x = -59;
		test.y = -26;
		test.z = 10;
		test.length = 2;
		std::set<EleFace*> eles = root0.queryEles(test);

		std::set<EleFace*>::iterator ele;
		for (ele = eles.begin(); ele != eles.end(); ele++) {
			EleFace* face = *ele;
			std::cout << face->vertex0 << std::endl << face->vertex1 << std::endl << face->vertex2 << std::endl;
		}
	}

	}
	int b = 0;
	int a = 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file