#pragma once
#include <QDialog>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QDebug>

class MyDialog : public QDialog {

public:
    MyDialog();

private slots:
    void onButtonClick();

private:
    QTableWidget* tableWidget;
};



