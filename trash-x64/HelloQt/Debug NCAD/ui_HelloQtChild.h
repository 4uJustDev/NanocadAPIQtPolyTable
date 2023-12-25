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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HelloQtChildClass
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_Update;
    QPushButton *pushButton;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_delete;
    QPushButton *pushButton_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QComboBox *comboBox;

    void setupUi(QWidget *HelloQtChildClass)
    {
        if (HelloQtChildClass->objectName().isEmpty())
            HelloQtChildClass->setObjectName(QString::fromUtf8("HelloQtChildClass"));
        HelloQtChildClass->resize(359, 561);
        verticalLayout = new QVBoxLayout(HelloQtChildClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton_Update = new QPushButton(HelloQtChildClass);
        pushButton_Update->setObjectName(QString::fromUtf8("pushButton_Update"));

        verticalLayout->addWidget(pushButton_Update);

        pushButton = new QPushButton(HelloQtChildClass);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButton_delete = new QPushButton(HelloQtChildClass);
        pushButton_delete->setObjectName(QString::fromUtf8("pushButton_delete"));

        horizontalLayout_2->addWidget(pushButton_delete);

        pushButton_2 = new QPushButton(HelloQtChildClass);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout_2->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));

        verticalLayout->addLayout(verticalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEdit = new QLineEdit(HelloQtChildClass);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        comboBox = new QComboBox(HelloQtChildClass);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout->addWidget(comboBox);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(HelloQtChildClass);

        QMetaObject::connectSlotsByName(HelloQtChildClass);
    } // setupUi

    void retranslateUi(QWidget *HelloQtChildClass)
    {
        HelloQtChildClass->setWindowTitle(QCoreApplication::translate("HelloQtChildClass", "HelloQtChild", nullptr));
        pushButton_Update->setText(QCoreApplication::translate("HelloQtChildClass", "\320\230\320\267\320\274\320\265\320\275\320\270\321\202\321\214", nullptr));
        pushButton->setText(QCoreApplication::translate("HelloQtChildClass", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214", nullptr));
        pushButton_delete->setText(QCoreApplication::translate("HelloQtChildClass", "Delete row", nullptr));
        pushButton_2->setText(QCoreApplication::translate("HelloQtChildClass", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\201\321\202\321\200\320\276\320\272\321\203", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("HelloQtChildClass", "Scientific", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("HelloQtChildClass", "Decimal", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("HelloQtChildClass", "Engineering ", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("HelloQtChildClass", "Architectural", nullptr));
        comboBox->setItemText(4, QCoreApplication::translate("HelloQtChildClass", "Fractional", nullptr));

    } // retranslateUi

};

namespace Ui {
    class HelloQtChildClass: public Ui_HelloQtChildClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELLOQTCHILD_H
