#ifndef OCT_TREE_H
#define OCT_TREE_H

#define MAX_ELE_NUM 5
#define NULL 0
//octree is a cubic tree. Each length of its eages is the same.
struct Region {
	double x;//x
	double y;
	double z;
	double length;
};

struct Vertex {
	double x_value;
	double y_value;
	double z_value;
};

struct EleFace {
	struct Vertex* vertex0;
	struct Vertex* vertex1;
	struct Vertex* vertex2;
	double region_x;//x
	double region_y;
	double region_z;
	double region_length;
};

struct EleChain {
	struct EleChain* front;
	struct EleChain* rear;
	struct EleFace* data;

};

struct OcTreeNode {
	int depth;	// 0 means root
	double min_length;
    int is_leaf;
    struct Region region;
	struct OcTreeNode *x1y1z1;
    struct OcTreeNode *x1y1z2;
    struct OcTreeNode *x1y2z1;
    struct OcTreeNode *x1y2z2;
	struct OcTreeNode *x2y1z1;
	struct OcTreeNode *x2y1z2;
	struct OcTreeNode *x2y2z1;
	struct OcTreeNode *x2y2z2;
	int ele_num;
	struct EleChain *ele_head;
};

void initNode(struct OcTreeNode* node, int depth, struct Region region, double min_length);

void insertEle(struct OcTreeNode* node, struct EleFace* ele);

void insertChainEle(struct EleChain** head, struct EleFace* ele);

void deleteEle(struct OcTreeNode* node, struct EleFace* ele);

void splitNode(struct OcTreeNode* node);

void combineNode(struct OcTreeNode* node);

void queryEles(struct OcTreeNode* node, struct Region region);

void initRegion(struct Region *region, double x, double y, double z, double length);

struct OcTreeNode *createChildNode(struct OcTreeNode *node, double x, double y, double z, double length);
#endif