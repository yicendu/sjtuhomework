#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ModelIntersection.h"
#include <QStringList>

class ModelIntersection : public QMainWindow
{
	Q_OBJECT

public:
	ModelIntersection(QWidget *parent = Q_NULLPTR);

private:
	Ui::ModelIntersectionClass ui;
	QStringList fileList;
	QStringList nameList;

private slots:
	//��ť
	void onFindLoadPath();
	void onAddFileToList();
	void showModelOne();
	void showModelTwo();
	void deleteModelOne();
	void deleteModelTwo();
	void onIntersection();
	void onDisplayIntersection();


};
