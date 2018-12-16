#include "stlrender.h"
#include<gl\GLUT.H>

#include<iostream>

std::map<std::string, StlFile> stlFileMap;

std::set<std::string> fileToRender;

float angle_x = 0.0;
float angle_y = 0.0;
float eyez = 350.0;
float eyex = 0.0;
float eyey = 0.0;

bool lMouseDown = false;
bool rMouseDown = false;

float xMouse = 0.0f;
float yMouse = 0.0f;

void demoMenu(int value)
{
	switch (value)
	{
	case 1:{
		StlFile stlFile = slt_read("yellowcat.stl");
		addFile("yellowcat.stl", stlFile);
		break;
	}
	default:
		break;
	}
	glutPostRedisplay();
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); //背景黑色 
}

StlRender * StlRender::p = NULL;

void display(void)
{
	
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LEQUAL);

	glColor3f(1.0, 1.0, 1.0); //画笔白色  

	glLoadIdentity();  //加载单位矩阵 

	gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, 1, 0);
	glRotatef(angle_x, 1.0f, 0.0f, 0.0f);
	glRotatef(angle_y, 0.0f, 1.0f, 0.0f);
	// define sun light
	GLfloat sunPos[] = { 50.0f, 250.0f, 150.0f, 1.0f };
	GLfloat sunAm[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat sunSp[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat sunDi[] = { 1.0f, 1.0f, 1.0f, 1.0f };


	glLightfv(GL_LIGHT1, GL_POSITION, sunPos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, sunAm);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, sunDi);
	glLightfv(GL_LIGHT1, GL_SPECULAR, sunSp);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	if (!fileToRender.empty())
	{
		std::set<std::string>::iterator it;
		for (it = fileToRender.begin(); it != fileToRender.end(); it++) {
			StlFile stlFile = stlFileMap[*it];
			
			glPushMatrix();
			for (size_t i = 0; i < stlFile.faces.size(); i++)
			{
				glFrontFace(GL_CCW);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				glBegin(GL_TRIANGLES);
				EleFace face = stlFile.faces[i];
				glNormal3f(face.normal.x, face.normal.y, face.normal.z);

				glVertex3f(face.vertex0.x, face.vertex0.y, face.vertex0.z);
				glVertex3f(face.vertex1.x, face.vertex1.y, face.vertex1.z);
				glVertex3f(face.vertex2.x, face.vertex2.y, face.vertex2.z);
				glEnd();
			}
			glPopMatrix();
		}
	}

	
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glutSwapBuffers();
}

void myidle(void)
{
	display();
}


void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// 键位操作说明：鼠标拖动（上下左右）旋转视角 pgUp pgDw拉近（远）相机位置
void myKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP){
		angle_x += 5.0f;
		if (angle_x > 355.0f)angle_x = 0.0f;
	}

	if (key == GLUT_KEY_DOWN){
		angle_x -= 5.0f;
		if (angle_x < 0)angle_x = 355.0f;
	}

	if (key == GLUT_KEY_LEFT){
		angle_y += 5.0;
		if (angle_y>355.0f)angle_y = 0.0f;
	}
	if (key == GLUT_KEY_RIGHT){
		angle_y -= 5.0;
		if (angle_y < 0)angle_y = 355.0f;
	}
	if (key == GLUT_KEY_PAGE_DOWN)eyez -= 10.0f;
	if (key == GLUT_KEY_PAGE_UP)eyez += 10.0f;
	glutPostRedisplay();
}



void myMouse(int button, int state, int x, int y){
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_RIGHT_BUTTON) rMouseDown = true;
		if (button == GLUT_LEFT_BUTTON) lMouseDown = true;
	}
	else
	{
		if (button == GLUT_RIGHT_BUTTON) rMouseDown = false;
		if (button == GLUT_LEFT_BUTTON) lMouseDown = false;
	}
	xMouse = x;
	yMouse = y;
}

void myMotion(int x, int y){
	if (lMouseDown){
		angle_y += (x - xMouse);
		angle_x += (y - yMouse);
	}
	xMouse = x;
	yMouse = y;

}



void StlRender::startup() {
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(750, 100);
	glutCreateWindow("");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutIdleFunc(myidle);


	glutSpecialFunc(myKeys);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);

	glutCreateMenu(demoMenu);
	glutAddMenuEntry("DEMO:加载模型", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}

bool addFile(std::string fileName, StlFile file) {
	std::cout << "debug" << std::endl;
	if (stlFileMap.find(fileName) != stlFileMap.end())
	{
		return false; // 模型已存在
	}
	stlFileMap[fileName] = file;
	fileToRender.insert(fileName);
	return true;
}

bool hide(std::string fileName) {
	int affectNum = fileToRender.erase(fileName);
	return affectNum > 0;
}

void show(std::string fileName) {
	fileToRender.insert(fileName);
}