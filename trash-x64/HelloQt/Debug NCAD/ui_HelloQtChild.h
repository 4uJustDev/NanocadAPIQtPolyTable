/********************************************************************************
** Form generated from reading UI file 'HelloQtChild.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELLOQTCHILD_H
#define UI_HELLOQTCHILD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HelloQtChildClass
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QLabel *label;
    QListWidget *listWidget;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;

    void setupUi(QWidget *HelloQtChildClass)
    {
        if (HelloQtChildClass->objectName().isEmpty())
            HelloQtChildClass->setObjectName(QString::fromUtf8("HelloQtChildClass"));
        HelloQtChildClass->resize(659, 366);
        verticalLayout = new QVBoxLayout(HelloQtChildClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton = new QPushButton(HelloQtChildClass);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);

        label = new QLabel(HelloQtChildClass);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        listWidget = new QListWidget(HelloQtChildClass);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);

        lineEdit = new QLineEdit(HelloQtChildClass);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        lineEdit_2 = new QLineEdit(HelloQtChildClass);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        verticalLayout->addWidget(lineEdit_2);

        lineEdit_3 = new QLineEdit(HelloQtChildClass);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        verticalLayout->addWidget(lineEdit_3);


        retranslateUi(HelloQtChildClass);

        QMetaObject::connectSlotsByName(HelloQtChildClass);
    } // setupUi

    void retranslateUi(QWidget *HelloQtChildClass)
    {
        HelloQtChildClass->setWindowTitle(QCoreApplication::translate("HelloQtChildClass", "HelloQtChild", nullptr));
        pushButton->setText(QCoreApplication::translate("HelloQtChildClass", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214", nullptr));
        label->setText(QCoreApplication::translate("HelloQtChildClass", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HelloQtChildClass: public Ui_HelloQtChildClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELLOQTCHILD_H
