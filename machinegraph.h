#ifndef MACHINEGRAPH_H
#define MACHINEGRAPH_H

#include "machinemodel.h"

class MachineGraph
{
public:
    MachineGraph(MachineModel * m) { machine = m; }
    ~MachineGraph();
    State * getStartState();
private:
    MachineModel * machine;
};

#endif // MACHINEGRAPH_H
