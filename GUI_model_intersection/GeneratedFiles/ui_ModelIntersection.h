/********************************************************************************
** Form generated from reading UI file 'ModelIntersection.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELINTERSECTION_H
#define UI_MODELINTERSECTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "tdwidget.h"

QT_BEGIN_NAMESPACE

class Ui_ModelIntersectionClass
{
public:
    QAction *actionFile;
    QAction *closeAction;
    QWidget *centralWidget;
    QGridLayout *gridLayout_4;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QPushButton *intersectButton;
    QPushButton *pushButton_4;
    QGroupBox *gridGroupBox_2;
    QGridLayout *gridLayout_3;
    QPushButton *previewButton1;
    QPushButton *deleteButton1;
    QLabel *label;
    QComboBox *comboBox1;
    QComboBox *comboBox2;
    QLabel *label_2;
    QPushButton *previewButton2;
    QPushButton *deleteButton2;
    QGroupBox *horizontalGroupBox;
    QHBoxLayout *horizontalLayout;
    TDWidget *openGLWidget_2;
    TDWidget *openGLWidget_3;
    QGroupBox *gridGroupBox;
    QGridLayout *gridLayout_2;
    QPushButton *FileButton;
    QPushButton *loadButton;
    QLineEdit *lineEdit;
    QGroupBox *gridGroupBox1;
    QGridLayout *gridLayout;
    TDWidget *openGLWidget;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_5;
    QTextBrowser *textBrowser;
    QMenuBar *menuBar;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ModelIntersectionClass)
    {
        if (ModelIntersectionClass->objectName().isEmpty())
            ModelIntersectionClass->setObjectName(QStringLiteral("ModelIntersectionClass"));
        ModelIntersectionClass->resize(1024, 768);
        actionFile = new QAction(ModelIntersectionClass);
        actionFile->setObjectName(QStringLiteral("actionFile"));
        actionFile->setShortcutVisibleInContextMenu(true);
        closeAction = new QAction(ModelIntersectionClass);
        closeAction->setObjectName(QStringLiteral("closeAction"));
        centralWidget = new QWidget(ModelIntersectionClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_4 = new QGridLayout(centralWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        intersectButton = new QPushButton(frame);
        intersectButton->setObjectName(QStringLiteral("intersectButton"));
        intersectButton->setMaximumSize(QSize(2000, 30));

        verticalLayout->addWidget(intersectButton);

        pushButton_4 = new QPushButton(frame);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setMaximumSize(QSize(20000, 30));

        verticalLayout->addWidget(pushButton_4);


        gridLayout_4->addWidget(frame, 3, 1, 1, 1);

        gridGroupBox_2 = new QGroupBox(centralWidget);
        gridGroupBox_2->setObjectName(QStringLiteral("gridGroupBox_2"));
        gridLayout_3 = new QGridLayout(gridGroupBox_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        previewButton1 = new QPushButton(gridGroupBox_2);
        previewButton1->setObjectName(QStringLiteral("previewButton1"));

        gridLayout_3->addWidget(previewButton1, 2, 0, 1, 1);

        deleteButton1 = new QPushButton(gridGroupBox_2);
        deleteButton1->setObjectName(QStringLiteral("deleteButton1"));

        gridLayout_3->addWidget(deleteButton1, 2, 1, 1, 1);

        label = new QLabel(gridGroupBox_2);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMaximumSize(QSize(16777215, 20));

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        comboBox1 = new QComboBox(gridGroupBox_2);
        comboBox1->setObjectName(QStringLiteral("comboBox1"));

        gridLayout_3->addWidget(comboBox1, 1, 0, 1, 2);

        comboBox2 = new QComboBox(gridGroupBox_2);
        comboBox2->setObjectName(QStringLiteral("comboBox2"));

        gridLayout_3->addWidget(comboBox2, 4, 0, 1, 2);

        label_2 = new QLabel(gridGroupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(16777215, 20));

        gridLayout_3->addWidget(label_2, 3, 0, 1, 1);

        previewButton2 = new QPushButton(gridGroupBox_2);
        previewButton2->setObjectName(QStringLiteral("previewButton2"));

        gridLayout_3->addWidget(previewButton2, 5, 0, 1, 1);

        deleteButton2 = new QPushButton(gridGroupBox_2);
        deleteButton2->setObjectName(QStringLiteral("deleteButton2"));

        gridLayout_3->addWidget(deleteButton2, 5, 1, 1, 1);


        gridLayout_4->addWidget(gridGroupBox_2, 1, 1, 1, 1);

        horizontalGroupBox = new QGroupBox(centralWidget);
        horizontalGroupBox->setObjectName(QStringLiteral("horizontalGroupBox"));
        horizontalLayout = new QHBoxLayout(horizontalGroupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        openGLWidget_2 = new TDWidget(horizontalGroupBox);
        openGLWidget_2->setObjectName(QStringLiteral("openGLWidget_2"));

        horizontalLayout->addWidget(openGLWidget_2);

        openGLWidget_3 = new TDWidget(horizontalGroupBox);
        openGLWidget_3->setObjectName(QStringLiteral("openGLWidget_3"));

        horizontalLayout->addWidget(openGLWidget_3);


        gridLayout_4->addWidget(horizontalGroupBox, 3, 0, 1, 1);

        gridGroupBox = new QGroupBox(centralWidget);
        gridGroupBox->setObjectName(QStringLiteral("gridGroupBox"));
        sizePolicy.setHeightForWidth(gridGroupBox->sizePolicy().hasHeightForWidth());
        gridGroupBox->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(gridGroupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        FileButton = new QPushButton(gridGroupBox);
        FileButton->setObjectName(QStringLiteral("FileButton"));

        gridLayout_2->addWidget(FileButton, 1, 0, 1, 1);

        loadButton = new QPushButton(gridGroupBox);
        loadButton->setObjectName(QStringLiteral("loadButton"));

        gridLayout_2->addWidget(loadButton, 1, 1, 1, 1);

        lineEdit = new QLineEdit(gridGroupBox);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        gridLayout_2->addWidget(lineEdit, 0, 0, 1, 2);


        gridLayout_4->addWidget(gridGroupBox, 0, 1, 1, 1);

        gridGroupBox1 = new QGroupBox(centralWidget);
        gridGroupBox1->setObjectName(QStringLiteral("gridGroupBox1"));
        gridLayout = new QGridLayout(gridGroupBox1);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        openGLWidget = new TDWidget(gridGroupBox1);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));

        gridLayout->addWidget(openGLWidget, 0, 1, 1, 1);


        gridLayout_4->addWidget(gridGroupBox1, 0, 0, 3, 1);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_5 = new QGridLayout(groupBox);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        textBrowser = new QTextBrowser(groupBox);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        gridLayout_5->addWidget(textBrowser, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupBox, 2, 1, 1, 1);

        gridLayout_4->setRowStretch(0, 1);
        gridLayout_4->setRowStretch(1, 3);
        gridLayout_4->setRowStretch(2, 1);
        gridLayout_4->setRowStretch(3, 2);
        gridLayout_4->setColumnStretch(0, 5);
        gridLayout_4->setColumnStretch(1, 2);
        ModelIntersectionClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ModelIntersectionClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        ModelIntersectionClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ModelIntersectionClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ModelIntersectionClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ModelIntersectionClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ModelIntersectionClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(actionFile);
        menu->addAction(closeAction);

        retranslateUi(ModelIntersectionClass);
        QObject::connect(closeAction, SIGNAL(triggered()), ModelIntersectionClass, SLOT(close()));

        QMetaObject::connectSlotsByName(ModelIntersectionClass);
    } // setupUi

    void retranslateUi(QMainWindow *ModelIntersectionClass)
    {
        ModelIntersectionClass->setWindowTitle(QApplication::translate("ModelIntersectionClass", "ModelIntersection", nullptr));
        actionFile->setText(QApplication::translate("ModelIntersectionClass", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
#ifndef QT_NO_SHORTCUT
        actionFile->setShortcut(QApplication::translate("ModelIntersectionClass", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        closeAction->setText(QApplication::translate("ModelIntersectionClass", "\345\205\263\351\227\255", nullptr));
        intersectButton->setText(QApplication::translate("ModelIntersectionClass", "\346\261\202\344\272\244", nullptr));
        pushButton_4->setText(QApplication::translate("ModelIntersectionClass", "\350\276\223\345\207\272", nullptr));
        gridGroupBox_2->setTitle(QApplication::translate("ModelIntersectionClass", "\351\200\211\346\213\251\346\250\241\345\236\213", nullptr));
        previewButton1->setText(QApplication::translate("ModelIntersectionClass", "\351\242\204\350\247\210", nullptr));
        deleteButton1->setText(QApplication::translate("ModelIntersectionClass", "\345\210\240\351\231\244", nullptr));
        label->setText(QApplication::translate("ModelIntersectionClass", "\346\250\241\345\236\2131", nullptr));
        label_2->setText(QApplication::translate("ModelIntersectionClass", "\346\250\241\345\236\2132", nullptr));
        previewButton2->setText(QApplication::translate("ModelIntersectionClass", "\351\242\204\350\247\210", nullptr));
        deleteButton2->setText(QApplication::translate("ModelIntersectionClass", "\345\210\240\351\231\244", nullptr));
        horizontalGroupBox->setTitle(QApplication::translate("ModelIntersectionClass", "\345\216\237\345\247\213\346\250\241\345\236\213", nullptr));
        gridGroupBox->setTitle(QApplication::translate("ModelIntersectionClass", "\350\275\275\345\205\245\346\250\241\345\236\213", nullptr));
        FileButton->setText(QApplication::translate("ModelIntersectionClass", "\351\200\211\346\213\251\346\250\241\345\236\213\346\226\207\344\273\266", nullptr));
        loadButton->setText(QApplication::translate("ModelIntersectionClass", "\350\275\275\345\205\245", nullptr));
        gridGroupBox1->setTitle(QApplication::translate("ModelIntersectionClass", "\345\267\245\344\275\234\345\214\272", nullptr));
        groupBox->setTitle(QApplication::translate("ModelIntersectionClass", "\344\277\241\346\201\257", nullptr));
        menu->setTitle(QApplication::translate("ModelIntersectionClass", "\350\217\234\345\215\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModelIntersectionClass: public Ui_ModelIntersectionClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELINTERSECTION_H
