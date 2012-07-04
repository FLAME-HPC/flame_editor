/*!
 * \file machine.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of model machine
 */
#include <QStringList>
#include "./machine.h"

Machine::Machine(int t, Machine *parent) {
    type = t;
    parentItem = parent;
    // itemData = data;
    location = 0;
    enabled = true;
    isSubModel = false;
    memoryModel = new MemoryModel();
    machineModel = 0;
    machineScene = 0;

    if (t == 0) {  // model
        machineModel = 0;
        machineScene = new MachineScene(this);
        machineScene->setSceneRect(0, 0, 500, 500);

        memoryModel->setIsModel(true);
        memoryModel->addVariable("string", "version", "");
        memoryModel->addVariable("string", "author", "");
        memoryModel->addVariable("string", "description", "");
    } else if (t == 1) {  // agent
        machineModel = new MachineModel(memoryModel);
        machineScene = new MachineScene(this);
        machineScene->setSceneRect(0, 0, 500, 500);
        machineScene->setMachineModel(machineModel);

        QObject::connect(machineModel, SIGNAL(updateStateName(State*)),
                machineScene, SLOT(updateStateName(State*)));
        QObject::connect(machineModel,
                SIGNAL(updateTransitionName(Transition*)),
                machineScene, SLOT(updateTransitionName(Transition*)));
        QObject::connect(machineModel, SIGNAL(updateInput(Transition*)),
                machineScene, SLOT(updateInput(Transition*)));
        QObject::connect(machineModel, SIGNAL(updateOutput(Transition*)),
                machineScene, SLOT(updateOutput(Transition*)));
    }
}

Machine::~Machine() {
    // If not root model then remove from parent childItems
    if (type != -1) {
        parent()->removeChild(this);
    }

    qDeleteAll(childItems);
    delete memoryModel;
    if (machineModel != 0) delete machineModel;
    if (machineScene != 0) delete machineScene;
}

void Machine::removeChild(Machine *m) {
    childItems.removeAt(childItems.indexOf(m));
}

void Machine::insertChild(Machine *m, int index) {
    childItems.insert(index, m);
}

/*void Machine::setName(QString n)
{
    //myName = n;
    //machineItem->setText(0, n);
}*/

void Machine::addTransitionString(QString fname, QString cs, QString ns,
        Condition c, Mpost mpost, Communication input, Communication output,
        QString desc) {
    Transition * t =
            machineModel->addTransitionString(fname, cs, ns, c,
                    mpost, input, output, desc);
    // machineScene->addTransitionString(name, cs, fname, ns, 0);
    machineScene->addTransitionTransition(name, t);
}

void Machine::appendChild(Machine *item) {
    childItems.append(item);
}

Machine *Machine::child(int row) {
    return reinterpret_cast<Machine*>(childItems.value(row));
}

int Machine::childCount() const {
    return childItems.count();
}

int Machine::columnCount() const {
    return 2;  // itemData.count();
}

QVariant Machine::data(int column) const {
    // return itemData.value(column);
    if (type == 0) {
        if (column == 0) return qVariantFromValue(QString("model"));
        else if (column == 1) return qVariantFromValue(name);
    } else if (type == 1) {
        if (column == 0) return qVariantFromValue(QString("agent"));
        else if (column == 1) return qVariantFromValue(name);
        else if (column == 2) return qVariantFromValue(description);
    } else if (type == 2) {
        if (column == 0) return qVariantFromValue(QString("message"));
        else if (column == 1) return qVariantFromValue(name);
        else if (column == 2) return qVariantFromValue(description);
    } else if (type == 3) {
        if (column == 0) return qVariantFromValue(QString("environment"));
        else if (column == 1) return qVariantFromValue(QString(""));
    } else if (type == 4) {
        if (column == 0) return qVariantFromValue(QString("time unit"));
        else if (column == 1) return qVariantFromValue(timeUnit().toString());
    } else if (type == 5) {
        if (column == 0) return qVariantFromValue(QString("data type"));
        else if (column == 1) return qVariantFromValue(name);
        else if (column == 2) return qVariantFromValue(description);
    } else if (type == 6) {
        if (column == 0) return qVariantFromValue(QString("function file"));
        else if (column == 1) return qVariantFromValue(name);
    }
    return QVariant();
}

Machine *Machine::parent() {
    return parentItem;
}

int Machine::row() const {
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<Machine*>(this));

    return 0;
}

QStringList Machine::getTimeUnits(bool b) {
    QStringList list;

    // If first call then find root model
    if (b) {
        list.append(myRootModel->getTimeUnits(false));
        list.prepend("iteration");
        list = list.toSet().toList();  // Remove duplicates
        list.removeOne(timeUnit().name);  // Remove this time unit
    } else {
        if (type == 0 || type == 3) {  // model
            for (int i = 0; i < childCount(); i++) {
                if (child(i)->type == 4) {  // time unit
                    if (!list.contains(child(i)->timeUnit().name))
                        list.append(child(i)->timeUnit().name);
                } else if (child(i)->type == 0 || child(i)->type == 3) {
                    // model or environment
                    list.append(child(i)->getTimeUnits(false));
                }
            }
        }
    }

    return list;
}

QStringList Machine::getMessageNames(bool b) {
    QStringList list;

    // If first call then find root model
    if (b) {
        list.append(myRootModel->getMessageNames(false));
    } else {
        if (type == 0) {  // model
            for (int i = 0; i < childCount(); i++) {
                if (child(i)->type == 2) {  // message
                    list.append(child(i)->name);
                } else if (child(i)->type == 0) {  // model
                    list.append(child(i)->getMessageNames(false));
                }
            }
        }
    }

    return list;
}

QStringList Machine::getAgentMemoryNames(QString n, bool b) {
    QStringList list;

    // If first call then find root model
    if (b) {
        list.append(myRootModel->getAgentMemoryNames(n, false));
    } else {
        // Add this agent memory if called n
        if (type == 1 && name == n) {
            list.append(memoryModel->getNames());
        }
        if (type == 0) {  // model
            for (int i = 0; i < childCount(); i++) {
                list.append(child(i)->getAgentMemoryNames(n, false));
            }
        }
    }

    return list;
}

QStringList Machine::getMessageMemoryNames(QString n, bool b) {
    QStringList list;

    // If first call then find root model
    if (b) {
        list.append(myRootModel->getMessageMemoryNames(n, false));
    } else {
        // Add this message memory if called n
        if (type == 2 && name == n) {
            list.append(memoryModel->getNames());
        }
        if (type == 0) {  // model
            for (int i = 0; i < childCount(); i++) {
                list.append(child(i)->getMessageMemoryNames(n, false));
            }
        }
    }

    return list;
}

QStringList Machine::getDataTypes(bool b) {
    QStringList list;

    // If first call then find root model
    if (b) {
        list.append("int");
        list.append("float");
        list.append("double");
        list.append("char");
        list.append(myRootModel->getDataTypes(false));
        if (type != 2 || type != 3) {  // not message or environment
            // For every data type add dynamic array version
            int listSize = list.size();
            for (int i = 0; i < listSize; i++) {
                QString n = list.at(i);
                n.append("_array");
                list.append(n);
            }
        }
    } else {
        if (type == 0 || type == 3) {  // model
            for (int i = 0; i < childCount(); i++) {
                if (child(i)->type == 5) {  // datatype
                    list.append(child(i)->name);  // memoryModel->getNames());
                } else if (child(i)->type == 0 || child(i)->type == 3) {
                    // model or environment
                    list.append(child(i)->getDataTypes(false));
                }
            }
        }
    }

    return list;
}

Machine * Machine::getMessageType(QString n, bool b) {
    Machine * m = 0;

    if (b) {
        myRootModel = getMessageType(n, false);
    } else {
        // Add this agent memory if called n
        if (type == 2 && name == n) {
            return this;
        }
        if (type == 0) {  // model
            for (int i = 0; i < childCount(); i++) {
                Machine * m2 = child(i)->getMessageType(n, false);
                if (m2) return m2;
            }
        }
    }

    return m;
}

QString Machine::filePath() {
    QString filepath;
    filepath.append(fileDirectory);
    filepath.append("/");
    filepath.append(fileName);
    return filepath;
}
