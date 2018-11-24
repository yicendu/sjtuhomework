#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "octree.h"

/**
 * insertEle
 * 1.Check if it's splitted, For those splitted node,
 *   choose the appropriate child node.
 * 2.Check if the leaf node is overloaded. If overloaded,
 *   split the node and insert the eles to the child nodes.
 * 3.Directly add into the unoverloaded node.
 *
 * @param ptr node
 * @param ptr ele
 */
void insertEle(struct OcTreeNode *node, struct EleFace *ele) {
	if (1 == node->is_leaf) {
		if (node->ele_num + 1 > MAX_ELE_NUM && node->region.length > node->min_length) {
			splitNode(node);
			insertEle(node, ele);
		}
		else {
			insertChainEle(&node->ele_head, ele);
			node->ele_num++;
		}

		return;
	}

	double mid_x = node->region.x + node->region.length / 2;
	double mid_y = node->region.y + node->region.length / 2;
	double mid_z = node->region.z + node->region.length / 2;
	double x1 = ele->region_x;
	double x2 = x1 + ele->region_length / 2;
	double y1 = ele->region_y;
	double y2 = y1 + ele->region_length / 2;
	double z1 = ele->region_z;
	double z2 = z1 + ele->region_length / 2;

	if (x1 < mid_x) {
		if (y1 < mid_y) {
			if (z1 < mid_z) {
				insertEle(node->x1y1z1, ele);
			}
			if (z2 > mid_z) {
				insertEle(node->x1y1z2, ele);
			}
		}
		if (y2 > mid_y) {
			if (z1 < mid_z) {
				insertEle(node->x1y2z1, ele);
			}
			if (z2 > mid_z) {
				insertEle(node->x1y2z2, ele);
			}
		}
	}
	if (x2 > mid_x) {
		if (y1 < mid_y) {
			if (z1 < mid_z) {
				insertEle(node->x2y1z1, ele);
			}
			if (z2 > mid_z) {
				insertEle(node->x2y1z2, ele);
			}
		}
		if (y2 > mid_y) {
			if (z1 < mid_z) {
				insertEle(node->x2y2z1, ele);
			}
			if (z2 > mid_z) {
				insertEle(node->x2y2z2, ele);
			}
		}
	}
}


void insertChainEle(struct EleChain** head, struct EleFace* ele) {
	if (*head == NULL) {
		*head = (struct EleChain*) malloc(sizeof(struct EleChain));
		(*head)->data = ele;
		(*head)->front = NULL;
		(*head)->rear = NULL;
		return;
	}
	struct EleChain* head_temp = *head;
	while (head_temp->rear) head_temp = head_temp->rear;
	head_temp->rear = (struct EleChain*) malloc(sizeof(struct EleChain));
	head_temp->rear->front = head_temp;
	head_temp->rear->data = ele;
	head_temp->rear->rear = NULL;
}

/**
 * splitNode
 * 1.Access the depth and region of the parent node.
 * 2.Fork into 8 child nodes.
 *
 * @param ptr node
 */
void splitNode(struct OcTreeNode *node) {
	double child_lenght = node->region.length / 2;
	double mid_x = node->region.x + child_lenght;
	double mid_y = node->region.y + child_lenght;
	double mid_z = node->region.z + child_lenght;

	node->is_leaf = 0;
	node->x1y1z1 = createChildNode(node, node->region.x, node->region.y, node->region.z, child_lenght);
	node->x1y1z2 = createChildNode(node, node->region.x, node->region.y, mid_z, child_lenght);
	node->x1y2z1 = createChildNode(node, node->region.x, mid_y, node->region.z, child_lenght);
	node->x1y2z2 = createChildNode(node, node->region.x, mid_y, mid_z, child_lenght);
	node->x2y1z1 = createChildNode(node, mid_x, node->region.y, node->region.z, child_lenght);
	node->x2y1z2 = createChildNode(node, mid_x, node->region.y, mid_z, child_lenght);
	node->x2y2z1 = createChildNode(node, mid_x, mid_y, node->region.z, child_lenght);
	node->x2y2z2 = createChildNode(node, mid_x, mid_y, mid_z, child_lenght);

	while (node->ele_head) {
		insertEle(node, node->ele_head->data);
		if (node->ele_head->rear) {
			node->ele_head = node->ele_head->rear;
			free(node->ele_head->front);
			node->ele_head->front = NULL;
		}
		else {
			free(node->ele_head);
			node->ele_head = NULL;
		}
	}
	node->ele_num = 0;
}

struct OcTreeNode *createChildNode(struct OcTreeNode *node, double x, double y, double z, double length) {
	int depth = node->depth + 1;
	struct OcTreeNode *childNode = (struct OcTreeNode *) malloc(sizeof(struct OcTreeNode));
	struct Region *region = (struct Region *) malloc(sizeof(struct Region));
	initRegion(region, x, y, z, length);
	initNode(childNode, depth, *region, node->min_length);
	childNode->min_length = node->min_length;
	return childNode;
}

/**
 * deleteEle
 */
void deleteEle(struct OcTreeNode *node, struct EleFace *ele) {

}

/**
 * combineNode
 */
void combineNode(struct OcTreeNode *node) {

}

void queryEles(struct OcTreeNode* node, struct Region region) {
	if (node->is_leaf == 1) {
		printf("There are %d faces. i.e.\n", node->ele_num);
		
		struct EleChain* head = node->ele_head;
		for (int j = 0; j < node->ele_num; j++) {
			printf("Face index%d\n", j);
			printf("Vertex1:%f,%f,%f\n", head->data->vertex0->x_value, head->data->vertex0->y_value, head->data->vertex0->z_value);
			printf("Vertex2:%f,%f,%f\n", head->data->vertex1->x_value, head->data->vertex1->y_value, head->data->vertex1->z_value);
			printf("Vertex3:%f,%f,%f\n", head->data->vertex2->x_value, head->data->vertex2->y_value, head->data->vertex2->z_value);
			head = head->rear;
		}
		
		return;
	}

	double child_lenght = node->region.length / 2;
	double mid_x = node->region.x + child_lenght;
	double mid_y = node->region.y + child_lenght;
	double mid_z = node->region.z + child_lenght;
	double x1 = region.x;
	double x2 = x1 + region.length;
	double y1 = region.y;
	double y2 = y1 + region.length;
	double z1 = region.z;
	double z2 = z1 + region.length;

	if (x1 < mid_x) {
		if (y1 < mid_y) {
			if (z1 < mid_z) {
				queryEles(node->x1y1z1, region);
			}
			if (z2 > mid_z) {
				queryEles(node->x1y1z2, region);
			}
		}
		if (y2 > mid_y) {
			if (z1 < mid_z) {
				queryEles(node->x1y2z1, region);
			}
			if (z2 > mid_z) {
				queryEles(node->x1y2z2, region);
			}
		}
	}
	if (x2 > mid_x) {
		if (y1 < mid_y) {
			if (z1 < mid_z) {
				queryEles(node->x2y1z1, region);
			}
			if (z2 > mid_z) {
				queryEles(node->x2y1z2, region);
			}
		}
		if (y2 > mid_y) {
			if (z1 < mid_z) {
				queryEles(node->x2y2z1, region);
			}
			if (z2 > mid_z) {
				queryEles(node->x2y2z2, region);
			}
		}
	}

}

void initNode(struct OcTreeNode *node, int depth, struct Region region, double min_length) {
	node->depth = depth;
	node->is_leaf = 1;
	node->ele_num = 0;
	node->region = region;
	node->min_length = min_length;
	node->ele_head = NULL;
}

void initRegion(struct Region *region, double x, double y, double z, double length) {
	region->x = x;
	region->y = y;
	region->z = z;
	region->length = length;
}
