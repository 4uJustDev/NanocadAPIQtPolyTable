#include "stdafx.h"
#include "ObjectToNotify.h"

//ObjectToNotify::ObjectToNotify() : AcDbObject() {};

void ObjectToNotify::modified(const AcDbObject* pObj)
{
    AcDb3dPolyline* pPolyline = AcDb3dPolyline::cast(pObj);
    //if (!pPolyline) {
    //    const TCHAR* cstr = pObj->isA()->name();
    //    acutPrintf(_T("This is a %s.\n"), cstr);
    //    acutPrintf(_T("I only work with polylines. Sorry.\n"));
    //    return;
    //}
    acutPrintf(_T("Modified polylines.\n"));
    helloQtChild->updateDataInTable(pPolyline);

   // acutPrintf(_T("\nReactor attached to %lx calling %lx.\n"),
       // pPolyline->objectId().asOldId(), mId.asOldId());

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
