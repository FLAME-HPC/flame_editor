/*!
 * \file simulationthread.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for the simulation thread
*/
#ifndef SIMULATIONTHREAD_H_
#define SIMULATIONTHREAD_H_

#include <QThread>
#include <QMutex>
#include "./machinemodel.h"
#include "./memorymodel.h"
#include "./arrow.h"
#include "./machine.h"
#include "./state.h"
#include "./transition.h"

class SimulationThread : public QThread {
    Q_OBJECT

  public:
    explicit SimulationThread(QObject *parent = 0);
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

#endif  // SIMULATIONTHREAD_H_
