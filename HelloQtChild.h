#pragma once

#include <QMainWindow>
#include "ui_HelloQtChild.h"

class HelloQtChild : public QWidget
{
  Q_OBJECT

public:
  HelloQtChild(QWidget *parent = nullptr);
  ~HelloQtChild();

  double getXCoordinate() const;
  double getYCoordinate() const;
  double getZCoordinate() const;
  AcDb3dPolyline* pPoly3d;
private slots:
  void addCoordinate();

private:
  Ui::HelloQtChildClass ui;
};

