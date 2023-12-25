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
  this->ui.pushButton_Update->setVisible(false);

  label = new QLabel(this);
  ui.verticalLayout_2->addWidget(label);
  label->setVisible(false);
  label->setAlignment(Qt::AlignCenter);

  ui.lineEdit->setToolTip(QString::fromUtf16(u"Введите точность или будет использовано значение по умолчанию: 2"));

  QColor col1 = QColor::fromRgb(144, 238, 144);
  QColor col2 = QColor::fromRgb(163, 172, 238);
  QColor col3 = QColor::fromRgb(238, 236, 167);
  QColor col4 = QColor::fromRgb(238, 133, 126);

  if (col1.isValid() && col2.isValid() && col3.isValid()) {
      QString qss1 = QString("background-color: %1").arg(col1.name());
      QString qss2 = QString("background-color: %1").arg(col2.name());
      QString qss3 = QString("background-color: %1").arg(col3.name());
      QString qss4 = QString("background-color: %1").arg(col4.name());

      ui.pushButton->setStyleSheet(qss1);
      ui.pushButton_2->setStyleSheet(qss2);
      ui.pushButton_Update->setStyleSheet(qss3);
      ui.pushButton_delete->setStyleSheet(qss4);
  }


  QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(createPoly()));
  QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(addRow()));
  QObject::connect(ui.pushButton_Update, SIGNAL(clicked()), this, SLOT(acceptChanges()));
  QObject::connect(ui.pushButton_delete, SIGNAL(clicked()), this, SLOT(deleteRow()));
}

HelloQtChild::~HelloQtChild() {}

AcGePoint3dArray HelloQtChild::getDataFromTable(){
    AcGePoint3dArray arrayPnt;
    QTableWidgetItem* item1, * item2, * item3;

    for (int i = 0; i < tableWidget->rowCount(); i++) {
        item1 = tableWidget->item(i, 0);
        item2 = tableWidget->item(i, 1);
        item3 = tableWidget->item(i, 2);
        if (item1 == NULL || item2 == NULL || item3 == NULL) {
            continue;
        }
        double x = item1->text().toDouble();
        double y = item2->text().toDouble();
        double z = item3->text().toDouble();
        AcGePoint3d pnt(x, y, z);
        arrayPnt.append(pnt);
    }
    return arrayPnt;
}

void HelloQtChild::updateDataInTable(AcDb3dPolyline* pEnt)
{   
    AcDbObjectIterator* pIter = pEnt->vertexIterator();
    
    AcGePoint3dArray arrayPnt;
    
    //Заполняем точки из выбранной полилинии
    for (pIter->start(); !pIter->done(); pIter->step()) {
        
        AcDbObjectId valIteration = pIter->objectId();
    
        NcDb3dPolylineVertex* vertexix;
        pEnt->openVertex(vertexix, valIteration, AcDb::kForRead);
    
        AcGePoint3d pnt = vertexix->position();
        double x = pnt.x;
        double y = pnt.y;
        double z = pnt.z;
        AcGePoint3d prt(x, y, z);
        arrayPnt.append(prt);

        vertexix->close();
    }
    delete pIter;
    

    //Обновляем таблицу
    tableWidget->setRowCount(0);
    int distance = arrayPnt.length();

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

void HelloQtChild::insertStub(long count) {
    unshowing();
    label->setText(QString::fromUtf16(u"<b>Выбрано %1 полилинии").arg(count));
}

void HelloQtChild::showing() {
    this->ui.pushButton_2->setVisible(true);
    this->ui.pushButton->setVisible(true);
    this->ui.pushButton_delete->setVisible(true);
    this->ui.comboBox->setVisible(true);
    this->ui.lineEdit->setVisible(true);
    this->tableWidget->setVisible(true);
    this->label->setVisible(false);
}

void HelloQtChild::unshowing() {
    this->ui.pushButton_2->setVisible(false);
    this->ui.pushButton->setVisible(false);
    this->ui.pushButton_delete->setVisible(false);
    this->ui.comboBox->setVisible(false);
    this->ui.lineEdit->setVisible(false);   
    this->tableWidget->setVisible(false);
    this->label->setVisible(true);
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

    // Create the AsdkObjectToNotify for polyline

    ObjectToNotify* pObj = new ObjectToNotify(this);
    pObj->eLinkage(polyID);

    AcDbObjectId objId;
    if ((pNameList->getAt(_T("object_to_notify"), objId))
        == Acad::eKeyNotFound)
    {
        pNameList->setAt(_T("object_to_notify"), pObj, objId);
        pObj->close();
    }
    else {
        delete pObj;
    }

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

void HelloQtChild::refreshPolyline() {

    createPoly();

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

void HelloQtChild::createPoly()
{
    try
    {
        AcGePoint3dArray points = getDataFromTable();

        Create3dPolyline(points);

    }
    catch (const std::exception& e)
    {
        acutPrintf(_T("\nОшибка в главной функции: %s"), e.what());
    }
}

void HelloQtChild::addRow() {
    tableWidget->insertRow(tableWidget->rowCount());
}

void HelloQtChild::deleteRow() {
    int row = this->tableWidget->currentRow();
    this->tableWidget->removeRow(row);
}

void HelloQtChild::acceptChanges() {
   
    refreshPolyline();
    this->ui.pushButton_Update->setVisible(false);
}

