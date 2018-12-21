#include "tdwidget.h"
#include <QDebug>

TDWidget::TDWidget(QWidget* parent, const char* name, bool fs)
	: QGLWidget(parent)
{
	if (fullscreen)
		showFullScreen();
}

TDWidget::~TDWidget()
{
}

void TDWidget::initializeGL()
{
	GLfloat light0Position[] = { 0.0f, 0.0f, 0.0f, 1.f };
	GLfloat sunAm[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat sunSp[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat sunDi[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glShadeModel(GL_FLAT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT, GL_DIFFUSE);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sunAm);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDi);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sunSp);
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}



void TDWidget::paintGL()
{
	if (hide)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		return;
	}
		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45.0, winAspect, 0.1, 100.0);
	gluPerspective(g_fov, winAspect, zNear, zFar);
	//glTranslatef(xpan, ypan, -sdepth);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(0.0, 0.0, -6.0);
	glTranslatef(xpan, ypan, -sdepth);
	glTranslatef(transVec[0], transVec[1], transVec[2]);
	glTranslatef(0,0, mdepth);
	glRotatef(pitch, 1.0, 0.0, 0.0);
	glRotatef(yaw, 0.0, 1.0, 0.0);
	glTranslatef(-g_center[0], -g_center[1], -g_center[2]);
	//glShadeModel(GL_FLAT);
	glEnable(GL_LIGHTING);
	glColor3f(0.4, 0.4, 1.0);
	//glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glBegin(GL_TRIANGLES);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (size_t i = 0; i < fList.size(); i++) {
		
		EleFace f = fList[i];
		const Vector3f& pos1 = f.vertex0;
		const Vector3f& pos2 = f.vertex1;
		const Vector3f& pos3 = f.vertex2;
		Vector3f normal = f.normal;
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(pos1.x, pos1.y, pos1.z);
		glVertex3f(pos2.x, pos2.y, pos2.z);
		glVertex3f(pos3.x, pos3.y, pos3.z);
	}
	glEnd();
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void TDWidget::resizeGL(int width, int height)
{
	if (height == 0)
	{
		height = 1;
	}
	winWidth = width;
	winHeight = height;
	winAspect = (double)width / (double)height;
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glTranslatef(xpan, ypan, -10);
	gluPerspective(g_fov, winAspect, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void TDWidget::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_F2:
		fullscreen = !fullscreen;
		if (fullscreen)
		{
			showFullScreen();
		}
		else
		{
			showNormal();
			setGeometry(0, 0, 640, 480);
		}
		updateGL();
		break;
	case Qt::Key_Escape:
		close();
	}
}

void TDWidget::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton|| e->button() == Qt::RightButton)
	{
		LastX = e->x();
		LastY = e->y();
	}
	

	updateGL();
}


void TDWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (e->buttons() & Qt::LeftButton)
	{
		/*sphi += (double)(e->x() - LastX) / 4.0;
		stheta += (double)(LastY - e->y()) / 4.0;*/
		GLfloat xoffset = e->x() - LastX;
		GLfloat yoffset = e->y() - LastY;
		LastX = e->x();
		LastY = e->y();

		GLfloat sensitivity = 0.4f;     //旋转时的灵敏度  
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		//可以用来设置俯仰角的上下界  
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
	else if (e->buttons() == Qt::RightButton)
	{
		//计算x,y方向的偏移量  
		GLfloat xoffset = e->x() - LastX;
		GLfloat yoffset = e->y() - LastY;
		LastX = e->x();
		LastY = e->y();

		GLfloat sensitivity = 0.01f;    //移动时的灵敏度  
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		//仅需在x-y平面内移动即可  
		transVec += QVector3D(xoffset, -yoffset, 0.0f);
	}

	updateGL();
}

void TDWidget::wheelEvent(QWheelEvent *e)
{
	// 当滚轮远离使用者时
	if (e->delta() > 0) {
		mdepth += (double)e->delta()/100;
	}
	else {//当滚轮向使用者方向旋转时
		mdepth += (double)e->delta()/100;
	}
	updateGL();
}

bool TDWidget::loadObjObject(QString fileName, QString filePath)
{
	//std::cout << "debug" << std::endl;
	if (stlFileMap.find(fileName) != stlFileMap.end())
	{
		return false; // 模型已存在
	}

	char tmp[100];
	QByteArray c = filePath.toLocal8Bit();
	strcpy(tmp, c.data());
	stlFileMap[fileName] = slt_read(tmp);
	
	return true;
}


void TDWidget::SetBoundaryBox(const Vector3f& bmin, const Vector3f& bmax) {
	double PI = 3.14159265358979323846;
	double radius = (bmax - bmin).L2Norm();
	g_center = 0.5 * (bmin + bmax);
	zNear = 0.2 * radius / sin(0.5 * g_fov * PI / 180.0);
	zFar = zNear + 2.0 * radius;
	g_sdepth = zNear + radius;
	zNear *= 0.1;
	zFar *= 10;
	sdepth = g_sdepth;
}

void TDWidget::hideIt()
{
	hide = !hide;
	updateGL();
}

void TDWidget::showAllFile(QStringList fileName, QStringList filePath,int COI)
{
	fList.clear();
	for (int i = 0; i < fileName.size(); i++)
	{
		const char* tmp = fileName[i].toUtf8();
		loadObjObject(fileName[i], filePath[i]);
		if (fList.empty())
			fList = stlFileMap[fileName[i]].faces;
		else
			fList.insert(fList.end(), stlFileMap[fileName[i]].faces.begin(), stlFileMap[fileName[i]].faces.end());
	}
	SetBoundaryBox(stlFileMap[fileName[COI]].MinCoord(), stlFileMap[fileName[COI]].MaxCoord());
	updateGL();
	stlFileMap.clear();
}

void TDWidget::selectFile(QString fileName, QString filePath)
{
	loadObjObject(fileName,filePath);
	fList = stlFileMap[fileName].faces;
	SetBoundaryBox(stlFileMap[fileName].MinCoord(), stlFileMap[fileName].MaxCoord());
	updateGL();
	deleteFile(fileName);
}

void TDWidget::deleteFile(QString fileName)
{
	std::map<QString, StlFile>::iterator it;
	it = stlFileMap.find(fileName);
	if ( it == stlFileMap.end())
	{
		return; 
	}
	stlFileMap.erase(it);
	return;
}