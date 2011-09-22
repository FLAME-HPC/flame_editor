#ifndef SIMULATIONTHREAD_H
#define SIMULATIONTHREAD_H

#include <QThread>
#include <QMutex>
#include <QThread>
#include "machinemodel.h"
#include "memorymodel.h"
#include "arrow.h"
#include "machine.h"
#include "state.h"
#include "transition.h"

class SimulationThread : public QThread
{
    Q_OBJECT

public:
    SimulationThread(QObject *parent = 0);
    ~SimulationThread();
    void setMachine(Machine * m) { machine = m; }

signals:
    void updateScene();
    void selectTransition(QString n);
    void selectState(QString n);

public slots:
    void stopSim();
    void pauseSim();
    void startSim();

protected:
    void run();

private:
    int m_abort;
    QMutex mutex;
    Machine * machine;
    QGraphicsItem * item;
    State * currentState;
    Transition * currentTransition;
};

#endif // SIMULATIONTHREAD_H
