#include "machine.h"

Machine::~Machine()
{
    //delete machineItem;
    delete machineModel;
    delete memoryModel;
}

/*void Machine::setName(QString n)
{
    //myName = n;
    //machineItem->setText(0, n);
}*/

bool Machine::writeModelXML(QFile * file)
{
    QXmlStreamWriter stream(file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("xmachinemodel");
    stream.writeAttribute("version", "0.1");
    stream.writeTextElement("name", this->text(0));
    //stream.writeTextElement("startState", this->machineModel->getStartState()->name());

    stream.writeStartElement("memory");
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

    stream.writeEndElement(); // machine

    stream.writeEndDocument();
    file->close();
    if(file->error())
    {
        return false;
    }

    location = file;
    this->setText(1, location->fileName());

    return true;
}
