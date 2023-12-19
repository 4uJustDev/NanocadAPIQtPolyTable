#pragma once

#include <QMainWindow>
#include "ui_HelloQtChild.h"
#include "CustomTableWidget.h"

class HelloQtChild : public QWidget
{
  Q_OBJECT

public:
  HelloQtChild(QWidget *parent = nullptr);
  ~HelloQtChild();

  AcGePoint3dArray getDataCoordinates() const;

  AcDbObjectId m_addrres;
  AcGePoint3dArray m_points;
  NcDb3dPolyline* m_pPoly3d;

  CustomTableWidget* tableWidget;
  // add a function to create a 3D polyline
  AcDbObjectId Create3dPolyline(AcGePoint3dArray points);
  static AcDbObjectId PostToModelSpace(AcDbEntity* pEnt);
  Acad::ErrorStatus HelloQtChild::AddVertexToPolyline(AcDbObjectId entId, AcGePoint3d ptBase);

private slots:
  void addCoordinate();
  void addRow();

private:
  Ui::HelloQtChildClass ui;
};

