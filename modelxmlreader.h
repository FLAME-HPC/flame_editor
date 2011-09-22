#ifndef MODELXMLREADER_H
#define MODELXMLREADER_H

#include <QXmlStreamReader>
#include "machinemodel.h"
#include "memorymodel.h"
#include "machinescene.h"
//#include "arrow.h"
#include "mpre.h"
#include "mpost.h"
#include "machine.h"

class modelXMLReader : public QXmlStreamReader
{
public:
    modelXMLReader(Machine * m);

    bool read(QIODevice * device);

private:
    void readUnknownElement();
    void readModel();
    void readVariables();
    void readTransitions();
    void readVariable();
    void readTransition();
    void readMpre();
    void readMpost();

    Machine * machine;
    MachineModel * machineModel;
    MemoryModel * memoryModel;
    Mpre mpre;
    Mpost mpost;
};

#endif // MODELXMLREADER_H
