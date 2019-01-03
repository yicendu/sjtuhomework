#pragma once

#include <vector>
#include <vector3.h>
class Octree;
class EleFace;
typedef std::vector<std::vector<Vector3f*>> LineVector;
bool cal_intersection(EleFace tria[2], std::vector<Vector3f*> &point);
LineVector search_inter_lines(Octree *nodeA, Octree *nodeB);