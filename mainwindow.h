#include "OcctQtViewer.h"

#include <Standard_WarningsDisable.hxx>
#include <QApplication>
#include <QSurfaceFormat>

#include <QAction>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <Standard_WarningsRestore.hxx>
#include"Appfunc.h"
#include <Standard_Version.hxx>
#include<QStatusBar>
class MyMainWindow : public QMainWindow
{
    OcctQtViewer* myViewer;
    Handle(Appfunc) myappfunc;
    QStatusBar* statusBar;
    QMenuBar* aMenuBar;
public:
    MyMainWindow();
    void ReadBrepFile();
    void ReadStepFile();
    void ReadIgesFile();

    void InitFileMenu();
    void InitImportMenu();
    void InitDebugMenu();
    void InitSelectMenu();

    void PortDebugInfo();
};
