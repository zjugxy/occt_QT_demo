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
#include<StdSelect_BRepOwner.hxx>
#include<TopoDS.hxx>
#include<TopoDS_Edge.hxx>
#include<BRepFilletAPI_MakeFillet.hxx>

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
//debug demo
void Appfunc::SelecttoBuildPrim()
{
    EdgeSelectMode();
    havetoselect = 1;
}

void Appfunc::SelectionDonePrim()
{
    if(havetoselect!=0)
        return;

    auto myAISCtx = myviewer->Context();



    for (myAISCtx->InitSelected(); myAISCtx->MoreSelected(); myAISCtx->NextSelected())
    {
        std::cout<<"donecount"<<std::endl;
        Handle(SelectMgr_EntityOwner) anOwner = myAISCtx->SelectedOwner();
        Handle(AIS_InteractiveObject) anObj = Handle(AIS_InteractiveObject)::DownCast (anOwner->Selectable());
        if (Handle(StdSelect_BRepOwner) aBRepOwner = Handle(StdSelect_BRepOwner)::DownCast (anOwner))
        {
            // to be able to use the picked shape
            TopoDS_Shape aShape = aBRepOwner->Shape();
            if(aShape.ShapeType()==TopAbs_EDGE){
                std::cout<<"is_edge"<<std::endl;
                TopoDS_Edge aedge = TopoDS::Edge(aShape);
                Handle(AIS_Shape) obj =  Handle(AIS_Shape)::DownCast(anOwner->Selectable());
                auto baseshape = obj->Shape();
                myAISCtx->Remove(obj,false);
                BRepFilletAPI_MakeFillet MF(baseshape);
                MF.Add(10,aedge);
                TopoDS_Shape newshape = MF.Shape();
                Handle(AIS_Shape) aisShape = new AIS_Shape(newshape);
                myObject3d.Clear();
                myObject3d.Append(aisShape);
                    myAISCtx->ClearSelected(false);
                myviewer->Context()->Display(aisShape, AIS_Shaded, 0, Standard_False);
                myviewer->AXO_FitAll();
                std::cout<<"workthrough"<<std::endl;
                break;
            }
        }
    }

}


