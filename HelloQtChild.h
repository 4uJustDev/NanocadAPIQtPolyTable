#pragma once

#include <QMainWindow>
#include <QLabel>
#include "ui_HelloQtChild.h"
#include "CustomTableWidget.h"

class HelloQtChild : public QWidget
{
  Q_OBJECT

public:
  HelloQtChild(QWidget *parent = nullptr);
  ~HelloQtChild();

  CustomTableWidget* tableWidget;
  QLabel* label;

  //Manipulations with Table
  AcGePoint3dArray getDataFromTable();	
  void updateDataInTable(AcDb3dPolyline* pEnt);
  void insertStub(long count);

  void showing();
  void unshowing();

  //Manipulation wuth NcDbEntities
  AcDbObjectId Create3dPolyline(AcGePoint3dArray points);
  //static AcDbObjectId PostToModelSpace(AcDbEntity* pEnt);
  void PostToModelSpace(AcDbObjectId& objId, AcDbEntity* pEntity);
  void refreshPolyline();

private slots:
  void createPoly();
  void addRow();
  void deleteRow();
  void acceptChanges();


public:
  Ui::HelloQtChildClass ui;
};