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

  signals:
    void machineSelectedSignal(Machine *);
    void edit();
    void move();

  public slots:
    void selectTransition(QString n);
    void selectState(QString n);
    void repositionView(float x, float y);
    void machineSelected(Machine *);
    void functionSelected(Transition *);
    void functionDoubleClicked(Transition *);

  private slots:
    void on_actionHelp_triggered();
    void on_actionAbout_triggered();
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionClose_triggered();
    void on_pushButton_viewModel_clicked();
    void on_pushButton_update_clicked();
    void on_pushButtonAddMemory_clicked();
    void on_pushButtonDeleteMemory_clicked();
    void on_toolButton_edit_clicked();
    void on_toolButton_move_clicked();
    void on_treeView_machines_clicked(const QModelIndex &index);
    void on_treeView_machines_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindowClass *ui;
    void handleNewAndOpenedModel(Machine * m);
    void defaultGuiSettings();
    void handleMachineSelected(Machine * m);
    int openModel_internal(QString fileName, bool test);
    MachineTree * machineTree;
    Machine * currentMachine;
    Machine * rootMachine;
};

#endif  // MAINWINDOW_H_
