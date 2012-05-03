#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /* Setup the main window UI using the
     * Qt Form (mainwindow.ui) */
    ui->setupUi(this);

    /* Create the machine scene object */
    machineScene = new MachineScene(this);
    /* Set the graphics view to use the machine scene object */
    ui->graphicsView->setScene(machineScene);
}

MainWindow::~MainWindow()
{
    delete machineScene;
    delete ui;
}
