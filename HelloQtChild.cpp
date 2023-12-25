#include "stdafx.h"
#include "HelloQtChild.h"
#include <dbobjptr.h>
#include "CustomTableWidget.h"
#include "ObjectToNotify.h"

#include "hostUI.h"
#include "hostQt.h"

HelloQtChild::HelloQtChild(QWidget *parent) : QWidget(parent)
{
  ui.setupUi(this);

  tableWidget = new CustomTableWidget(this);
  ui.verticalLayout_2->addWidget(tableWidget); 
  
  ui.lineEdit->setToolTip("Введите точность или будет использовано значение по умолчанию (2)");

  QColor col1 = QColor::fromRgb(144, 238, 144);
  QColor col2 = QColor::fromRgb(163, 172, 238);
  QColor col3 = QColor::fromRgb(238, 236, 167);

  if (col1.isValid() && col2.isValid() && col3.isValid()) {
      QString qss1 = QString("background-color: %1").arg(col1.name());
      QString qss2 = QString("background-color: %1").arg(col2.name());
      QString qss3 = QString("background-color: %1").arg(col3.name());

      ui.pushButton->setStyleSheet(qss1);
      ui.pushButton_2->setStyleSheet(qss2);
      ui.pushButton_Update->setStyleSheet(qss3);
  }


  QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(addCoordinate()));
  QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(addRow()));
  QObject::connect(ui.pushButton_Update, SIGNAL(clicked()), this, SLOT(acceptChanges()));
}

HelloQtChild::~HelloQtChild() {}

AcGePoint3dArray HelloQtChild::getDataFromTable(){
    AcGePoint3dArray arrayPnt;
    QTableWidgetItem* a1, *a2, *a3;

    for (int i = 0; i < tableWidget->rowCount(); i++) {
        a1 = tableWidget->item(i, 0);
        a2 = tableWidget->item(i, 1);
        a3 = tableWidget->item(i, 2);
        if (a1 == NULL || a2 == NULL || a3 == NULL || a1->text() == "d" || a2->text() == "d" || a3->text() == "d") {
            continue;
        }
        double x = a1->text().toDouble();
        double y = a2->text().toDouble();
        double z = a3->text().toDouble();
        AcGePoint3d pnt(x, y, z);
        arrayPnt.append(pnt);
    }
    return arrayPnt;
}

void HelloQtChild::updateDataInTable(AcDb3dPolyline* pEnt)
{
    AcDbObjectIdArray vertexArray;
    
    AcDbObjectIterator* pIter = pEnt->vertexIterator();
    
    AcGePoint3dArray arrayPnt;
    
    //Заполняем точки из выбранной полилинии
    for (pIter->start(); !pIter->done(); pIter->step()) {
        
        AcDbObjectId valIteration = pIter->objectId();
        vertexArray.append(valIteration);
    
        NcDb3dPolylineVertex* vertexix;
        pEnt->openVertex(vertexix, valIteration, AcDb::kForWrite);
    
        AcGePoint3d pnt = vertexix->position();
        double x = pnt.x;
        double y = pnt.y;
        double z = pnt.z;
        AcGePoint3d prt(x, y, z);
        arrayPnt.append(prt);

        vertexix->close();
    }
    
    delete pIter;
    int distance = vertexArray.length();

    //Обновляем таблицу
    tableWidget->setRowCount(0);
    
    for (int row = 0; row < distance; row++) {
    
        tableWidget->insertRow(tableWidget->rowCount());
        AcGePoint3d pnt =  arrayPnt.getAt(row); 
        double x = pnt.x;
        double y = pnt.y;
        double z = pnt.z;

        int param = ui.comboBox->currentIndex();
        int accuracy = ui.lineEdit->text().toInt();

        if (accuracy<=0) {
            accuracy = 2;
        }
        NCHAR fmtval[12];

        ncdbRToS(x, param, accuracy, fmtval);
        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdWString(fmtval)));
        ncdbRToS(y, param, accuracy, fmtval);
        tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdWString(fmtval)));
        ncdbRToS(z, param, accuracy, fmtval);
        tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdWString(fmtval)));
    
    }
}

void HelloQtChild::insertStub() {
    this->tableWidget->setRowCount(0);
}

AcDbObjectId HelloQtChild::Create3dPolyline(AcGePoint3dArray points)
{
    AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();

    AcDbObjectId polyID;

    NcDb3dPolyline* m_pPoly3d = new AcDb3dPolyline(AcDb::k3dSimplePoly, points);

    acdbOpenObject(m_pPoly3d, polyID, AcDb::kForWrite);

    m_pPoly3d->setDatabaseDefaults(pDb);

    HelloQtChild::PostToModelSpace(polyID, m_pPoly3d);

    // Open the named object dictionary, check if there is
    // an entry with the key "ASDK_DICT".  If not, create a
    // dictionary and add it.
    // 
    AcDbDictionary* pNamedObj;
    AcDbDictionary* pNameList;
    pDb->getNamedObjectsDictionary(pNamedObj,
        AcDb::kForWrite);
    if (pNamedObj->getAt(_T("ASDK_DICT"),
        (AcDbObject*&)pNameList, AcDb::kForWrite)
        == Acad::eKeyNotFound)
    {
        pNameList = new AcDbDictionary;
        AcDbObjectId DictId;
        pNamedObj->setAt(_T("ASDK_DICT"), pNameList, DictId);
    }
    pNamedObj->close();
    AcDbLine;
    // Create the AsdkObjectToNotify for lineA
    //
    ObjectToNotify* pObj = new ObjectToNotify(this);
    pObj->eLinkage(polyID);

    AcDbObjectId objId;
    if ((pNameList->getAt(_T("object_to_notify_A"), objId))
        == Acad::eKeyNotFound)
    {
        pNameList->setAt(_T("object_to_notify_A"), pObj, objId);
        pObj->close();
    }
    else {
        delete pObj;
        ads_printf(_T("object_to_notify_A already exists\n"));
    }

    // Set up persistent reactor link between polyline and AsdkObjectToNotify
    m_pPoly3d->addPersistentReactor(objId);
    pNameList->close();
    m_pPoly3d->close();

    return polyID;
}

void HelloQtChild::PostToModelSpace(AcDbObjectId& objId, AcDbEntity* pEntity)
{
    AcDbBlockTable* pBlockTable;
    AcDbBlockTableRecord* pSpaceRecord;

    acdbHostApplicationServices()->workingDatabase()
        ->getSymbolTable(pBlockTable, AcDb::kForRead);

    pBlockTable->getAt(ACDB_MODEL_SPACE, pSpaceRecord,
        AcDb::kForWrite);
    pBlockTable->close();

    pSpaceRecord->appendAcDbEntity(objId, pEntity);
    pSpaceRecord->close();

    return;
}
    
AcDb3dPolyline* HelloQtChild::selectEntity(AcDbObjectId& eId, AcDb::OpenMode openMode)
{
    AcDb3dPolyline* pEnt;
    acdbOpenObject(pEnt, eId, openMode);
    return pEnt;
}

void HelloQtChild::refreshPolyline() {

    addCoordinate();

    struct resbuf* prbGrip = NULL;

    struct resbuf* prbPick = NULL;

    // Get the selection sets

    ncedSSGetFirst(&prbGrip, &prbPick);

    long gripLen, pickLen;
    if (prbPick->restype != RTPICKS)
        return;

    acedSSLength(prbPick->resval.rlname, &pickLen);

    ads_name entres;
    AcDbObjectId objId;
    acedSSName(prbPick->resval.rlname, 0, entres);
    acdbGetObjectId(objId, entres);

    AcDbEntity* pEnt;
    acdbOpenAcDbEntity(pEnt, objId, kForWrite);

    pEnt->erase();
    pEnt->close();

    acedSSFree(prbPick->resval.rlname);

    acutRelRb(prbPick);
};

void HelloQtChild::addCoordinate()
{
    try
    {
        AcGePoint3dArray some_points = getDataFromTable();

        AcDbObjectId mId = Create3dPolyline(some_points);

    }
    catch (const std::exception& e)
    {
        acutPrintf(_T("\nОшибка в главной функции: %s"), e.what());
    }
}

void HelloQtChild::addRow() {
    tableWidget->insertRow(tableWidget->rowCount());
}
void HelloQtChild::acceptChanges() {
   
    refreshPolyline();
}

