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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HelloQtChildClass
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton_2;
    QVBoxLayout *verticalLayout_3;

    void setupUi(QWidget *HelloQtChildClass)
    {
        if (HelloQtChildClass->objectName().isEmpty())
            HelloQtChildClass->setObjectName(QString::fromUtf8("HelloQtChildClass"));
        HelloQtChildClass->resize(659, 561);
        verticalLayout = new QVBoxLayout(HelloQtChildClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton = new QPushButton(HelloQtChildClass);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pushButton_2 = new QPushButton(HelloQtChildClass);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        verticalLayout_2->addWidget(pushButton_2);


        verticalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));

        verticalLayout->addLayout(verticalLayout_3);


        retranslateUi(HelloQtChildClass);

        QMetaObject::connectSlotsByName(HelloQtChildClass);
    } // setupUi

    void retranslateUi(QWidget *HelloQtChildClass)
    {
        HelloQtChildClass->setWindowTitle(QCoreApplication::translate("HelloQtChildClass", "HelloQtChild", nullptr));
        pushButton->setText(QCoreApplication::translate("HelloQtChildClass", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214", nullptr));
        pushButton_2->setText(QCoreApplication::translate("HelloQtChildClass", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\201\321\202\321\200\320\276\320\272\321\203", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HelloQtChildClass: public Ui_HelloQtChildClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELLOQTCHILD_H
