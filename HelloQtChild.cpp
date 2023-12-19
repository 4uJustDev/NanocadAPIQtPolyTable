#include "stdafx.h"
#include "HelloQtChild.h"
#include <dbobjptr.h>
#include "CustomTableWidget.h"

HelloQtChild::HelloQtChild(QWidget *parent) : QWidget(parent)
{
  ui.setupUi(this);

  tableWidget = new CustomTableWidget(this);
  ui.verticalLayout_2->addWidget(tableWidget); 
  
  
  QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(addCoordinate()));
  QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(addRow()));
}

HelloQtChild::~HelloQtChild() {}

AcGePoint3dArray HelloQtChild::getDataCoordinates() const
{
    AcGePoint3dArray arrayPnt;
    QTableWidgetItem* a;

    for (int i = 0; i < tableWidget->rowCount(); i++) {
        a = tableWidget->item(i, 0);
        int x = a->text().toInt();
        a = tableWidget->item(i, 1);
        int y = a->text().toInt();
        a = tableWidget->item(i, 2);
        int z = a->text().toInt();
        AcGePoint3d pnt(x, y, z);
        arrayPnt.append(pnt);
    }
    return arrayPnt;
}


AcDbObjectId HelloQtChild::Create3dPolyline(AcGePoint3dArray points)
{
    m_pPoly3d = new AcDb3dPolyline(AcDb::k3dSimplePoly, points);

    return HelloQtChild::PostToModelSpace(m_pPoly3d);
}

AcDbObjectId HelloQtChild::PostToModelSpace(AcDbEntity* pEnt)
{

    // Get pointers to the block table
    AcDbBlockTable* pBlockTable;
    acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);

    // Get pointers that point to specific block table records (model space)
    AcDbBlockTableRecord* pBlockTableRecord;
    pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);

    // Add an object of the ACDBline class to the block table record
    AcDbObjectId entId;
    pBlockTableRecord->appendAcDbEntity(entId, pEnt);

    // Turn off the various objects of the graphical database
    pBlockTable->close();
    pBlockTableRecord->close();
    pEnt->close();
    return entId;
}

Acad::ErrorStatus HelloQtChild::AddVertexToPolyline(AcDbObjectId entId, AcGePoint3d pnt)
{

    if (entId)
    {
        AcDbEntity* pEnt;
        acdbOpenAcDbEntity(pEnt, entId, AcDb::kForWrite);

        AcDb3dPolyline* pPoly = AcDb3dPolyline::cast(pEnt);

        NcDb3dPolylineVertex* newVertex = new NcDb3dPolylineVertex(pnt);
        pPoly->appendVertex(newVertex);

        pEnt->close();
        pPoly->close();

        return Acad::eOk;
    }
    else
    {
        acutPrintf(_T("\nInvalid 3D Polyline pointer."));
        return Acad::eInvalidInput;
    }
}

void HelloQtChild::addRow() {
    tableWidget->insertRow(tableWidget->rowCount());
}

AcDb3dPolyline* selectEntity(AcDbObjectId& eId, AcDb::OpenMode openMode)
{
    ads_name en;
    ads_point pt;
    acedEntSel(L"\nSelect an entity: ", en, pt);
    // Exchange the ads_name for an object ID.
    //
    acdbGetObjectId(eId, en);
    AcDb3dPolyline* pEnt;
    acdbOpenObject(pEnt, eId, openMode);
    return pEnt;
}

void HelloQtChild::editPolyline(){

    AcDbObjectId polyId;

    AcDb3dPolyline* pEnt = selectEntity(polyId, AcDb::kForWrite);
    
    // add each vertex to objectId array   

    AcDbObjectIdArray vertexArray;

    AcDbObjectIterator* pIter = pEnt->vertexIterator();

    for (pIter->start(); !pIter->done(); pIter->step())

        vertexArray.append(pIter->objectId());

    delete pIter;

    int delVertex = -1;
    acutPrintf(_T("Select Vertex 1 - %d .\n"), vertexArray.length());

    ncedGetInt(L"\nWrite the vertex:", &delVertex);
    int x, y, z;
    ncedGetInt(L"\nWrite the x:", &x);
    ncedGetInt(L"\nWrite the y:", &y);
    ncedGetInt(L"\nWrite the z:", &z);
    AcGePoint3d pnt(x,y,z);

    NcDb3dPolylineVertex* newVertex = new NcDb3dPolylineVertex(pnt);
    pEnt->appendVertex(newVertex);

    pEnt->close();
}

void HelloQtChild::addCoordinate()
{
    try
    {
        AcGePoint3dArray some_points = getDataCoordinates();

        AcDbObjectId mId = Create3dPolyline(some_points);

        AcDb3dPolyline* pEnt;

        Acad::ErrorStatus es = acdbOpenObject(pEnt, mId, AcDb::kForWrite, false);

        pEnt->close();

    }
    catch (const std::exception& e)
    {
        acutPrintf(_T("\nОшибка в главной функции: %s"), e.what());
    }
}
