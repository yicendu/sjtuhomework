#pragma once

#include <vector>
#include <vector3.h>

class Octree;
class EleFace;
typedef std::vector<Vector3f*> PointVector;
typedef std::vector<PointVector> LineVector;

bool cal_intersection(EleFace tria[2], PointVector &point);
LineVector search_inter_lines(Octree *nodeA, Octree *nodeB);