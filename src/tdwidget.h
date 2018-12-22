#ifndef TDWIDGET_H
#define TDWIDGET_H

#include <QtOpenGL/qgl.h>
#include "stlloading.h"
#include "octree.h"
#include "glut.h"
#include "GL/GL.h"
#include <QKeyEvent>
#include <QtOpenGL/QGLWidget>
#include <QDebug>
#include <QVector3D>
//#include <Eigen/Dense> 
//#include <QtUiPlugin/QDesignerExportWidget>
//#include <Eigen/OpenGLSupport>

typedef std::vector<EleFace> FaceList;

class TDWidget : public QGLWidget
{
    Q_OBJECT

public:
    
	TDWidget( QWidget* parent = 0, const char* name = 0, bool fs=false);
  ~TDWidget();

public Q_SLOTS:
	bool loadObjObject(QString fileName, QString filePath);
	void SetBoundaryBox(const Vector3f& bmin, const Vector3f& bmax);
	void hideIt();
	void selectFile(QString fileName , QString filePath);
	void deleteFile(QString fileName);
	void showAllFile(QStringList fileName, QStringList filePath, int COI=0);

protected:

  void initializeGL();
  void paintGL();
  void resizeGL( int width, int height );
  void keyPressEvent(QKeyEvent *e );
  void mousePressEvent(QMouseEvent *e);
  void mouseMoveEvent(QMouseEvent *e);
  void wheelEvent(QWheelEvent *e);
  

protected:

  bool fullscreen;

private:
	bool hide=false;
	//记录鼠标事件
   int LastX = 0;
   int LastY = 0;
   double yaw=0;
   double pitch=0;
   QVector3D transVec = QVector3D(0.0f, 0.0f, 0.0f);
   double mdepth=0;

   //记录模型
   std::map<QString, StlFile> stlFileMap;
   std::map<QString, Octree> octreeMap;
   FaceList fList;
  
   //投影参数
   int winWidth, winHeight;                            // window width and height
   double winAspect;
   double sdepth = 10;
   double xpan = 0.0, ypan = 0.0;                      
   double zNear = 1.0, zFar = 100.0;                  
   double g_fov = 45.0;
   double g_sdepth;
   Vector3f g_center;
};

#endif//TDWIDGET_H
