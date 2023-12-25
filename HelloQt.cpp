#include "stdafx.h"
#include "hostUI.h"
#include "hostQt.h"

#include "HelloQtChild.h"
#include "ObjectToNotify.h"

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

ACRX_DXF_DEFINE_MEMBERS(ObjectToNotify, AcDbObject,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    0, \
    ASDKOBJECTTONOTIFY, persreac);

HelloQtChild* pWidgetChild;

class AcEdReactor : public NcEditorReactor
{
    void virtual pickfirstModified() override {

        pWidgetChild->showing();
        pWidgetChild->ui.pushButton_Update->setVisible(false);

         //Get the selection set
        struct resbuf* prbGrip = NULL;

        struct resbuf* prbPick = NULL;

        // Get the selection sets

        acedSSGetFirst(&prbGrip, &prbPick);

        long pickLen;

        if (prbPick->restype != RTPICKS)
            return;

        acedSSLength(prbPick->resval.rlname, &pickLen);
        
        if (pickLen == 1) {
            for (int i = 0; i < pickLen; i++)
            {
                ads_name entres;
                AcDbObjectId objId;
                acedSSName(prbPick->resval.rlname, i, entres);
                acdbGetObjectId(objId, entres);

                AcDbEntity* pEnt;
                acdbOpenAcDbEntity(pEnt, objId, kForWrite);

                AcGePoint3dArray arrayPnts;
                AcDb3dPolyline* polyline = AcDb3dPolyline::cast(pEnt);

                pWidgetChild->updateDataInTable(polyline);
                pEnt->close();

                if (polyline == NULL)
                    continue;

                polyline->close();
                pWidgetChild->showing();
                pWidgetChild->ui.pushButton_Update->setVisible(true);

            }
        }
        else {
            pWidgetChild->insertStub(pickLen);
        }


        acedSSFree(prbPick->resval.rlname);

        acutRelRb(prbPick);
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
  acedRegCmds->addCommand(L"PolyQtTable_GROUP",
                          L"_PolyQtTable",
                          L"PolyQtTable",
                          ACRX_CMD_MODAL,
                          helloQtPaletteCmd);

  testReactor = new AcEdReactor();
  acedEditor->addReactor(testReactor);

  ObjectToNotify::rxInit();
  acrxBuildClassHierarchy();
}

void uninitApp()
{
  acedRegCmds->removeGroup(L"PolyQtTable_GROUP");

  deleteAcRxClass(ObjectToNotify::desc());

  acedEditor->removeReactor(testReactor);
  delete testReactor;
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

    initApp();
    break;

  case AcRx::kUnloadAppMsg:
    uninitApp();
    break;
  }

  return AcRx::kRetOK;
}