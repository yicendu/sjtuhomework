// Tree_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "octree.h"
#include <iostream>
#include <time.h>

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

struct EleFace ele[100000];
struct Vertex vertex[300000];

int main()
{
    std::cout << "Hello World!\n"; 
	struct OcTreeNode root;
	struct Region root_region;

	initRegion(&root_region, 0, 0, 0, 180);
	initNode(&root, 1, root_region, 0.5);

	float x_length;
	float y_length;
	float z_length;
	srand((int)time(NULL));
	for (int i = 0; i < 100000; i++) {
		double mid_x = (double)(rand() % 180 + (double)(rand() % 1000) / 1000);
		double mid_y = (double)(rand() % 180 + (double)(rand() % 1000) / 1000);
		double mid_z = (double)(rand() % 180 + (double)(rand() % 1000) / 1000);

		for (int j = 0; j < 3; j++) {
			vertex[j + 3 * i].x_value = mid_x + (double)(rand() % 1000) / 2000;
			vertex[j + 3 * i].y_value = mid_y + (double)(rand() % 1000) / 2000;
			vertex[j + 3 * i].z_value = mid_z + (double)(rand() % 1000) / 2000;
		}


		ele[i].vertex0 = &vertex[0 + 3 * i];
		ele[i].vertex1 = &vertex[1 + 3 * i];
		ele[i].vertex2 = &vertex[2 + 3 * i];
		ele[i].region_x = min(min(vertex[0 + 3 * i].x_value, vertex[1 + 3 * i].x_value), vertex[2 + 3 * i].x_value);
		ele[i].region_y = min(min(vertex[0 + 3 * i].y_value, vertex[1 + 3 * i].y_value), vertex[2 + 3 * i].y_value);
		ele[i].region_z = min(min(vertex[0 + 3 * i].z_value, vertex[1 + 3 * i].z_value), vertex[2 + 3 * i].z_value);
		
		x_length = max(max(vertex[0 + 3 * i].x_value, vertex[1 + 3 * i].x_value), vertex[2 + 3 * i].x_value) - ele[i].region_x;
		y_length = max(max(vertex[0 + 3 * i].y_value, vertex[1 + 3 * i].y_value), vertex[2 + 3 * i].y_value) - ele[i].region_y;
		z_length = max(max(vertex[0 + 3 * i].z_value, vertex[1 + 3 * i].z_value), vertex[2 + 3 * i].z_value) - ele[i].region_z;

		ele[i].region_length = max(max(x_length, y_length), z_length);
		insertEle(&root, ele+i);
	}

	struct Region test;
	test.x = 0;
	test.y = 0;
	test.z = 0;
	test.length = 1;
	queryEles(&root, test);

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
