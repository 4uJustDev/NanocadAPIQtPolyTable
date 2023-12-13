#include "stdafx.h"
#include "HelloQtChild.h"
#include <dbobjptr.h>

Acad::ErrorStatus addToModelSpace(AcDbObjectId& objId, AcDbEntity* pEntity);

HelloQtChild::HelloQtChild(QWidget *parent) : QWidget(parent)
{
  ui.setupUi(this);
  //pPoly3d = new AcDb3dPolyline();
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

void deleteVertex()
{
    ads_name polyName;
    ads_point pt;
    if (acedEntSel(_T("\nВыберите a 3D-полилинию: "), polyName, pt) != RTNORM)
        return;
    AcDbObjectId polyId;
    acdbGetObjectId(polyId, polyName);
    // получим полилинию как интеллектуальный указатель
    AcDbObjectPointer<AcDb3dPolyline> pPoly(polyId, AcDb::kForRead);
    Acad::ErrorStatus es = pPoly.openStatus();
    // если не открылась
    if (es != Acad::eOk)
    {
        if (es == Acad::eNotThatKindOfClass)
            acutPrintf(_T("\nВы выбрали не 3D-полилинию."));
        else
            acutPrintf(_T("\nОшибка открытия примитива."));
        return;

    }
    // Добавим все вершины к массиву AcDbObjectId  
    AcDbObjectIdArray vertexArray;
    AcDbObjectIterator* pIter = pPoly->vertexIterator();
    for (pIter->start(); !pIter->done(); pIter->step())
        vertexArray.append(pIter->objectId());
    delete pIter;
    // Указываем и находим вершину для удаления
    ACHAR prompt[256];
    acutPrintf(prompt, _T("\nУкажите номер удаляемой вершины (1-%d): "), vertexArray.length());
    int delVertex = -1;
    if (acedGetInt(prompt, &delVertex) != RTNORM)
        return;
    if (delVertex < 1 || delVertex > vertexArray.length())
    {
        acutPrintf(_T("\nНедопустимый номер вершины."));
        return;
    }
    // Открываем вершину и удаляем её
    AcDbObjectPointer<AcDbObject> pObj(vertexArray[delVertex - 1], AcDb::kForWrite);
    if (pObj.openStatus() != Acad::eOk)
    {
        acutPrintf(_T("\nОшибка открытия вершины: %d"), delVertex);
        return;
    }
    pObj->erase();
}

Acad::ErrorStatus addToModelSpace(AcDbObjectId& objId, AcDbEntity* pEntity)
{
    AcDbBlockTable* pBlockTable;
    AcDbBlockTableRecord* pSpaceRecord;

    acdbHostApplicationServices()->workingDatabase()
        ->getSymbolTable(pBlockTable, AcDb::kForRead);

    pBlockTable->getAt(ACDB_MODEL_SPACE, pSpaceRecord,
        AcDb::kForWrite);

    pBlockTable->close();

    pSpaceRecord->appendAcDbEntity(objId, pEntity);

    pEntity->close();
    pSpaceRecord->close();

    return Acad::eOk;
}

Acad::ErrorStatus AddVertexToPolyline(AcDb3dPolyline* pPolyline, const AcGePoint3d& pnt)
{
    if (pPolyline)
    {
        NcDb3dPolylineVertex* newVertex = new NcDb3dPolylineVertex(pnt);

        pPolyline->appendVertex(newVertex);
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
        /*AcDbObjectId polyId;

        if (pPoly3d == nullptr)
        {
            pPoly3d = new AcDb3dPolyline();
        }*/
        AcDbObjectId polyId;
        pPoly3d = new AcDb3dPolyline();
        AddVertexToPolyline(pPoly3d, AcGePoint3d(x, y, z));
        addToModelSpace(polyId, pPoly3d);

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
