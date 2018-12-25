#ifndef STLRENDER_H
#define STLRENDER_H


#include<map>
#include <mutex>
#include<set>
#include "stlloading.h"


class StlRender {
public:
	
	void startup();

	static StlRender * getInstance() {
		if (p == NULL)
			p = new StlRender();
		return p;
	}
	
	
protected:

	StlRender() {
		
	}; 


private:
	static StlRender * p;
};

extern std::map<std::string, StlFile> stlFileMap;

extern std::set<std::string> fileToRender;

bool hide(std::string fileName); // 不展示名称为fileName的模型，不进行模型存在性校验

void show(std::string fileName); // 重新展示名称为fileName的模型，不进行模型存在性校验

bool addFile(std::string fileName, StlFile stlFile); // 添加模型

// open gl 相关

void demoMenu(int value);

void init();

void reshape(int weight, int height);

void myKeys(int key, int x, int y);

void myMouse(int button, int state, int x, int y);

void myMotion(int x, int y);

void display(void);

void myidle(void);

#endif