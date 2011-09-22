#ifndef MACHINE_H
#define MACHINE_H

#include <QTGui>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QFile>
#include "machinemodel.h"
#include "memorymodel.h"
#include "machinescene.h"

class Machine : public QTreeWidgetItem
{
    //Q_OBJECT

public:
    Machine(QTreeWidget *parent = 0)
         : QTreeWidgetItem(parent)
    {
        location = 0;
        memoryModel = new MemoryModel();
        machineModel = new MachineModel(memoryModel);
        machineScene = new MachineScene(machineModel);
        machineScene->setSceneRect(0, 0, 500, 500);

        QObject().connect(machineScene, SIGNAL(addedState(QString,bool)), machineModel, SLOT(addState(QString,bool)));
        QObject().connect(machineScene, SIGNAL(addedTransition(QString,QString,QString)), machineModel, SLOT(addTransition(QString,QString,QString)));
        QObject().connect(machineModel, SIGNAL(updateStateName(QString,QString)), machineScene, SLOT(updateStateName(QString,QString)));

        //connect(this->machine, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(transitionChanged(QModelIndex,QModelIndex)));
        QObject().connect(machineModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), machineModel, SLOT(transitionUpdated(QModelIndex,QModelIndex)));
    }
    ~Machine();
    bool writeModelXML(QFile * file);

    //void setName(QString n);
    //QString name() const { return myName; }

    MachineModel * machineModel;
    MemoryModel * memoryModel;
    MachineScene * machineScene;

private:
    //QString myName;
    QFile * location;
};

#endif // MACHINE_H
