#include "stdafx.h"

#include "hostUI.h"
#include "hostQt.h"
#include "HelloQtChild.h"

#include <string.h>
#include <stdlib.h>
#include <aced.h>
#include <dbents.h>
#include <dbsymtb.h>
#include <dbapserv.h>
#include <adslib.h>
#include "tchar.h"

#include "acarray.h"
#include "acdocman.h"
#include "aced.h"
#include "dbents.h"
#include "dbapserv.h"
#include "dbapserv.h"
#include "dbapserv.h"


#include <string.h>
#include <stdlib.h>
#include <aced.h>
#include <dbents.h>
#include <dbsymtb.h>
#include <dbapserv.h>
#include <adslib.h>
#include "tchar.h"



void assocLines();
void addToModelSpace(AcDbObjectId&, AcDbEntity*);
void initApp(void);
void unloadApp(void);
class AsdkObjectToNotify : public AcDbObject
    //
    // AsdkObjectToNotify - customized AcDbObject for persistent
    // reactor to notify:
    //
{
public:
    ACRX_DECLARE_MEMBERS(AsdkObjectToNotify);
    AsdkObjectToNotify() {};
    void eLinkage(AcDbObjectId i, double f = 1.0)
    {
        mId = i; mFactor = f;
    };
    void              modified(const AcDbObject*);
    Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
    Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
    Acad::ErrorStatus dxfInFields(AcDbDxfFiler*);
    Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*) const;

private:
    AcDbObjectId mId;
    double mFactor;
};

ACRX_DXF_DEFINE_MEMBERS(AsdkObjectToNotify, AcDbObject,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    0, \
    ASDKOBJECTTONOTIFY, persreac);

void AsdkObjectToNotify::modified(const AcDbObject* pObj)
{
    AcDbLine* pLine = AcDbLine::cast(pObj);
    if (!pLine) {
        const TCHAR* cstr = pObj->isA()->name();
        acutPrintf(_T("This is a %s.\n"), cstr);
        acutPrintf(_T("I only work with lines. Sorry.\n"));
        return;
    }
    acutPrintf(_T("\nReactor attached to %lx calling %lx.\n"),
        pLine->objectId().asOldId(), mId.asOldId());

    //// This open will fail during notification caused by
    //// reactor being added to the entity or when this
    //// notification is in reaction to a change due to the
    //// other line's reactor changing this line.  This will
    //// properly prevent an infinite recursive cross loop
    //// between the two lines and their reactors.
    //// 
    //AcDbLine* pLine2;
    //if (acdbOpenObject(pLine2, mId,
    //    AcDb::kForWrite) == Acad::eOk)
    //{
    //    // get length of line entity we're being notified
    //    // has just been modified
    //    //
    //    AcGePoint3d p = pLine->startPoint();
    //    AcGePoint3d q = pLine->endPoint();
    //    AcGeVector3d v = q - p;
    //    double len = v.length();
    //    //
    //    // update other entity to match:
    //    //
    //    p = pLine2->startPoint();
    //    q = pLine2->endPoint();
    //    v = q - p;
    //    v = len * mFactor * v.normal();
    //    pLine2->setEndPoint(p + v);
    //    pLine2->close();
    //}
}

Acad::ErrorStatus AsdkObjectToNotify::dwgInFields(AcDbDwgFiler* filer)
{
    assertWriteEnabled();
    AcDbObject::dwgInFields(filer);
    filer->readItem(&mFactor);
    filer->readItem((AcDbSoftPointerId*)&mId);
    return filer->filerStatus();
}

Acad::ErrorStatus AsdkObjectToNotify::dwgOutFields(AcDbDwgFiler* filer) const
{
    assertReadEnabled();
    AcDbObject::dwgOutFields(filer);
    filer->writeItem(mFactor);
    filer->writeItem((AcDbSoftPointerId&)mId);
    return filer->filerStatus();
}

Acad::ErrorStatus AsdkObjectToNotify::dxfInFields(AcDbDxfFiler* filer)
{
    assertWriteEnabled();

    Acad::ErrorStatus es;
    if ((es = AcDbObject::dxfInFields(filer))
        != Acad::eOk)
    {
        return es;
    }

    // Check if we're at the right subclass data marker
    //
    if (!filer->atSubclassData(_T("AsdkObjectToNotify"))) {
        return Acad::eBadDxfSequence;
    }

    struct resbuf rbIn;

    while (es == Acad::eOk) {
        if ((es = filer->readItem(&rbIn)) == Acad::eOk) {
            if (rbIn.restype == AcDb::kDxfReal) {
                mFactor = rbIn.resval.rreal;
            }
            else if (rbIn.restype
                == AcDb::kDxfSoftPointerId)
            {
                // ObjectIds are filed in as ads_names
                // 
                acdbGetObjectId(mId, rbIn.resval.rlname);
            }
            else {   // invalid group
                return(filer->pushBackItem());
            }
        }
    }
    return filer->filerStatus();
}

Acad::ErrorStatus AsdkObjectToNotify::dxfOutFields(AcDbDxfFiler* filer) const
{
    assertReadEnabled();
    AcDbObject::dxfOutFields(filer);
    filer->writeItem(AcDb::kDxfSubclass,
        _T("AsdkObjectToNotify"));
    filer->writeItem(AcDb::kDxfReal, mFactor);
    filer->writeItem(AcDb::kDxfSoftPointerId, mId);
    return filer->filerStatus();
}



void assocLines()
{
    AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
    AcDbObjectId aId, bId;

    AcDbLine* pLineA = new AcDbLine;
    pLineA->setDatabaseDefaults(pDb);
    pLineA->setStartPoint(AcGePoint3d(1, 1, 0));
    pLineA->setEndPoint(AcGePoint3d(2, 1, 0));
    addToModelSpace(aId, pLineA);

    ads_printf(_T("Line A is %lx from 1,1 to 2,1.\n"),
        pLineA->objectId().asOldId());


    AcDbLine* pLineB = new AcDbLine;
    pLineB->setDatabaseDefaults(pDb);
    pLineB->setStartPoint(AcGePoint3d(1, 2, 0));
    pLineB->setEndPoint(AcGePoint3d(2, 2, 0));
    addToModelSpace(bId, pLineB);

    ads_printf(_T("Line B is %lx from 1,2 to 2,2.\n"),
        pLineB->objectId().asOldId());

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

    // Create the AsdkObjectToNotify for lineA
    //
    AsdkObjectToNotify* pObj = new AsdkObjectToNotify();
    pObj->eLinkage(bId);

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

    // Set up persistent reactor link between line a
    // and AsdkObjectToNotify
    //
    pLineA->addPersistentReactor(objId);
    pLineA->close();

    // Create the AsdkObjectToNotify for LineB
    //
    pObj = new AsdkObjectToNotify();
    pObj->eLinkage(aId);

    if ((pNameList->getAt(_T("object_to_notify_B"), objId))
        == Acad::eKeyNotFound)
    {
        pNameList->setAt(_T("object_to_notify_B"), pObj, objId);
        pObj->close();
    }
    else {
        delete pObj;
        ads_printf(_T("object_to_notify_B already exists\n"));
    }
    pNameList->close();

    // Set up persistent reactor link between line b
    // and AsdkObjectToNotify
    //
    pLineB->addPersistentReactor(objId);
    pLineB->close();
}

void addToModelSpace(AcDbObjectId& objId, AcDbEntity* pEntity)
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


HelloQtChild* pWidgetChild;

class AcEdReactor : public NcEditorReactor
{


    void virtual pickfirstModified() override {
        // Get the selection set
        ads_name sset;
        if (acedSSGet(nullptr, nullptr, nullptr, nullptr, sset) != RTNORM) {
            // Failed to get the selection set
            ncutPrintf(L"\nFailed to get the selection set");
            return;
        }

        // Process each selected object in the set
        long length = 0;
 
        ads_name ent;
        if (acedSSName(sset, 0, ent) == RTNORM) {

            AcDbObjectId eId;
            acdbGetObjectId(eId, ent);
            AcDb3dPolyline* pEnt;
            acdbOpenObject(pEnt, eId, AcDb::kForWrite);
            
            pWidgetChild->updateDataInTable(pEnt);

            ads_printf(L"\nSelected Entity Handle: %x", ent[0]);
        }

        // Release the selection set
        acedSSFree(sset);

        // Continue with your other code
        pWidgetChild->ui.pushButton->setVisible(false);
        pWidgetChild->ui.pushButton_Update->setVisible(true);        

        ncutPrintf(L"\nWorking Reactor");
    }
};

hostUiPaletteSet* m_pPalSet = NULL;

HINSTANCE _hdllInstance =NULL ;
AC_IMPLEMENT_EXTENSION_MODULE(theArxDLL);

class helloQtPalette : public hostQtPalette
{
  DECLARE_DYNAMIC(helloQtPalette)

public:
  helloQtPalette() {};

  afx_msg void    OnSize(UINT nType, int cx, int cy)
  {
    if (m_pWinWidget)
    {
      HWND wnd = (HWND)m_pWinWidget->windowHandle()->winId();
      ::SetWindowPos(wnd, nullptr, 0, 0, cx, cy, SWP_NOZORDER);
    }
  }

  DECLARE_MESSAGE_MAP();
};

BEGIN_MESSAGE_MAP(helloQtPalette, hostQtPalette)
  //{{AFX_MSG_MAP(helloQtPalette)
  ON_WM_SIZE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(helloQtPalette, hostQtPalette);

void helloQtPaletteCmd()
{
  if (!m_pPalSet)
  {
    CAcModuleResourceOverride ThisRes;
    m_pPalSet = new hostUiPaletteSet();
    m_pPalSet->Create(L"Test Qt Palette Set", WS_CHILD | WS_DLGFRAME | WS_VISIBLE, CRect(30, 50, 270, 300),
      CWnd::FromHandle(adsw_acadMainWnd()), PSS_SNAP | PSS_PROPERTIES_MENU | PSS_AUTO_ROLLUP | PSS_CLOSE_BUTTON);
    m_pPalSet->EnableDocking(CBRS_ALIGN_ANY);
    m_pPalSet->RestoreControlBar();

    helloQtPalette* pPal = new helloQtPalette();
    pPal->Create(WS_CHILD | WS_VISIBLE, L"QtPolyTable", m_pPalSet, 0);
    m_pPalSet->AddPalette(pPal);

    QWidget* pPaletteWidget1 = pPal->paletteWidget();

    pWidgetChild = new HelloQtChild(pPaletteWidget1);

    QVBoxLayout* vbox = new QVBoxLayout(pPaletteWidget1);
    vbox->setSpacing(0);
    vbox->setMargin(0);
    vbox->addWidget(pWidgetChild);
    vbox->addStretch();

    //WId winId = le3->winId(); // Make Qt windows real HWND windows

    pPaletteWidget1->setLayout(vbox);
    pPaletteWidget1->show();

    CRect cr;
    m_pPalSet->GetClientRect(&cr);
    pPal->OnSize(0, cr.Width(), cr.Height()); // Force to resize palette widget, needed when system scale !=100%
  }
  else
  {
    m_pPalSet->Show(!m_pPalSet->IsWindowVisible());
  }
}

static AcEdReactor* testReactor = NULL;

void initApp()
{
  acedRegCmds->addCommand(L"HELLOQT_GROUP",
                          L"_HELLOQTPALETTE",
                          L"HELLOQTPALETTE",
                          ACRX_CMD_MODAL,
                          helloQtPaletteCmd);
  acedRegCmds->addCommand(_T("HELLOQT_GROUP"), _T("ASDK_ALINES"),
      _T("ALINES"), ACRX_CMD_MODAL, assocLines);
  AsdkObjectToNotify::rxInit();
  acrxBuildClassHierarchy();
}

void uninitApp()
{
  acedRegCmds->removeGroup(L"HELLOQT_GROUP");
  acedRegCmds->removeGroup(L"ASDK_ALINES");
  deleteAcRxClass(AsdkObjectToNotify::desc());
  if (m_pPalSet)
  {
    m_pPalSet->DestroyWindow();
    m_pPalSet = 0;
  }
}

extern "C" __declspec(dllexport) AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* appId) 
{
  switch (msg) 
  {
  case AcRx::kInitAppMsg:
    acrxDynamicLinker->unlockApplication(appId);
    acrxDynamicLinker->registerAppMDIAware(appId);
    testReactor = new AcEdReactor();
    acedEditor->addReactor(testReactor);
    initApp();
    break;

  case AcRx::kUnloadAppMsg:
    uninitApp();
    acedEditor->removeReactor(testReactor);
    break;
  }

  return AcRx::kRetOK;
}