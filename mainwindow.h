#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtGui>
#include <QtGui/QMainWindow>
#include <QTableWidgetItem>
#include "machinemodel.h"
#include "memorymodel.h"
#include "simulationthread.h"
#include "arrow.h"
//#include "modeltree.h"
#include "machine.h"
#include "machinescene.h"

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void clearSceneSelection();
    void updateScene();
    void startSimulation();
    void pauseSimulation();

public slots:
    //void transitionChanged(QModelIndex topLeft, QModelIndex bottomRight);
    void startModel();
    void pauseModel();
    void stopModel();
    void selectTransition(QString n);
    void selectState(QString n);
    void updateStatusLabel(QString s);
    void insertMemory();
    void deleteMemory();
    void newModel();
    void openModel();
    void saveModel();
    void changeModel();

private:
    void enableUI(bool enable);

    Ui::MainWindowClass *ui;
    MachineModel * machine;
    SimulationThread * simulationThread;
    QToolBar * fileToolBar;
    QString modelPath;
    QString modelName;
    int noMachines;
    //ModelTree * modelTree;
    QList<Machine *> machines;
    MemoryModel * memory;
    MachineScene * scene;
    Machine * currentMachine;
};

#endif // MAINWINDOW_H
