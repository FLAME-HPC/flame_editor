/*!
 * \file modelxmlwriter.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of the XML model writer
 */
#include "./machine.h"

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
