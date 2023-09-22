#ifndef APPFUNC_H
#define APPFUNC_H


#include <AIS_InteractiveObject.hxx>
#include <NCollection_Vector.hxx>
#include <TCollection_AsciiString.hxx>
#include"OcctQtViewer.h"
#include<QString>
#include"GlobalVaries.h"

class Appfunc:public Standard_Transient
{
    DEFINE_STANDARD_RTTI_INLINE(Appfunc,Standard_Transient)


public:
    Appfunc(OcctQtViewer* _temp):myviewer(_temp){
    }
    void Addbottle();
    QString readBrep(const TCollection_AsciiString& filename);
    QString readStep(const TCollection_AsciiString& filename);
    QString readIges(const TCollection_AsciiString& filename);

    void VertexSelectMode();
    void EdgeSelectMode();
    void FaceSelectMode();
    void NeutralSelectMode();

    void SelecttoBuildPrim();
    void SelectionDonePrim();


private:
    NCollection_Vector<Handle(AIS_InteractiveObject)> myObject3d;
    OcctQtViewer* myviewer;
};

#endif // APPFUNC_H
