#include "Appfunc.h"
#include"MakeBottle.h"
#include<AIS_Shape.hxx>
#include<AIS_ViewCube.hxx>
#include<AIS_InteractiveContext.hxx>
#include<BRep_Builder.hxx>
#include<BRepTools.hxx>
#include<TopTools_HSequenceOfShape.hxx>
#include<STEPControl_Reader.hxx>
#include<IGESControl_Reader.hxx>
void
Appfunc::Addbottle()
{
    myObject3d.Clear();
    myviewer->Context()->RemoveAll(Standard_True);

    TopoDS_Shape aBottle = MakeBottle(50, 70, 30);
    Handle(AIS_InteractiveObject) aShape = new AIS_Shape(aBottle);
    myObject3d.Append(aShape);
    Handle(AIS_ViewCube) aViewCube = new AIS_ViewCube();
    myObject3d.Append(aViewCube);

    for(NCollection_Vector<Handle(AIS_InteractiveObject)>::Iterator anIter(myObject3d);
         anIter.More(); anIter.Next())
    {
        const Handle(AIS_InteractiveObject)& anObject = anIter.Value();
        myviewer->Context()->Display(anObject, AIS_Shaded, 0, Standard_False);
    }
    myviewer->Viewer()->Redraw();
}



QString Appfunc::readBrep(const TCollection_AsciiString &filename)
{
    myObject3d.Clear();
    myviewer->Context()->RemoveAll(Standard_True);
    TopoDS_Shape aTopoShape;
    BRep_Builder aBuilder;
    Standard_Boolean aResult = BRepTools::Read(aTopoShape, filename.ToCString(), aBuilder);
    if (aResult)
    {
        Handle(AIS_Shape) anAisShape = new AIS_Shape(aTopoShape);
        myObject3d.Append(anAisShape);
        myviewer->Context()->Display(anAisShape, AIS_Shaded, 0, Standard_False);
        myviewer->AXO_FitAll();
        return QString("A BREP file was read successfully");
    }
    else
    {
        return QString("A BREP file was not read successfully");
    }
}

QString Appfunc::readStep(const TCollection_AsciiString &filename)
{
    myObject3d.Clear();
    myviewer->Context()->RemoveAll(Standard_True);

    Handle(TopTools_HSequenceOfShape) aSequence = new TopTools_HSequenceOfShape();
    STEPControl_Reader aReader;
    const IFSelect_ReturnStatus aStatus = aReader.ReadFile(filename.ToCString());
    if (aStatus != IFSelect_RetDone)
    {
        return QString("A BREP file was not read successfully");
    }

    bool anIsFailsOnly = false;
    aReader.PrintCheckLoad(anIsFailsOnly, IFSelect_ItemsByEntity);

    int aRootsNumber = aReader.NbRootsForTransfer();
    aReader.PrintCheckTransfer(anIsFailsOnly, IFSelect_ItemsByEntity);
    for (Standard_Integer i = 1; i <= aRootsNumber; i++)
    {
        aReader.TransferRoot(i);
    }

    int aShapesNumber = aReader.NbShapes();
    for (int i = 1; i <= aShapesNumber; i++)
    {
        TopoDS_Shape aTopoShape = aReader.Shape(i);
        Handle(AIS_Shape) anAisShape = new AIS_Shape(aTopoShape);
        myObject3d.Append(anAisShape);
        myviewer->Context()->Display(anAisShape, AIS_Shaded, 0, Standard_False);
    }
    myviewer->AXO_FitAll();
    return QString("A STEP file was read successfully");
}

QString Appfunc::readIges(const TCollection_AsciiString &filename)
{
    myObject3d.Clear();
    myviewer->Context()->RemoveAll(Standard_True);

    IGESControl_Reader aReader;
    int status = aReader.ReadFile (filename.ToCString());
    if (status != IFSelect_RetDone)
    {
        return QString("A IGES file was not read successfully");
    }
    aReader.TransferRoots();
    TopoDS_Shape aTopoShape = aReader.OneShape();
    Handle(AIS_Shape) anAisShape = new AIS_Shape (aTopoShape);
    myObject3d.Append(anAisShape);
    myviewer->Context()->Display(anAisShape, AIS_Shaded, 0, Standard_False);
    myviewer->AXO_FitAll();
    return QString("A IGES file was read successfully");

}

void Appfunc::VertexSelectMode()
{
    myviewer->Context()->Deactivate();
    myviewer->Context()->Activate(AIS_Shape::SelectionMode(TopAbs_VERTEX));
}

void Appfunc::EdgeSelectMode()
{
    myviewer->Context()->Deactivate();
    myviewer->Context()->Activate(AIS_Shape::SelectionMode(TopAbs_EDGE));
}

void Appfunc::FaceSelectMode()
{
    myviewer->Context()->Deactivate();
    myviewer->Context()->Activate(AIS_Shape::SelectionMode(TopAbs_FACE));
}

void Appfunc::NeutralSelectMode()
{
    myviewer->Context()->Deactivate();
    myviewer->Context()->Activate(0);
}


