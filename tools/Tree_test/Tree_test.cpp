// Tree_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "octree.h"
#include "stlreading.h"
#include <iostream>
#include <time.h>

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

EleFace ele[1000000];
Vector3D vertex[3];

int main(int argc, char** argv)
{
    std::cout << "Hello World!\n"; 
	Region root_region = {0,0,0,180};

	Octree root(0, root_region, 2, 5);

	float x_length;
	float y_length;
	float z_length;
	srand((int)time(NULL));
	for (int i = 0; i < 1000000; i++) {
		float mid_x = (float)(rand() % 180 + (float)(rand() % 1000) / 1000);
		float mid_y = (float)(rand() % 180 + (float)(rand() % 1000) / 1000);
		float mid_z = (float)(rand() % 180 + (float)(rand() % 1000) / 1000);

		for (int j = 0; j < 3; j++) {
			vertex[j].x = mid_x + (float)(rand() % 1000) / 2000;
			vertex[j].y = mid_y + (float)(rand() % 1000) / 2000;
			vertex[j].z = mid_z + (float)(rand() % 1000) / 2000;
		}


		ele[i].vertex0 = vertex[0];
		ele[i].vertex1 = vertex[1];
		ele[i].vertex2 = vertex[2];
		ele[i].region.x = min(min(vertex[0].x, vertex[1].x), vertex[2].x);
		ele[i].region.y = min(min(vertex[0].y, vertex[1].y), vertex[2].y);
		ele[i].region.z = min(min(vertex[0].z, vertex[1].z), vertex[2].z);
		
		x_length = max(max(vertex[0].x, vertex[1].x), vertex[2].x) - ele[i].region.x;
		y_length = max(max(vertex[0].y, vertex[1].y), vertex[2].y) - ele[i].region.y;
		z_length = max(max(vertex[0].z, vertex[1].z), vertex[2].z) - ele[i].region.z;

		ele[i].region.length = max(max(x_length, y_length), z_length);
		root.insertEle(ele+i);
	}
	{
		Region test;
		test.x = 0;
		test.y = 0;
		test.z = 0;
		test.length = 2;
		std::set<EleFace*> eles = root.queryEles(test);
		std::set<EleFace*>::iterator ele;
		for (ele = eles.begin(); ele != eles.end(); ele++) {
			EleFace* face = *ele;
			std::cout << face->vertex0.x << " " << face->vertex0.y << " " << face->vertex0.x << "\n";
			std::cout << face->vertex1.x << " " << face->vertex1.y << " " << face->vertex1.x << "\n";
			std::cout << face->vertex2.x << " " << face->vertex2.y << " " << face->vertex2.x << "\n";
		}
	}

	if (argc < 2) {
		std::cerr << "program requires at least one argument of filename\n";
		return 1;
	}


	StlFile stl = slt_read(argv[1]);

	Octree root0(&stl,5,5);

	{
		Region test;
		test.x = -59;
		test.y = -26;
		test.z = 10;
		test.length = 26;
		std::set<EleFace*> eles = root0.queryEles(test);
		
		std::set<EleFace*>::iterator ele;
		for (ele = eles.begin(); ele != eles.end(); ele++) {
			EleFace* face = *ele;
			std::cout << face->vertex0.x << " " << face->vertex0.y << " " << face->vertex0.z << "\n";
			std::cout << face->vertex1.x << " " << face->vertex1.y << " " << face->vertex1.z << "\n";
			std::cout << face->vertex2.x << " " << face->vertex2.y << " " << face->vertex2.z << "\n";
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
