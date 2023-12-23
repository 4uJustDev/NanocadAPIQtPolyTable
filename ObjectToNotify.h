#pragma once

class ObjectToNotify : public AcDbObject{

public:
    ACRX_DECLARE_MEMBERS(ObjectToNotify);
    ObjectToNotify() {};
    ObjectToNotify(HelloQtChild* helloQtChild) {
        this->helloQtChild = helloQtChild;
    };
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
    HelloQtChild* helloQtChild;
};