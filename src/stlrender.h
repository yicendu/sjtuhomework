#ifndef STLRENDER_H
#define STLRENDER_H


#include<map>
#include <mutex>
#include<set>
#include "stlreading.h"


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

bool hide(std::string fileName); // ��չʾ����ΪfileName��ģ�ͣ�������ģ�ʹ�����У��

void show(std::string fileName); // ����չʾ����ΪfileName��ģ�ͣ�������ģ�ʹ�����У��

bool addFile(std::string fileName, StlFile stlFile); // ���ģ��

// open gl ���

void demoMenu(int value);

void init();

void reshape(int weight, int height);

void myKeys(int key, int x, int y);

void myMouse(int button, int state, int x, int y);

void myMotion(int x, int y);

void display(void);

void myidle(void);

#endif