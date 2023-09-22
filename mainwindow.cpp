#include"mainwindow.h"
#include<QFileDialog>


MyMainWindow::MyMainWindow() : myViewer (nullptr),myappfunc(nullptr)
{
    {
        // menu bar with Quit item
        aMenuBar = new QMenuBar();
        InitFileMenu();
        InitImportMenu();
        InitSelectMenu();
        InitDebugMenu();

        setMenuBar (aMenuBar);
    }
    {
        // 3D Viewer and some controls on top of it
        myViewer = new OcctQtViewer();
        //            QVBoxLayout* aLayout = new QVBoxLayout (myViewer);
        //            aLayout->setDirection (QBoxLayout::BottomToTop);
        //            aLayout->setAlignment (Qt::AlignBottom);
        //            {
        //                QPushButton* aQuitBtn = new QPushButton ("About");
        //                aLayout->addWidget (aQuitBtn);
        //                connect (aQuitBtn, &QPushButton::clicked, [this]()
        //                        {
        //                            QMessageBox::information (0, "About Sample", QString()
        //                                                                            + "OCCT 3D Viewer sample embedded into Qt Widgets.\n\n"
        //                                                                            + "Open CASCADE Technology v." OCC_VERSION_STRING_EXT "\n"
        //                                                                            + "Qt v." QT_VERSION_STR "\n\n"
        //                                                                            + "OpenGL info:\n"
        //                                                                            + myViewer->getGlInfo());
        //                        });
        //            }
        //            {
        //                QWidget* aSliderBox = new QWidget();
        //                QHBoxLayout* aSliderLayout = new QHBoxLayout (aSliderBox);
        //                {
        //                    QLabel* aSliderLabel = new QLabel ("Background");
        //                    aSliderLabel->setStyleSheet ("QLabel { background-color: rgba(0, 0, 0, 0); color: white; }");
        //                    aSliderLabel->setGeometry (50, 50, 50, 50);
        //                    aSliderLabel->adjustSize();
        //                    aSliderLayout->addWidget (aSliderLabel);
        //                }
        //                {
        //                    QSlider* aSlider = new QSlider (Qt::Horizontal);
        //                    aSlider->setRange (0, 255);
        //                    aSlider->setSingleStep (1);
        //                    aSlider->setPageStep (15);
        //                    aSlider->setTickInterval (15);
        //                    aSlider->setTickPosition (QSlider::TicksRight);
        //                    aSlider->setValue (0);
        //                    aSliderLayout->addWidget (aSlider);
        //                    connect (aSlider, &QSlider::valueChanged, [this](int theValue)
        //                            {
        //                                const float aVal = theValue / 255.0f;
        //                                const Quantity_Color aColor (aVal, aVal, aVal, Quantity_TOC_sRGB);

        //                                for (const Handle(V3d_View)& aSubviewIter : myViewer->View()->Subviews())
        //                                {
        //                                    aSubviewIter->SetBgGradientColors (aColor, Quantity_NOC_BLACK, Aspect_GradientFillMethod_Elliptical);
        //                                    aSubviewIter->Invalidate();
        //                                }
        //                                //myViewer->View()->SetBackgroundColor (aColor);
        //                                myViewer->View()->SetBgGradientColors (aColor, Quantity_NOC_BLACK, Aspect_GradientFillMethod_Elliptical);
        //                                myViewer->View()->Invalidate();
        //                                myViewer->update();
        //                            });
        //                }
        //                aLayout->addWidget (aSliderBox);
        //            }
        setCentralWidget (myViewer);
        myappfunc = new Appfunc(myViewer);
        statusBar = new QStatusBar(this);
        setStatusBar(statusBar);

    }
}

void MyMainWindow::InitFileMenu(){
    QMenu* aMenuWindow = aMenuBar->addMenu ("&File");
    {
        QAction* anActionSplit = new QAction(aMenuWindow);
        anActionSplit->setText("Split Views");
        aMenuWindow->addAction(anActionSplit);
        connect(anActionSplit, &QAction::triggered, [this]()
                {
                    if (!myViewer->View()->Subviews().IsEmpty())
                    {
                        // remove subviews
                        myViewer->View()->View()->SetSubviewComposer(false);
                        NCollection_Sequence<Handle(V3d_View)> aSubviews = myViewer->View()->Subviews();
                        for (const Handle(V3d_View)& aSubviewIter : aSubviews)
                        {
                            aSubviewIter->Remove();
                        }
                        myViewer->OnSubviewChanged(myViewer->Context(), nullptr, myViewer->View());
                    }
                    else
                    {
                        // create two subviews splitting window horizontally
                        myViewer->View()->View()->SetSubviewComposer(true);

                        Handle(V3d_View) aSubView1 = new V3d_View(myViewer->Viewer());
                        aSubView1->SetImmediateUpdate(false);
                        aSubView1->SetWindow(myViewer->View(), Graphic3d_Vec2d(0.5, 1.0),
                                             Aspect_TOTP_LEFT_UPPER, Graphic3d_Vec2d(0.0, 0.0));

                        Handle(V3d_View) aSubView2 = new V3d_View(myViewer->Viewer());
                        aSubView2->SetImmediateUpdate(false);
                        aSubView2->SetWindow(myViewer->View(), Graphic3d_Vec2d(0.5, 1.0),
                                             Aspect_TOTP_LEFT_UPPER, Graphic3d_Vec2d(0.5, 0.0));

                        myViewer->OnSubviewChanged(myViewer->Context(), nullptr, aSubView1);
                    }
                    myViewer->View()->Invalidate();
                    myViewer->update();
                });
    }
    {
        QAction* anActionQuit = new QAction (aMenuWindow);
        anActionQuit->setText ("Quit");
        aMenuWindow->addAction (anActionQuit);
        connect (anActionQuit, &QAction::triggered, [this]()
                {
                    close();
                });
    }
    {
        QAction* anActionAddBottle = new QAction (aMenuWindow);
        anActionAddBottle->setText ("AddBottle");
        aMenuWindow->addAction (anActionAddBottle);
        connect (anActionAddBottle, &QAction::triggered, [this]()
                {
                    myappfunc->Addbottle();
                });
    }
}

void MyMainWindow::InitImportMenu()
{
    QMenu* aMenuimport = aMenuBar->addMenu("Import");
    {
        QAction* anActionImpBrep = new QAction(aMenuimport);
        anActionImpBrep->setText("BREP import");
        aMenuimport->addAction(anActionImpBrep);
        connect(anActionImpBrep,&QAction::triggered,[this](){
            ReadBrepFile();
        });
    }
    {
        QAction* anActionImpStep = new QAction(aMenuimport);
        anActionImpStep->setText("STEP import");
        aMenuimport->addAction(anActionImpStep);
        connect(anActionImpStep,&QAction::triggered,[this](){
            ReadStepFile();
        });
    }
    {
        QAction* anActionImpIges = new QAction(aMenuimport);
        anActionImpIges->setText("IGES import");
        aMenuimport->addAction(anActionImpIges);
        connect(anActionImpIges,&QAction::triggered,[this](){
            ReadIgesFile();
        });
    }


}

void MyMainWindow::InitDebugMenu()
{
    QMenu* aMenuDebug = aMenuBar->addMenu("Debug");
    {
        QAction* anActionPortDebug = new QAction(aMenuDebug);
        anActionPortDebug->setText("Port Debug");
        aMenuDebug->addAction(anActionPortDebug);
        connect(anActionPortDebug,&QAction::triggered,[this](){
            PortDebugInfo();
        });
    }
    {
        QAction* anActionSelectionDebug = new QAction(aMenuDebug);
        anActionSelectionDebug->setText("Selection Debug");
        aMenuDebug->addAction(anActionSelectionDebug);
        connect(anActionSelectionDebug,&QAction::triggered,[this](){
            myappfunc->SelecttoBuildPrim();
        });
    }
    {
        QAction* anActionPrimDebug = new QAction(aMenuDebug);
        anActionPrimDebug->setText("Prim Debug");
        aMenuDebug->addAction(anActionPrimDebug);
        connect(anActionPrimDebug,&QAction::triggered,[this](){
            myappfunc->SelectionDonePrim();
        });
    }
}

void MyMainWindow::InitSelectMenu()
{
    QMenu* aMenuSelect = aMenuBar->addMenu("Select");
    {
        QAction* anActionVertex = new QAction(aMenuSelect);
        anActionVertex->setText("Vertex Mode");
        aMenuSelect->addAction(anActionVertex);
        connect(anActionVertex,&QAction::triggered,[this](){
            myappfunc->VertexSelectMode();
        });
    }
    {
        QAction* anActionEdge = new QAction(aMenuSelect);
        anActionEdge->setText("Edge Mode");
        aMenuSelect->addAction(anActionEdge);
        connect(anActionEdge,&QAction::triggered,[this](){
            myappfunc->EdgeSelectMode();
        });
    }
    {
        QAction* anActionFace = new QAction(aMenuSelect);
        anActionFace->setText("Face Mode");
        aMenuSelect->addAction(anActionFace);
        connect(anActionFace,&QAction::triggered,[this](){
            myappfunc->FaceSelectMode();
        });
    }
    {
        QAction* anActionDefault = new QAction(aMenuSelect);
        anActionDefault->setText("Neutral Mode");
        aMenuSelect->addAction(anActionDefault);
        connect(anActionDefault,&QAction::triggered,[this](){
            myappfunc->NeutralSelectMode();
        });
    }

}

void MyMainWindow::PortDebugInfo()
{
    Standard_Real mywid,myhei;
    myViewer->Context()->LastActiveView()->Size(mywid,myhei);
    qDebug()<<"occt wid,height"<<mywid<<" "<<myhei;
    QSize tempsize = myViewer->size();
    qDebug()<<"qt   wid,height"<<tempsize.width()<<" "<<tempsize.height();
}

void MyMainWindow::TestSelction()
{
    return;
}



void MyMainWindow::ReadBrepFile()
{
    QString selectedFilePath = QFileDialog::getOpenFileName(
        this,
        "选择BREP文件",
        "",
        "BREP文件 (*.brep)"
        );

    if (!selectedFilePath.isEmpty()) {
        QString ret = myappfunc->readBrep(selectedFilePath.toUtf8().data());
        statusBar->showMessage(ret);
    }
}

void MyMainWindow::ReadStepFile()
{
    QString selectedFilePath = QFileDialog::getOpenFileName(
        this,
        "选择STEP文件",
        "",
        "STEP文件 (*.step)"
        );

    if (!selectedFilePath.isEmpty()) {
        QString ret = myappfunc->readStep(selectedFilePath.toUtf8().data());
        statusBar->showMessage(ret);
    }
}

void MyMainWindow::ReadIgesFile()
{
    QString selectedFilePath = QFileDialog::getOpenFileName(
        this,
        "选择IGES文件",
        "",
        "IGES文件 (*.iges)"
        );

    if (!selectedFilePath.isEmpty()) {
        QString ret = myappfunc->readIges(selectedFilePath.toUtf8().data());
        statusBar->showMessage(ret);
    }
}
