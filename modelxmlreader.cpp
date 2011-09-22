#include <QtGui>
#include "modelxmlreader.h"

modelXMLReader::modelXMLReader(Machine * m)
{
    machine = m;
    machineModel = m->machineModel;
    memoryModel = m->memoryModel;
}

bool modelXMLReader::read(QIODevice * device)
{
    setDevice(device);

    while (!atEnd())
    {
         readNext();

         if (isStartElement()) {
             if (name() == "xmachinemodel" && attributes().value("version") == "0.1")
                 readModel();
             else
                 raiseError(QObject::tr("The file is not an xmachinemodel version 0.1 file."));
         }
     }

     return !error();
}

void modelXMLReader::readUnknownElement()
{
    Q_ASSERT(isStartElement());

     while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement())
             readUnknownElement();
     }

}

void modelXMLReader::readModel()
{
    Q_ASSERT(isStartElement() && name() == "xmachinemodel");

     while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "name")
                machine->setText(0, readElementText());
             //else if (name() == "startState")
             //   machineModel->setStartStateString(readElementText());
             else if (name() == "memory")
                 readVariables();
             else if (name() == "transitions")
             {
                 readTransitions();
                 machine->machineScene->runLayoutEngine();
             }
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readVariables()
{
    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "variable")
                 readVariable();
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readVariable()
{
    //qDebug() << "readVariable";
    QString myname = "";
    QString mytype = "";
    double myvalue;

    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "type")
                mytype = readElementText();
             else if (name() == "name")
                 myname = readElementText();
             else if (name() == "value")
             {
                 if(mytype == "int") myvalue = readElementText().toInt();
                 if(mytype == "double") myvalue = readElementText().toDouble();
             }
             else
                 readUnknownElement();
         }
     }

    memoryModel->addVariable(mytype, myname, myvalue);
}

void modelXMLReader::readTransitions()
{
    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "transition")
                 readTransition();
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readTransition()
{
    QString cState = "";
    QString nState = "";
    QString myname = "";
    QString myinput = "";
    QString myoutput = "";

    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "currentState")
                 cState = readElementText();
             else if (name() == "input")
                 myinput = readElementText();
             else if (name() == "mpre")
                 readMpre();
             else if (name() == "name")
                 myname = readElementText();
             else if (name() == "mpost")
                 readMpost();
             else if (name() == "output")
                 myoutput = readElementText();
             else if (name() == "nextState")
                 nState = readElementText();
             else
                 readUnknownElement();
         }
     }

    machineModel->addTransitionString(myname, cState, nState, mpre, mpost);
    machine->machineScene->addTransitionString(cState, myname, nState);
}

void modelXMLReader::readMpre()
{
    QString eText;

    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement())
         {
             if (name() == "enabled")
             {
                 eText = readElementText();
                 if(eText == "true") mpre.setEnabled(true);
                 if(eText == "false") mpre.setEnabled(false);
             }
             else if (name() == "not")
             {
                 eText = readElementText();
                 if(eText == "true") mpre.setNot(true);
                 if(eText == "false") mpre.setNot(false);
             }
             else if (name() == "name")
                 mpre.setName(readElementText());
             else if (name() == "op")
                 mpre.setOp(readElementText());
             else if (name() == "value")
                 mpre.setValue(readElementText().toInt());
             else
                 readUnknownElement();
         }
     }
}

void modelXMLReader::readMpost()
{
    QString eText;

    while (!atEnd())
    {
         readNext();

         if (isEndElement())
             break;

         if (isStartElement()) {
             if (name() == "enabled")
             {
                 eText = readElementText();
                 if(eText == "true") mpost.setEnabled(true);
                 if(eText == "false") mpost.setEnabled(false);
             }
             else if (name() == "name")
                 mpost.setName(readElementText());
             else if (name() == "op")
                 mpost.setOp(readElementText());
             else if (name() == "name")
                 mpost.setName2(readElementText());
                 //mpost.setValue(readElementText().toInt());
             else
                 readUnknownElement();
         }
     }
}
