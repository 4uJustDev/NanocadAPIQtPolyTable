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

  CustomTableWidget* tableWidget;

  //Manipulations with Table
  AcGePoint3dArray getDataFromTable();	
  void HelloQtChild::updateDataInTable(AcDb3dPolyline* pEnt);

  //Manipulation wuth NcDbEntities
  AcDbObjectId Create3dPolyline(AcGePoint3dArray points);
  //static AcDbObjectId PostToModelSpace(AcDbEntity* pEnt);
  void PostToModelSpace(AcDbObjectId& objId, AcDbEntity* pEntity);
  AcDb3dPolyline* selectEntity(AcDbObjectId& eId, AcDb::OpenMode openMode);
  void refreshPolyline();
  void test();
  //Kostil
  AcDbObjectId globalId;

private slots:
  void addCoordinate();
  void addRow();
  void acceptChanges();


public:
  Ui::HelloQtChildClass ui;
};