#include "ModelIntersection.h"
#include <QtWidgets/QApplication>
#include <QmessageBox>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ModelIntersection w;
	w.show();
	return a.exec();
}
