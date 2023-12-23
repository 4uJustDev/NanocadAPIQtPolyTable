#include "stdafx.h"
#include "ObjectToNotify.h"

void ObjectToNotify::modified(const AcDbObject* pObj)
{
    AcDb3dPolyline* pPolyline = AcDb3dPolyline::cast(pObj);

    if (!pPolyline) {
        const TCHAR* cstr = pObj->isA()->name();
        acutPrintf(_T("This is a %s.\n"), cstr);
        acutPrintf(_T("I only work with polylines. Sorry.\n"));
        return;
    }

    acutPrintf(_T("Modified polylines.\n"));
    helloQtChild->updateDataInTable(pPolyline);
}

Acad::ErrorStatus ObjectToNotify::dwgInFields(AcDbDwgFiler* filer)
{
    assertWriteEnabled();
    AcDbObject::dwgInFields(filer);
    filer->readItem(&mFactor);
    filer->readItem((AcDbSoftPointerId*)&mId);
    return filer->filerStatus();
}

Acad::ErrorStatus ObjectToNotify::dwgOutFields(AcDbDwgFiler* filer) const
{
    assertReadEnabled();
    AcDbObject::dwgOutFields(filer);
    filer->writeItem(mFactor);
    filer->writeItem((AcDbSoftPointerId&)mId);
    return filer->filerStatus();
}

Acad::ErrorStatus ObjectToNotify::dxfInFields(AcDbDxfFiler* filer)
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
    if (!filer->atSubclassData(_T("ObjectToNotify"))) {
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

Acad::ErrorStatus ObjectToNotify::dxfOutFields(AcDbDxfFiler* filer) const
{
    assertReadEnabled();
    AcDbObject::dxfOutFields(filer);
    filer->writeItem(AcDb::kDxfSubclass,
        _T("ObjectToNotify"));
    filer->writeItem(AcDb::kDxfReal, mFactor);
    filer->writeItem(AcDb::kDxfSoftPointerId, mId);
    return filer->filerStatus();
}
