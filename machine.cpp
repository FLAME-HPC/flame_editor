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

bool Machine::writeModelXML(QFile * file) {
    QXmlStreamWriter stream(file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("xmodel");
    stream.writeAttribute("version", "2");
    stream.writeAttribute("xmlns:xsi",
            "http://www.w3.org/2001/XMLSchema-instance");
    stream.writeAttribute("xsi:noNamespaceSchemaLocation",
            "http://flame.ac.uk/schema/xmml_v2.xsd");

    stream.writeTextElement("name", name);
    stream.writeTextElement(memoryModel->variables[0].name,
            memoryModel->variables[0].description);  // version
    stream.writeTextElement(memoryModel->variables[1].name,
            memoryModel->variables[1].description);  // author
    stream.writeTextElement(memoryModel->variables[2].name,
            memoryModel->variables[2].description);  // description

    bool hasSubModels = false;
    for (int i = 0; i < childItems.size(); i++) {
        if (childItems[i]->type == 0) hasSubModels = true;
    }
    // Sub models
    if (hasSubModels) {
        stream.writeStartElement("models");
        for (int i = 0; i < childItems.size(); i++) {
            if (childItems[i]->type == 0) {
                stream.writeStartElement("model");

                QDir dir(fileDirectory);
                QString s = dir.canonicalPath();
                QDir dir2(s);
                s = dir2.relativeFilePath(childItems[i]->filePath());
                stream.writeTextElement("file", s);

                if (childItems[i]->enabled) stream.writeTextElement(
                        "enabled", "true");
                else
                    stream.writeTextElement("enabled", "false");
                stream.writeEndElement();  // model
            }
        }
        stream.writeEndElement();  // models
    }
    // Environment
    Machine * env = 0;
    for (int i = 0; i < childItems.size(); i++)
        if (childItems[i]->type == 3) env = childItems[i];
    Q_ASSERT(env != 0);
    stream.writeStartElement("environment");
    if (env->memoryModel->variables.size() > 0) {
        stream.writeStartElement("constants");
        for (int i = 0; i < env->memoryModel->variables.size(); i++) {
            stream.writeStartElement("variable");
            stream.writeTextElement("type",
                    env->memoryModel->variables[i].type);
            stream.writeTextElement("name",
                    env->memoryModel->variables[i].name);
            stream.writeTextElement("description",
                    env->memoryModel->variables[i].description);
            stream.writeEndElement();  // variable
        }
        stream.writeEndElement();  // constants
    }

    bool hasFunctionFiles = false;
    bool hasDataTypes = false;
    bool hasTimeUnits = false;
    for (int i = 0; i < env->childItems.size(); i++) {
        if (env->childItems[i]->type == 6) hasFunctionFiles = true;
        if (env->childItems[i]->type == 5) hasDataTypes = true;
        if (env->childItems[i]->type == 4) hasTimeUnits = true;
    }

    if (hasFunctionFiles) {
        stream.writeStartElement("functionFiles");
        for (int i = 0; i < env->childItems.size(); i++) {
            if (env->childItems[i]->type == 6) {
                stream.writeTextElement("file", env->childItems[i]->fileName);
            }
        }
        stream.writeEndElement();  // functionFiles
    }
    if (hasTimeUnits) {
        stream.writeStartElement("timeUnits");
        for (int i = 0; i < env->childItems.size(); i++) {
            if (env->childItems[i]->type == 4) {
                stream.writeStartElement("timeUnit");
                stream.writeTextElement("name", env->childItems[i]->
                        myTimeUnit.name);
                stream.writeTextElement("unit", env->childItems[i]->
                        myTimeUnit.unit);
                stream.writeTextElement("period", QString::number(
                        env->childItems[i]->myTimeUnit.period));
                stream.writeEndElement();  // timeUnit
            }
        }
        stream.writeEndElement();  // timeUnits
    }
    if (hasDataTypes) {
        stream.writeStartElement("dataTypes");
        for (int i = 0; i < env->childItems.size(); i++) {
            if (env->childItems[i]->type == 5) {
                stream.writeStartElement("dataType");
                stream.writeTextElement("name", env->childItems[i]->name);
                stream.writeTextElement("description", env->childItems[i]->
                        description);
                stream.writeStartElement("variables");
                for (int j = 0; j < env->childItems[i]->memoryModel->
                variables.size(); j++) {
                    stream.writeStartElement("variable");
                    stream.writeTextElement("type", env->childItems[i]->
                            memoryModel->variables[j].type);
                    stream.writeTextElement("name", env->childItems[i]->
                            memoryModel->variables[j].name);
                    stream.writeTextElement("description", env->childItems[i]->
                            memoryModel->variables[j].description);
                    stream.writeEndElement();  // variable
                }
                stream.writeEndElement();  // variables
                stream.writeEndElement();  // dataType
            }
        }
        stream.writeEndElement();  // dataTypes
    }
    stream.writeEndElement();  // environment
    stream.writeStartElement("agents");
    for (int i = 0; i < childItems.size(); i++) {
        if (childItems[i]->type == 1) {
            stream.writeStartElement("xagent");
            stream.writeTextElement("name", childItems[i]->name);
            stream.writeTextElement("description", childItems[i]->description);
            stream.writeStartElement("memory");
            for (int j = 0; j < childItems[i]->memoryModel->variables.size();
                    j++) {
                stream.writeStartElement("variable");
                stream.writeTextElement("type", childItems[i]->memoryModel->
                        variables[j].type);
                stream.writeTextElement("name", childItems[i]->memoryModel->
                        variables[j].name);
                if (childItems[i]->memoryModel->variables[j].constant)
                    stream.writeTextElement("constant", "true");
                stream.writeTextElement("description", childItems[i]->
                        memoryModel->variables[j].description);
                stream.writeEndElement();  // variable
            }
            stream.writeEndElement();  // memory
            stream.writeStartElement("functions");
            for (int j = 0; j < childItems[i]->machineModel->
            transitions.size(); j++) {
                stream.writeStartElement("function");
                stream.writeTextElement("name", childItems[i]->machineModel->
                        transitions[j]->name());
                stream.writeTextElement("description", childItems[i]->
                        machineModel->transitions[j]->description());
                stream.writeTextElement("currentState", childItems[i]->
                        machineModel->transitions[j]->currentState()->name());
                stream.writeTextElement("nextState", childItems[i]->
                        machineModel->transitions[j]->nextState()->name());
                // condition
                if (childItems[i]->machineModel->transitions[j]->
                        condition().enabled) {
                    stream.writeStartElement("condition");
                    childItems[i]->machineModel->transitions[j]->
                    condition().toXML(&stream);
                    stream.writeEndElement();  // condition
                }
                if (childItems[i]->machineModel->transitions[j]->
                        input().messageModel->messages.size() > 0) {
                    stream.writeStartElement("inputs");
                    for (int k = 0; k < childItems[i]->machineModel->
                    transitions[j]->input().messageModel->messages.size();
                    k++) {
                        MessageComm message = childItems[i]->machineModel->
                                transitions[j]->input().messageModel->
                                messages[k];
                        stream.writeStartElement("input");
                        stream.writeTextElement("messageName",
                                message.messageType);
                        // random
                        if (message.sort.isRandom) {
                            stream.writeTextElement("random", "true");
                        }
                        // filter
                        if (message.filter.enabled) {
                            stream.writeStartElement("filter");
                            message.filter.toXML(&stream);
                            stream.writeEndElement();  // filter
                        }
                        // sort
                        if (message.sort.isSort) {
                            stream.writeStartElement("sort");
                            stream.writeTextElement("key", message.sort.key);
                            stream.writeTextElement("order",
                                    message.sort.order);
                            stream.writeEndElement();  // sort
                        }
                        stream.writeEndElement();  // input
                    }
                    stream.writeEndElement();  // inputs
                }
                if (childItems[i]->machineModel->transitions[j]->
                        output().messageModel->messages.size() > 0) {
                    stream.writeStartElement("outputs");
                    for (int k = 0;
                            k < childItems[i]->machineModel->transitions[j]->
                            output().messageModel->messages.size(); k++) {
                        MessageComm message =
                            childItems[i]->machineModel->transitions[j]->
                            output().messageModel->messages[k];
                        stream.writeStartElement("output");
                        stream.writeTextElement("messageName",
                                message.messageType);
                        stream.writeEndElement();  // output
                    }
                    stream.writeEndElement();  // outputs
                }
                stream.writeEndElement();  // function
            }
            stream.writeEndElement();  // functions
            stream.writeEndElement();  // xagent
        }
    }
    stream.writeEndElement();  // agents
    stream.writeStartElement("messages");
    for (int i = 0; i < childItems.size(); i++) {
        if (childItems[i]->type == 2) {
            stream.writeStartElement("message");
            stream.writeTextElement("name", childItems[i]->name);
            stream.writeTextElement("description", childItems[i]->description);
            stream.writeStartElement("variables");
            for (int j = 0; j < childItems[i]->memoryModel->variables.size();
                    j++) {
                stream.writeStartElement("variable");
                stream.writeTextElement("type",
                        childItems[i]->memoryModel->variables[j].type);
                stream.writeTextElement("name",
                        childItems[i]->memoryModel->variables[j].name);
                stream.writeTextElement("description",
                        childItems[i]->memoryModel->variables[j].description);
                stream.writeEndElement();  // variable
            }
            stream.writeEndElement();  // variables
            stream.writeEndElement();  // message
        }
    }
    stream.writeEndElement();  // messages

/*    stream.writeStartElement("memory");
    for(int i = 0; i < this->memoryModel->rowCount(); i++)
    {
        stream.writeStartElement("variable");
        stream.writeTextElement("type", memoryModel->variables[i].type);// memoryModel->getTypes().at(i));
        stream.writeTextElement("name", memoryModel->variables[i].name);//getNames().at(i));
        if(memoryModel->variables[i].type == "int")
                stream.writeTextElement("value", QString::number(memoryModel->variables[i].ivalue));//getintValues().at(i)));
        if(memoryModel->variables[i].type == "double")
                stream.writeTextElement("value", QString::number(memoryModel->variables[i].dvalue));//getdoubleValues().at(i)));
        stream.writeEndElement(); // variable
    }
    stream.writeEndElement(); // memory

    stream.writeStartElement("transitions");
    for(int i = 0; i < this->machineModel->getTransitions().size(); i++)
    {
        Transition * t = machineModel->getTransitions().at(i);

        stream.writeStartElement("transition");
        stream.writeTextElement("currentState", t->currentState()->name());
        stream.writeTextElement("input", "");

        Mpre mpre = t->mpre();
        stream.writeStartElement("mpre");
        if(mpre.enabled())
        {
            stream.writeTextElement("enabled", "true");
            if(mpre.isNot()) stream.writeTextElement("not", "true");
            else stream.writeTextElement("not", "false");
            stream.writeTextElement("name", mpre.name());
            stream.writeTextElement("op", mpre.op());
            stream.writeTextElement("value", QString::number( mpre.value() ));
        }
        else
        {
            stream.writeTextElement("enabled", "false");
            stream.writeTextElement("not", "false");
            stream.writeTextElement("name", "");
            stream.writeTextElement("op", "");
            stream.writeTextElement("value", 0);
        }
        stream.writeEndElement(); // mpre

        stream.writeTextElement("name", t->name());

        Mpost mpost = t->mpost();
        stream.writeStartElement("mpost");
        if(mpost.enabled())
        {
            stream.writeTextElement("enabled", "true");
            stream.writeTextElement("name", mpost.name());
            stream.writeTextElement("op", mpost.op());
            //stream.writeTextElement("value", QString::number( mpost.value() ));
            stream.writeTextElement("name", mpost.name2());
        }
        else
        {
            stream.writeTextElement("enabled", "false");
            stream.writeTextElement("name", "");
            stream.writeTextElement("op", "");
            stream.writeTextElement("value", 0);
        }
        stream.writeEndElement(); // mpost

        stream.writeTextElement("output", "");
        stream.writeTextElement("nextState", t->nextState()->name());
        stream.writeEndElement(); // transition
    }
    stream.writeEndElement(); // transitions
*/
    stream.writeEndElement();  // xmodel

    stream.writeEndDocument();
    file->close();
    if (file->error()) {
        return false;
    }

    location = file;
    // this->setText(1, location->fileName());

    return true;
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
