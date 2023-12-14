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

  AcDbObjectId* m_addrres;
  AcGePoint3dArray m_points;
  
  // add a function to create a 3D polyline
  static AcDbObjectId Create3dPolyline(AcGePoint3dArray points);
  static AcDbObjectId PostToModelSpace(AcDbEntity* pEnt);
  Acad::ErrorStatus HelloQtChild::AddVertexToPolyline(AcDbObjectId entId, AcGePoint3d ptBase);

private slots:
  void addCoordinate();

private:
  Ui::HelloQtChildClass ui;
};

