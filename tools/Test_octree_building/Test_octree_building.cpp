// Tree_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "octree.h"
#include "stlloading.h"
#include <iostream>
#include <time.h>

int main(int argc, char** argv)
{
	{
	StlFile stl;
	stl.stl_read("../models/squarehead.stl");

	Octree root0(&stl, 5, 5);
	{
		Region test;
		test.m_x = -59;
		test.m_y = -26;
		test.m_z = 10;
		test.m_length = 2;
		std::set<EleFace*> eles = root0.query_eles(test);

		for (auto ele = eles.begin(); ele != eles.end(); ele++) {
			EleFace* face = *ele;
			std::cout << face->m_vertex0 << std::endl << face->m_vertex1 << std::endl << face->m_vertex2 << std::endl;
		}
	}

	}
	
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