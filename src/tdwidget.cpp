#include "tdwidget.h"


TDWidget::TDWidget(QWidget* parent, const char* name, bool fs) :QGLWidget(parent) {
	name = name;
	fullscreen = fs;
	if (fullscreen)
		showFullScreen();
}

TDWidget::~TDWidget()
{
}

void TDWidget::initializeGL()
{
	GLfloat light0Position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//GLfloat sunAm[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//GLfloat sunSp[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat sunDi[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glShadeModel(GL_FLAT);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT, GL_DIFFUSE);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, sunAm);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDi);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, sunSp);
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
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
	GLfloat width = 1;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45.0, winAspect, 0.1, 100.0);
	gluPerspective(g_fov, winAspect, zNear, zFar);
	//glTranslatef(xpan, ypan, -sdepth);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(0.0, 0.0, -6.0);
	//glTranslatef(xpan, ypan, -sdepth);

	//glTranslatef(transVec[0] - transVec_old[0], transVec[1] - transVec_old[1], transVec[2] - transVec_old[2]);
	//glTranslatef(0, 0, mdepth-mdepth_old);
	//glRotatef(pitch- pitch_old, 1.0, 0.0, 0.0);
	//glRotatef(yaw-yaw_old, 0.0, 1.0, 0.0);
	//glTranslatef(0, 0, -(transVec[2] + mdepth));
	//glRotatef(pitch, 1.0, 0.0, 0.0);
	//glRotatef(yaw, 0.0, 1.0, 0.0);
	//glTranslatef(-g_center[0], -g_center[1], -g_center[2]);
	//glShadeModel(GL_FLAT);

	/*这里是坐标系转换的表达，现在是绕视野中心旋转，
	如果你只想绕物体自身中心旋转，就把后面的代码注释掉（注释到坐标转换结束的位置），并去掉下面四行代码的注释*/
	//glTranslatef(transVec[0], transVec[1], transVec[2] + mdepth);
	//glRotatef(pitch, 1.0, 0.0, 0.0);
	//glRotatef(yaw, 0.0, 1.0, 0.0);
	//glTranslatef(-g_center[0], -g_center[1], -g_center[2]);
	if (m_flag)
	{
		glTranslatef(transVec[0], transVec[1], transVec[2] + mdepth);
		//glTranslatef(0, 0, mdepth);

		glRotatef(pitch, 1.0, 0.0, 0.0);
		glRotatef(yaw, 0.0, 1.0, 0.0);

		glTranslatef(-g_center[0], -g_center[1], -g_center[2]);
		glGetFloatv(GL_MODELVIEW_MATRIX, model);
	}
	glLoadIdentity();
	if(!m_flag)
		glTranslatef(transVec[0] - transVec_old[0], transVec[1] - transVec_old[1], transVec[2] - transVec_old[2] + mdepth - mdepth_old);
	glTranslatef(0, 0, (transVec_old[2] + mdepth_old));
	glRotatef(pitch- pitch_old, 1.0, 0.0, 0.0);
	glRotatef(yaw- yaw_old, 0.0, 1.0, 0.0);
	glTranslatef(0, 0, -(transVec_old[2] + mdepth_old));
	glMultMatrixf(model);
	m_flag = false;
	glGetFloatv(GL_MODELVIEW_MATRIX, model);


	transVec_old = transVec;
	mdepth_old = mdepth;
	yaw_old = yaw;
	pitch_old = pitch;
	/*坐标转换结束*/

	glEnable(GL_LIGHTING);
	
	//glColorMaterial(GL_FRONT, GL_DIFFUSE);
	//glBegin(GL_LINES);
	////glColor4f(0, 0, 0, 1);
	//glVertex3f(0, 0, 0);
	//glVertex3f(g_center[0], g_center[1], g_center[2]);
	//glVertex3f(g_center[0], g_center[1], g_center[2]);
	//glVertex3f(transVec[0], transVec[1],0);
	//glVertex3f(0, 0, 0);
	//glColor4f(1, 0, 0, 1);
	//glVertex3f(100, 0, 0);
	//glColor4f(0, 1, 0 , 1);
	//glVertex3f(0, 0, 0);
	//glColor4f(0, 0, 1.0, 1);
	//glVertex3f(0, 0, 100);
	//glEnd();
	glColor4f(0.4, 0.4, 1.0, 0.8);
	glEnable(GL_DEPTH_TEST);
	glBegin(GL_TRIANGLES);
	glFrontFace(GL_CCW);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (size_t i = 0; i < fList.size(); i++) {
		
		EleFace f = fList[i];
		const Vector3f& pos1 = f.vertex0;
		const Vector3f& pos2 = f.vertex1;
		const Vector3f& pos3 = f.vertex2;
		Vector3f normal = f.normal/ f.normal.L2Norm();
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(pos1.x, pos1.y, pos1.z);
		glVertex3f(pos2.x, pos2.y, pos2.z);
		glVertex3f(pos3.x, pos3.y, pos3.z);
	}
	glEnd();
	glColor4f(0.0, 0.0, 0.0,1.0);
	
	glDisable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonOffset(-1.0, -1.0);
	glEnable(GL_LINE_SMOOTH);
	width = 5.0;
	glLineWidth(width);
	glBegin(GL_LINES);
	for (int i = 0; i < llist.size(); i++)
	{
		glVertex3f(llist[i][0]->x, llist[i][0]->y, llist[i][0]->z);
		glVertex3f(llist[i][1]->x, llist[i][1]->y, llist[i][1]->z);
	}
	glEnd();

	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

		GLfloat sensitivity = (zFar-zNear)/geometry().width()/30;    //移动时的灵敏度  
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		//仅需在x-y平面内移动即可  
		transVec += QVector3D(xoffset, -yoffset, 0.f);
	}

	updateGL();
}

void TDWidget::wheelEvent(QWheelEvent *e)
{
	// 当滚轮远离使用者时
	GLfloat sensitivity = (zFar - zNear) / 700;
	if (e->delta() > 0) {
		mdepth += (double)e->delta()/ sensitivity;
	}
	else {//当滚轮向使用者方向旋转时
		mdepth += (double)e->delta()/ sensitivity;
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
	StlFile stl;
	stlFileMap[fileName] = stl;
	octreeMap[fileName] = Octree(&stlFileMap[fileName], 2.f,10);
	return true;
}

bool TDWidget::intersection(QString fileName1, QString filePath1, QString fileName2, QString filePath2)
{
	fList.clear();
	llist.clear();
	loadObjObject(fileName1, filePath1);
	loadObjObject(fileName2, filePath2);
	SetBoundaryBox(stlFileMap[fileName1].MinCoord(), stlFileMap[fileName1].MaxCoord());
	llist = search_inter_lines(&octreeMap[fileName1], &octreeMap[fileName2]);
	fList = stlFileMap[fileName1].faces;
	fList.insert(fList.end(), stlFileMap[fileName2].faces.begin(), stlFileMap[fileName2].faces.end());
	updateGL();
	deleteFile(fileName1);
	deleteFile(fileName2);
	return true;
}

void TDWidget::SetBoundaryBox(const Vector3f& bmin, const Vector3f& bmax) {
	double PI = 3.14159265358979323846;
	double radius = (bmax - bmin).L2Norm();
	g_center = 0.5 * (bmin + bmax);
	zNear = 0.3 * radius / sin(0.5 * g_fov * PI / 180.0);
	zFar = zNear + 2.0 * radius;
	g_sdepth = zNear + radius;
	zNear *= 0.05;
	zFar *= 10;
	sdepth = g_sdepth;
	transVec[2] = -sdepth;
	m_flag = true;
}

void TDWidget::hideIt()
{
	hide = !hide;
	updateGL();
}

inline Vector3f minVector3f(Vector3f a, Vector3f b)
{
	return Vector3f(std::min(a.x, b.x),
		std::min(a.y, b.y),
		std::min(a.z, b.z));
}

inline Vector3f maxVector3f(Vector3f a, Vector3f b)
{
	return Vector3f(std::max(a.x, b.x),
		std::max(a.y, b.y),
		std::max(a.z, b.z));
}

void TDWidget::showAllFile(QStringList fileName, QStringList filePath, int COI)
{
	fList.clear();
	for (int i = 0; i < fileName.size(); i++)
	{
		loadObjObject(fileName[i], filePath[i]);
		if (fList.empty())
			fList = stlFileMap[fileName[i]].faces;
		else
			fList.insert(fList.end(), stlFileMap[fileName[i]].faces.begin(), stlFileMap[fileName[i]].faces.end());
	}
	Vector3f mincoord= stlFileMap[fileName[COI]].MinCoord();
	Vector3f maxcoord= stlFileMap[fileName[COI]].MaxCoord();
	for (int i = 0; i < fileName.size(); i++)
	{
		mincoord = minVector3f(mincoord, stlFileMap[fileName[i]].MinCoord());
		maxcoord = maxVector3f(maxcoord, stlFileMap[fileName[i]].MaxCoord());
	}

	SetBoundaryBox(mincoord, maxcoord);
	//SetBoundaryBox(octreeMap[fileName[COI]].m_region.min, octreeMap[fileName[COI]].m_region.max);
	updateGL();
	stlFileMap.clear();
}

void TDWidget::selectFile(QString fileName, QString filePath)
{
	loadObjObject(fileName,filePath);
	fList = stlFileMap[fileName].faces;
	SetBoundaryBox(stlFileMap[fileName].MinCoord(), stlFileMap[fileName].MaxCoord());
	//SetBoundaryBox(octreeMap[fileName].m_region.min, octreeMap[fileName].m_region.max);
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