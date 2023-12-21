#include "stdafx.h"
#include "HelloQtChild.h"
#include <dbobjptr.h>
#include "CustomTableWidget.h"

#include "hostUI.h"
#include "hostQt.h"

HelloQtChild::HelloQtChild(QWidget *parent) : QWidget(parent)
{
  ui.setupUi(this);

  tableWidget = new CustomTableWidget(this);
  ui.verticalLayout_2->addWidget(tableWidget); 
  
  ui.pushButton_Update->setVisible(false);

  QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(addCoordinate()));
  QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(addRow()));
  QObject::connect(ui.pushButton_Update, SIGNAL(clicked()), this, SLOT(acceptChanges()));
}

HelloQtChild::~HelloQtChild() {}

AcGePoint3dArray HelloQtChild::getDataFromTable(){
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
        int x = pnt.x;
        int y = pnt.y;
        int z = pnt.z;
        AcGePoint3d prt(x, y, z);
        arrayPnt.append(prt);
    }
    
    delete pIter;
    int distance = vertexArray.length();

    //Обновляем таблицу
    tableWidget->setRowCount(0);
    
    for (int row = 0; row < distance; row++) {
    
        tableWidget->insertRow(tableWidget->rowCount());
        AcGePoint3d pnt =  arrayPnt.getAt(row); 
        int x = pnt.x;
        int y = pnt.y;
        int z = pnt.z;
    
        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(x)));
        tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(y)));
        tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(z)));
    
    }
}

AcDbObjectId HelloQtChild::Create3dPolyline(AcGePoint3dArray points)
{
    NcDb3dPolyline* m_pPoly3d = new AcDb3dPolyline(AcDb::k3dSimplePoly, points);

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

AcDb3dPolyline* HelloQtChild::selectEntity(AcDbObjectId& eId, AcDb::OpenMode openMode)
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


//void HelloQtChild::editPolyline(){
//
//    AcDbObjectId polyId;
//
//    AcDb3dPolyline* pEnt = selectEntity(polyId, AcDb::kForWrite);
//    
//    // add each vertex to objectId array   
//
//    AcDbObjectIdArray vertexArray;
//
//    AcDbObjectIterator* pIter = pEnt->vertexIterator();
//
//    AcGePoint3dArray arrayPnt;
//
//    //Заполняем точки из выбранной полилинии
//    for (pIter->start(); !pIter->done(); pIter->step()) {
//        
//        AcDbObjectId valIteration = pIter->objectId();
//        vertexArray.append(valIteration);
//
//        NcDb3dPolylineVertex* vertexix;
//        pEnt->openVertex(vertexix, valIteration, AcDb::kForWrite);
//
//        AcGePoint3d pnt = vertexix->position();
//        int x = pnt.x;
//        int y = pnt.y;
//        int z = pnt.z;
//        AcGePoint3d prt(x, y, z);
//        arrayPnt.append(prt);
//    }
//
//    delete pIter;
//    int distance = vertexArray.length();
//    //Создаем тоблицу из этих точек
//    CustomTableWidget* tableWidget1 = new CustomTableWidget();
//    tableWidget1->setRowCount(0);
//
//    for (int row = 0; row < distance; row++) {
//
//        tableWidget1->insertRow(tableWidget1->rowCount());
//        AcGePoint3d pnt =  arrayPnt.getAt(row);
//        int x = pnt.x;
//        int y = pnt.y;
//        int z = pnt.z;
//
//        tableWidget1->setItem(row, 0, new QTableWidgetItem(QString::number(x)));
//        tableWidget1->setItem(row, 1, new QTableWidgetItem(QString::number(y)));
//        tableWidget1->setItem(row, 2, new QTableWidgetItem(QString::number(z)));
//
//    }
//
//    acutPrintf(L"\nВы начали изменять выбранный AcDb3dPolyline!\n");
//
//    QPushButton* cancelButton = new QPushButton("Cancel");
//
//    QObject::connect(cancelButton, &QPushButton::clicked, [&, tableWidget1, vertexArray, pEnt]() {
//        acutPrintf(L"\nCancel!\n");
//        QTableWidgetItem* a;
//        AcGePoint3dArray arrayPnts;
//
//        for (int row = 0; row < vertexArray.length(); row++) {
//            AcGePoint3d pnt;
//            for (int column = 0; column < 3; column++) {
//                a = tableWidget1->item(row, column);
//                int val = a->text().toInt();
//                switch (column) {
//                case 0:
//                    pnt[X] = val;
//                    break;
//                case 1:
//                    pnt[Y] = val;
//                    break;
//                case 2:
//                    pnt[Z] = val;
//                    break;
//                }
//            }
//            arrayPnts.append(pnt);
//        }
//        
//        AcDbObjectIterator* pIter = pEnt->vertexIterator();
//        int row = 0;
//
//        for (pIter->start(); !pIter->done(); pIter->step()) {
//            AcDbObjectId valIteration = pIter->objectId();
//            NcDb3dPolylineVertex* vertex;
//            if (pEnt->openVertex(vertex, valIteration, AcDb::kForWrite) == Acad::eOk) {
//                vertex->setPosition(arrayPnts.at(row));
//                vertex->close();
//            }
//            row++;
//        }
//
//        delete pIter;
//        tableWidget1->hide();
//        });
//
//    
//
//    // Add widgets to the layout
//    verticalValue->addWidget(tableWidget1);
//    verticalValue->addWidget(cancelButton);
//    pEnt->close();
//}

void HelloQtChild::addCoordinate()
{
    try
    {
        AcGePoint3dArray some_points = getDataFromTable();

        AcDbObjectId mId = Create3dPolyline(some_points);

        //AcDb3dPolyline* pEnt;

        //Acad::ErrorStatus es = acdbOpenObject(pEnt, mId, AcDb::kForWrite, false);

        //pEnt->close();

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
    ui.pushButton_Update->setVisible(false);
    ui.pushButton->setVisible(true);
}

