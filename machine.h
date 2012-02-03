/*!
 * \file machine.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for model machine
*/
#ifndef MACHINE_H_
#define MACHINE_H_

#include <QList>
#include <QVariant>
#include <QFile>
#include "./machinemodel.h"
#include "./memorymodel.h"
#include "./machinescene.h"
#include "./timeunit.h"

class MachineItem;

class Machine {
  public:
    Machine(int type, Machine *parent = 0);
    ~Machine();

    bool writeModelXML(QFile * file);

    void appendChild(Machine *child);

    Machine *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    Machine *parent();
    void removeChild(Machine * m);
    void insertChild(Machine * m, int index);
    void addTransitionString(QString name, QString cs, QString ns,
        Condition c, Mpost mpost,
        Communication input, Communication output, QString desc);

    QStringList getTimeUnits(bool b = true);
    QStringList getAgentMemoryNames(QString name, bool b = true);
    QStringList getMessageMemoryNames(QString name, bool b = true);
    QStringList getMessageNames(bool b = true);
    QStringList getDataTypes(bool b = true);
    Machine * getMessageType(QString name, bool = true);
    bool isEnabled() { return enabled; }
    void setEnabled(bool b) { enabled = b; }
    Machine * rootModel() { return myRootModel; }
    void setRootModel(Machine * m) { myRootModel = m; }
    void setTimeUnit(TimeUnit t) { myTimeUnit = t; }
    TimeUnit timeUnit() const { return myTimeUnit; }
    QString filePath();
    void setTimeUnitName(QString s) { myTimeUnit.name = s; }

    MachineModel * machineModel;
    MemoryModel * memoryModel;
    MachineScene * machineScene;

    QStringList timeUnitNames;
    QString name;
    QString description;
    /* -1-rootItem
     *  0-model
     *  1-agent
     *  2-message
     *  3-environment
     *  4-timeunit
     *  5-datatype
     *  6-functionFiles
     */
    int type;
    bool isSubModel;
    QString fileDirectory;
    QString fileName;
    QList<Machine*> childItems;

  private:
    QList<QVariant> itemData;
    Machine *parentItem;
    QFile * location;
    bool enabled;
    Machine * myRootModel;
    TimeUnit myTimeUnit;
};

#endif  // MACHINE_H_
