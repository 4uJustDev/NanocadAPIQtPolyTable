#include "stdafx.h"
#include "HelloQtChild.h"
#include <dbobjptr.h>

HelloQtChild::HelloQtChild(QWidget *parent) : QWidget(parent)
{
  ui.setupUi(this);
  //m_addrres = NULL;
  AcGePoint3d pt1(0, 0, 0), pt2(10, 0, 0), pt3(10, 10, 0), pt4(10, 10, 10);
  m_points.append(pt1);
  m_points.append(pt2);
  m_points.append(pt3);
  m_points.append(pt4);

  m_addrres = Create3dPolyline(m_points);

  QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(addCoordinate()));
}

HelloQtChild::~HelloQtChild() {}

double HelloQtChild::getXCoordinate() const
{
    // Retrieve X coordinate from the QLineEdit or other widget
    return ui.lineEdit->text().toDouble();
}

double HelloQtChild::getYCoordinate() const
{
    // Retrieve Y coordinate from the QLineEdit or other widget
    return ui.lineEdit_2->text().toDouble();
}

double HelloQtChild::getZCoordinate() const
{
    // Retrieve Z coordinate from the QLineEdit or other widget
    return ui.lineEdit_3->text().toDouble();
}


AcDbObjectId HelloQtChild::Create3dPolyline(AcGePoint3dArray points)
{
    m_pPoly3d = new AcDb3dPolyline(AcDb::k3dSimplePoly, points);

    return HelloQtChild::PostToModelSpace(m_pPoly3d);
}

void deleteVertex()
{
    ads_name polyName;

    ads_point pt;

    if (acedEntSel(_T("\nSelect a 3d polyline: "), polyName, pt) != RTNORM)

        return;

    AcDbObjectId polyId;

    acdbGetObjectId(polyId, polyName);

    // get the polyline in an autoptr

    AcDbObjectPointer<AcDb3dPolyline> pPoly(polyId, AcDb::kForRead);

    Acad::ErrorStatus es = pPoly.openStatus();

    // if it didn't open

    if (es != Acad::eOk)
    {
        if (es == Acad::eNotThatKindOfClass)

            acutPrintf(_T("\nYou did not select a 3d polyline."));

        else

            acutPrintf(_T("\nError opening entity."));

        return;

    }
    // add each vertex to objectId array   

    AcDbObjectIdArray vertexArray;

    AcDbObjectIterator* pIter = pPoly->vertexIterator();

    for (pIter->start(); !pIter->done(); pIter->step())

        vertexArray.append(pIter->objectId());

    delete pIter;

    // get vertex to delete   

    ACHAR prompt[256];

    acutPrintf(prompt, _T("\nSelect vertex to delete (1-%d): "),

        vertexArray.length());

    int delVertex = -1;

    if (acedGetInt(prompt, &delVertex) != RTNORM)

        return;

    if (delVertex < 1 || delVertex > vertexArray.length())
    {
        acutPrintf(_T("\nInvalid vertex number."));

        return;
    }

    AcDbObjectPointer<AcDbObject> pObj(vertexArray[delVertex - 1],

        AcDb::kForWrite);

    if (pObj.openStatus() != Acad::eOk)
    {
        acutPrintf(_T("\nError opening vertex: %d"), delVertex);

        return;
    }

    pObj->erase();
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


void HelloQtChild::addCoordinate()
{
    double x = getXCoordinate();
    double y = getYCoordinate();
    double z = getZCoordinate();

    try
    {
        /*AcGePoint3d poi(x, y, z);
        m_points.append(poi);*/

        AcDb3dPolyline* pEnt;
        Acad::ErrorStatus es = acdbOpenObject(pEnt, m_addrres, AcDb::kForWrite, false);


        pEnt->appendVertex(new NcDb3dPolylineVertex(AcGePoint3d(x, y, z)));

        pEnt->close();
        //AcDbObjectId polyline3dId;
        //polyline3dId = HelloQtChild::Create3dPolyline(m_points);
        // Create a regular polygon (the center, the number


        ui.listWidget->addItem(QString("X: %1").arg(x));
        ui.listWidget->addItem(QString("Y: %1").arg(y));
        ui.listWidget->addItem(QString("Z: %1").arg(z));
        ui.listWidget->addItem(QString("___________________________________________"));
    }
    catch (const std::exception& e)
    {
        acutPrintf(_T("\nОшибка в главной функции."));
    }
}
