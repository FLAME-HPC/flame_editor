/*!
 * \file modelxmlreader.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of the XML model reader
 */
#include <QtGui>
#include "./modelxmlreader.h"
#include "./timeunit.h"

modelXMLReader::modelXMLReader(Machine * m) {
    machine = m;
}

bool modelXMLReader::read(QFile *f) {  // (QIODevice * device)
    file = f;
    setDevice(file);

    QFileInfo fileInfo(file->fileName());
    machine->fileDirectory = fileInfo.absolutePath();
    machine->fileName = fileInfo.fileName();

    while (!atEnd()) {
         readNext();

         if (isStartElement()) {
             if (name() == "xmodel" && attributes().value("version") == "2")
                 readModel();
             else
                 raiseError(QObject::tr(
                         "The file is not an xmodel version 2 file."));
         }
     }

     return !error();
}

void modelXMLReader::readUnknownElement() {
    Q_ASSERT(isStartElement());

    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement())
             readUnknownElement();
     }
}

void modelXMLReader::readModel() {
    Q_ASSERT(isStartElement() && name() == "xmodel");

    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "name")
                machine->name = readElementText();
             else if (name() == "version")
                machine->memoryModel->replaceValue("version",
                        readElementText());
             else if (name() == "author")
                machine->memoryModel->replaceValue("author", readElementText());
             else if (name() == "description")
                machine->memoryModel->replaceValue("description",
                        readElementText());
             else if (name() == "models")
                readIncludedModels();
             else if (name() == "environment")
                readEnvironment();
             else if (name() == "agents")
                readAgents();
             else if (name() == "messages")
                readMessages();
             else
                readUnknownElement();
         }
     }
}

void modelXMLReader::readIncludedModels() {
    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "model")
                    readIncludedModel();
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readIncludedModel() {
    bool enabled = false;
    QString fileName = "";

    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "file")
                    fileName = readElementText();
             else
                 if (name() == "enabled") {
                 QString v = readElementText();
                 if (v == "true") enabled = true;
                 if (v == "false") enabled = false;
             } else {
                 readUnknownElement();
             }
         }
     }

    // Create the new sub model
    Machine * a = new Machine(0, machine);
    a->setRootModel(machine->rootModel());
    machine->appendChild(a);
    a->isSubModel = true;
    a->setEnabled(enabled);
    // Add the environment to the new sub model
    Machine * b = new Machine(3, a);
    a->appendChild(b);
    b->setRootModel(a->rootModel());
    b->name = "environment";

    // Get file info of the current file being read
    QFileInfo fileInfo(file->fileName());
    // Get the absolute path to the current file
    QString path = fileInfo.absolutePath();
    path.append("/");  // Append a directory slash
    path.append(fileName);  // Append the path to the sub model file

    // Create a qfile for the sub model
    QFile filet(path);
    // Try and open the qfile
    if (!filet.open(QFile::ReadOnly | QFile::Text)) {
        raiseError(
            QString(
        "Because cannot open sub model file: %1\n%2").
        arg(path).
        arg(filet.errorString()));
        return;
    }
    // Try and read the qfile
    modelXMLReader reader(a);
    if (!reader.read(&filet)) {
        raiseError(
            QString(
    "Because cannot parse sub model in file %1 at line %2, column %3:\n%4").
        arg(path).
        arg(reader.lineNumber()).
        arg(reader.columnNumber()).
        arg(reader.errorString()));
    }
}

void modelXMLReader::readAgents() {
    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "xagent")
                    readAgent();
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readAgent() {
    Machine * a = new Machine(1, machine);
    machine->appendChild(a);
    a->setRootModel(machine->rootModel());

    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "name")
                a->name = readElementText();
             else if (name() == "description")
                a->description = readElementText();
             else if (name() == "memory")
                 readVariables(a);
             else if (name() == "functions")
                 readTransitions(a);
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readMessages() {
    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "message")
                    readMessage();
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readMessage() {
    Machine * a = new Machine(2, machine);
    machine->appendChild(a);
    a->setRootModel(machine->rootModel());

    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "name")
                a->name = readElementText();
             else if (name() == "description")
                a->description = readElementText();
             else if (name() == "variables")
                 readVariables(a);
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readVariables(Machine * a) {
    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "variable")
                 readVariable(a);
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readVariable(Machine * a) {
    QString myname = "";
    QString mytype = "";
    QString description = "";
    double myvalue = 0.0;
    bool constant = false;

    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "type")
                mytype = readElementText();
             else if (name() == "name")
                 myname = readElementText();
             else if (name() == "description")
                 description = readElementText();
             else
                 if (name() == "constant") {
                 QString constantString = readElementText();
                 if (constantString == "true") constant = true;
             } else if (name() == "value") {
                 if (mytype == "int") myvalue = readElementText().toInt();
                 if (mytype == "double") myvalue = readElementText().toDouble();
             } else {
                 readUnknownElement();
             }
         }
     }

    a->memoryModel->addVariable(mytype, myname,
            description, constant, myvalue);

    // qDebug() << machine->name << a->type << a->name << myname;
}

void modelXMLReader::readTransitions(Machine * a) {
    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "function")
                 readTransition(a);
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readTransition(Machine * a) {
    QString cState = "";
    QString nState = "";
    QString myname = "";
    QString myinput = "";
    QString myoutput = "";
    QString description = "";
    Condition c = Condition();
    Communication input = Communication(1);
    Communication output = Communication(0);

    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "currentState")
                 cState = readElementText();
             else if (name() == "inputs")
                 readInputs(&input);
             else
                 if (name() == "condition") {
                 readCondition(&c, 0);
                 c.enabled = true;
             } else if (name() == "name") {
                 myname = readElementText();
             } else if (name() == "description") {
                 description = readElementText();
             } else if (name() == "outputs") {
                 readOutputs(&output);
             } else if (name() == "nextState") {
                 nState = readElementText();
             } else {
                 readUnknownElement();
             }
         }
     }

    a->addTransitionString(myname, cState, nState, c, mpost,
            input, output, description);
    // a->machineModel->addTransitionString(myname, cState,
    // nState, c, mpost, input, output);
    // a->machineScene->addTransitionString(a->name, cState,
    // myname, nState, 0);
}

void modelXMLReader::readCondition(Condition * c, QString * s) {
    Q_ASSERT(c != 0);

    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "not") {
                 c->isNot  = true;
                 readCondition(c, 0);
             } else if (name() == "time") {
                 c->isTime = true;
                 c->isValues = false;
                 c->isConditions = false;

                 while (!atEnd()) {
                      readNext();

                      if (isEndElement())
                          break;

                      if (isStartElement()) {
                            if (name() == "period")
                                c->timePeriod = readElementText();
                            else if (name() == "phase")
                                c->timePhaseVariable = readElementText();
                            else if (name() == "duration")
                                c->timeDuration = readElementText().toInt();
                            else
                                readUnknownElement();
                      }
                 }

                 c->processSymbols();
             } else if (name() == "lhs") {
                 c->lhsCondition = new Condition();
                 c->lhsCondition->enabled = true;
                 readCondition(c->lhsCondition, &c->lhs);
                 if (c->lhs != "") {
                     c->isValues = true;
                     c->isConditions = false;
                 } else {
                     c->isValues = false;
                     c->isConditions = true;
                 }
             } else if (name() == "op") {
                 c->op = readElementText();
             } else if (name() == "rhs") {
                 c->rhsCondition = new Condition();
                 c->rhsCondition->enabled = true;
                 readCondition(c->rhsCondition, &c->rhs);
                 if (c->rhs != "") {
                     c->isValues = true;
                     c->isConditions = false;
                 } else {
                     c->isValues = false;
                     c->isConditions = true;
                 }

                 c->processSymbols();
             } else if (name() == "value") {
                 Q_ASSERT(s != 0);

                 *s = readElementText();
             } else {
                 readUnknownElement();
             }
         }
     }
}

void modelXMLReader::readMpost() {
    QString eText;

    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "enabled") {
                 eText = readElementText();
                 if (eText == "true") mpost.setEnabled(true);
                 if (eText == "false") mpost.setEnabled(false);
             } else if (name() == "name") {
                 mpost.setName(readElementText());
             } else if (name() == "op") {
                 mpost.setOp(readElementText());
             } else if (name() == "name") {
                 mpost.setName2(readElementText());
                 // mpost.setValue(readElementText().toInt());
             } else {
                 readUnknownElement();
             }
         }
     }
}

void modelXMLReader::readEnvironment() {
    Machine * a = 0;
    for (int i = 0; i < machine->childCount(); i++) {
        if (machine->child(i)->type == 3) a = machine->child(i);
    }
    Q_ASSERT(a != 0);

    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "timeUnits")
                 readTimeUnits(a);
             else if (name() == "dataTypes")
                 readDataTypes(a);
             else if (name() == "constants")
                readVariables(a);
             else if (name() == "functionFiles")
                readFunctionFiles(a);
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readFunctionFiles(Machine *m) {
    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "file") {
                 Machine * a = new Machine(6, m);
                 m->appendChild(a);
                 a->setRootModel(machine->rootModel());

                 a->name = readElementText();

                 // For function files keep fileDirectory the same as the
                 // model file directory
                 // and have the fileName as the path from the model
                 // directory to the file
                 a->fileDirectory = machine->fileDirectory;
                 a->fileName = a->name;
             } else {
                 readUnknownElement();
             }
         }
     }
}

void modelXMLReader::readDataTypes(Machine * a) {
    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "dataType")
                 readDataType(a);
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readDataType(Machine * m) {
    Machine * a = new Machine(5, m);
    m->appendChild(a);
    a->setRootModel(machine->rootModel());

    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "name")
                 a->name = readElementText();
             else if (name() == "description")
                 a->description = readElementText();
             else if (name() == "variables")
                 readVariables(a);
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readTimeUnits(Machine * a) {
    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "timeUnit")
                 readTimeUnit(a);
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readTimeUnit(Machine * m) {
    Machine * a = new Machine(4, m);
    m->appendChild(a);
    a->setRootModel(machine->rootModel());

    // a->timeUnit = new TimeUnit();
    TimeUnit t;

    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "name") {
                 t.name = readElementText();
                 a->name = t.name;
                 machine->timeUnitNames.append(a->name);
             } else if (name() == "unit") {
                 t.unit = readElementText();
             } else if (name() == "period") {
                 t.period = readElementText().toInt();
             } else {
                 readUnknownElement();
             }
         }
     }

    a->setTimeUnit(t);
}

void modelXMLReader::readInputs(Communication * input) {
    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "input")
                 readInput(input);
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readSort(MessageComm * mc) {
    mc->sort.isSort = true;

    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "key")
                 mc->sort.key = readElementText();
             else
                 if (name() == "order")
                 mc->sort.order = readElementText();
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readInput(Communication * input) {
    MessageComm mc;

    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "messageName") {
                mc.messageType = readElementText();
             } else if (name() == "random") {
                 if (readElementText() == "true") mc.sort.isRandom = true;
             } else if (name() == "filter") {
                 mc.filter.enabled = true;
                 readCondition(&mc.filter, 0);
             } else if (name() == "sort") {
                 readSort(&mc);
             } else {
                 readUnknownElement();
             }
         }
     }

    input->messageModel->addMessage(mc);
}

void modelXMLReader::readOutputs(Communication * output) {
    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "output")
                 readOutput(output);
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readOutput(Communication * output) {
    while (!atEnd()) {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "messageName") {
                 output->messageModel->addMessage(readElementText());
             } else {
                 readUnknownElement();
             }
         }
     }
}
