/*!
 * \file simulationthread.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of simulation thread
 */
#include <QtGui>
#include "./simulationthread.h"

SimulationThread::SimulationThread(QObject *parent)
     : QThread(parent) {
    machine = 0;
}

SimulationThread::~SimulationThread() {
    mutex.lock();
    m_abort = -1;
    mutex.unlock();

    wait();
}

void SimulationThread::startSim() {
    m_abort = 1;
    /*if(machine->machineModel->getStartState() != 0)
    {
        item = machine->getStartState();
        item->setSelected(true);

    }
    else item = 0;*/

    currentState = 0;  // machine->machineModel->getStartState();
    currentTransition = 0;

    start();
}

void SimulationThread::stopSim() {
    mutex.lock();
    m_abort = -1;
    mutex.unlock();
}

void SimulationThread::pauseSim() {
    mutex.lock();
    if (m_abort == 1) m_abort = 0;
    else if (m_abort == 0) m_abort = 1;
    mutex.unlock();
}

void SimulationThread::run() {
    // qDebug() << "SimulationThread machine: " << machine->text(0);
    // int rc;

    while (currentState != 0 || currentTransition != 0) {
        if (m_abort == -1) return;
        while (m_abort == 0) msleep(10);
        msleep(100);

        if (currentState != 0) {
            // qDebug() << "current State: " << currentState->name();
            emit(selectState(currentState->name()));

            for (int i = 0; i < machine->machineModel->getTransitions().size();
                    i++) {
                if (machine->machineModel->
                        getTransitions().at(i)->
                        currentState() == currentState &&
                   machine->machineModel->
                   getTransitions().at(i)->
                   passesCondition(machine->memoryModel))
                    currentTransition =
                            machine->machineModel->getTransitions().at(i);
            }

            currentState = 0;
        } else {
            // qDebug() << "current Transition: " << currentTransition->name();
            emit(this->selectTransition(currentTransition->name()));

            currentTransition->updateMemory(machine->memoryModel);
            currentState = currentTransition->nextState();

            currentTransition = 0;
        }
    }

    /*QList<Arrow *> transitions = machine->getTransitions();
    QGraphicsItem * newItem;

    while(item != 0)
    {
        newItem = 0;

        for(int i = 0; i < transitions.size() && newItem == 0; i++)
        {
            if (m_abort == -1) return;
            while(m_abort == 0) msleep(10);
            msleep(100);

            Arrow * t = transitions.at(i);
            if(t->startItem() == item && t->passesCondition(memory))
            {
                //qDebug() << "transition: " << t->getName();
                newItem = t;
                emit( selectTransition(t) );
                t->updateMemory(memory);
            }
            else if(t == item)
            {
                //qDebug() << "state: " << t->endItem()->getName();
                newItem = t->endItem();
            }
        }

        if(newItem == 0) return;

        item->setSelected(false);
        item = newItem;
        item->setSelected(true);
        emit( updateScene() );
    }*/

    /*for(int i = 0; i < 1000; i ++)
    {
        qDebug() << "run() " << i;
        if (m_abort == -1) return;
        while(m_abort == 0) msleep(10);
        msleep(10);
    }*/
}
