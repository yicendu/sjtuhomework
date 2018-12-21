#include "ModelIntersection.h"
#include <QFileDialog>
#include <QMessageBox>

ModelIntersection::ModelIntersection(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.actionFile, SIGNAL(triggered()), SLOT(onFindLoadPath()));
	connect(ui.FileButton, SIGNAL(clicked()), SLOT(onFindLoadPath()));
	connect(ui.loadButton, SIGNAL(clicked()), SLOT(onAddFileToList()));
	connect(ui.previewButton1, SIGNAL(clicked()), SLOT(showModelOne()));
	connect(ui.deleteButton1, SIGNAL(clicked()), SLOT(deleteModelOne()));
	connect(ui.previewButton2, SIGNAL(clicked()), SLOT(showModelTwo()));
	connect(ui.deleteButton2, SIGNAL(clicked()), SLOT(deleteModelTwo()));
	connect(ui.pushButton_4, SIGNAL(clicked()), SLOT(onDisplayIntersection()));
}
void ModelIntersection::onFindLoadPath()
{
	QFileDialog *fileDialog = new QFileDialog(this);
	//�����ļ��Ի������
	fileDialog->setWindowTitle(tr("Open a File"));
	//����Ĭ���ļ�·��
	fileDialog->setDirectory(".");
	//�����ļ�������
	fileDialog->setNameFilter(tr("Models(*.obj *.stl)"));
	//���ÿ���ѡ�����ļ�,Ĭ��Ϊֻ��ѡ��һ���ļ�QFileDialog::ExistingFiles
	fileDialog->setFileMode(QFileDialog::ExistingFile);
	//������ͼģʽ
	fileDialog->setViewMode(QFileDialog::Detail);
	//��ӡ����ѡ����ļ���·��
	QStringList fileName;
	if (fileDialog->exec())
	{
		fileName<<fileDialog->selectedFiles();
	}
	for (auto tmp : fileName)
		ui.lineEdit->setText(tmp);
}

void ModelIntersection::onAddFileToList()
{
	if (ui.lineEdit->text().isEmpty())
	{
		QMessageBox::critical(0,
			"critical message", "Please select model file!",
			QMessageBox::Ok | QMessageBox::Default,
			QMessageBox::Cancel | QMessageBox::Escape, 0);
	}
	else {
		QFileInfo currentFile = ui.lineEdit->text();
		fileList << currentFile.absoluteFilePath();
		nameList << currentFile.fileName();
		ui.comboBox1->addItem(currentFile.fileName(), currentFile.absoluteFilePath());
		ui.comboBox2->addItem(currentFile.fileName(), currentFile.absoluteFilePath());
	}
}

void ModelIntersection::showModelOne()
{
	if (ui.comboBox1->currentIndex()<0)
	{
		QMessageBox::critical(0,
			"critical message", "Please select model 1!",
			QMessageBox::Ok | QMessageBox::Default,
			QMessageBox::Cancel | QMessageBox::Escape, 0);
	}
	else {
		QString tmp_name = ui.comboBox1->currentText();
		QString tmp_path = ui.comboBox1->currentData().toString();
		ui.textBrowser->append("Model 1 is " + tmp_name);
		ui.openGLWidget_2->selectFile(tmp_name.toUtf8(),tmp_path.toUtf8());
	}
}

void ModelIntersection::showModelTwo()
{
	if (ui.comboBox2->currentIndex()<0)
	{
		QMessageBox::critical(0,
			"critical message", "Please select model 2!",
			QMessageBox::Ok | QMessageBox::Default,
			QMessageBox::Cancel | QMessageBox::Escape, 0);
	}
	else {
		QString tmp_name = ui.comboBox2->currentText();
		QString tmp_path = ui.comboBox2->currentData().toString();
		ui.textBrowser->append("Model 2 is " + tmp_name);
		//ui.openGLWidget_3->loadObjObject(tmp.toUtf8());
		ui.openGLWidget_3->selectFile(tmp_name.toUtf8(), tmp_path.toUtf8());
	}
}

void ModelIntersection::deleteModelOne()
{
	if (ui.comboBox1->currentIndex()<0)
	{
		QMessageBox::critical(0,
			"critical message", "Please select model 1!",
			QMessageBox::Ok | QMessageBox::Default,
			QMessageBox::Cancel | QMessageBox::Escape, 0);
	}
	else {
		int deleteFile = ui.comboBox1->currentIndex();
		QString deleteFileName = ui.comboBox1->currentText();
		ui.comboBox1->removeItem(deleteFile);
		ui.comboBox2->removeItem(deleteFile);
		ui.openGLWidget_2->deleteFile(deleteFileName.toUtf8());
		ui.openGLWidget_3->deleteFile(deleteFileName.toUtf8());

	}
}

void ModelIntersection::deleteModelTwo()
{
	if (ui.comboBox2->currentIndex()<0)
	{
		QMessageBox::critical(0,
			"critical message", "Please select model 2!",
			QMessageBox::Ok | QMessageBox::Default,
			QMessageBox::Cancel | QMessageBox::Escape, 0);
	}
	else {
		int deleteFile = ui.comboBox2->currentIndex();
		QString deleteFileName = ui.comboBox1->currentText();
		ui.comboBox1->removeItem(deleteFile);
		ui.comboBox2->removeItem(deleteFile);
		ui.openGLWidget_2->deleteFile(deleteFileName.toUtf8());
		ui.openGLWidget_3->deleteFile(deleteFileName.toUtf8());
	}
}

void ModelIntersection::onIntersection()
{

}

void ModelIntersection::onDisplayIntersection()
{
	QStringList fileName;
	QStringList filePath;
	QString tmp_name = ui.comboBox1->currentText();
	QString tmp_path = ui.comboBox1->currentData().toString();
	fileName<<tmp_name;
	filePath<<tmp_path;

	tmp_name = ui.comboBox2->currentText();
	tmp_path = ui.comboBox2->currentData().toString();
	fileName<<tmp_name;
	filePath<<tmp_path;
	
	ui.openGLWidget->showAllFile(fileName, filePath);

}
