/*!
 * \file modelxmlreader.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for the XML model reader
*/
#ifndef MODELXMLREADER_H_
#define MODELXMLREADER_H_

#include <QXmlStreamReader>
#include "./machinemodel.h"
#include "./memorymodel.h"
#include "./machinescene.h"
#include "./mpre.h"
#include "./mpost.h"
#include "./machine.h"
#include "./condition.h"
#include "./messagecomm.h"

class modelXMLReader : public QXmlStreamReader {
  public:
    explicit modelXMLReader(Machine * m);

    bool read(QFile * file);

  private:
    void readUnknownElement();
    void readModel();
    void readAgents();
    void readAgent();
    void readMessages();
    void readMessage();
    void readVariables(Machine * a);
    void readVariable(Machine * a);
    void readTransitions(Machine * a);
    void readTransition(Machine * a);
    void readCondition(Condition * c, QString * s);
    void readMpost();
    void readIncludedModels();
    void readIncludedModel();
    void readEnvironment();
    void readDataTypes(Machine * a);
    void readDataType(Machine * a);
    void readTimeUnits(Machine * a);
    void readTimeUnit(Machine * a);
    void readInputs(Communication * input);
    void readInput(Communication * input);
    void readOutputs(Communication * output);
    void readOutput(Communication * output);
    void readSort(MessageComm * input);
    void readFunctionFiles(Machine * a);

    Machine * machine;
    Mpre mpre;
    Mpost mpost;
    QFile * file;
};

#endif  // MODELXMLREADER_H_
