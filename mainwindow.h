/*!
 * \file mainwindow.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for main window
*/
#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QtCore>
#include <QtGui>
#include <QtGui/QMainWindow>
#include <QTableWidgetItem>
#include "./machinemodel.h"
#include "./memorymodel.h"
#include "./simulationthread.h"
#include "./arrow.h"
#include "./machine.h"
#include "./machinescene.h"
#include "./machinetree.h"

namespace Ui {
    class MainWindowClass;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTreeView * getMachineTreeWidget();
    QTableView * getMemoryTableWidget();
    QGraphicsView * getGraphicsViewWidget();
    int openModel_internal(QString fileName, bool test);
    int closeModel_internal();

  signals:
    void clearSceneSelection();
    void updateScene();
    void startSimulation();
    void pauseSimulation();

  public slots:
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
    void machineTreeClicked(QModelIndex);
    void machineTreeContextMenu(QPoint);
    void repositionView(float x, float y);

  private slots:
    void on_pushButton_viewModel_clicked();
    void on_pushButtonClose_clicked();
    void on_actionHelp_triggered();
    void on_actionAbout_triggered();
    void reload_scene();

  private:
    Ui::MainWindowClass *ui;
    void handleNewAndOpenedModel(Machine * m);
    void defaultGuiSettings();
    SimulationThread * simulationThread;
    QToolBar * fileToolBar;
    MachineTree * machineTree;
    QList<Machine *> machines;
    Machine * currentMachine;
};

#endif  // MAINWINDOW_H_
