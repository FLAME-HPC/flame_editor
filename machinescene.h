/*!
 * \file machinescene.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for the graphics scene for stategraphs
*/
#ifndef MACHINESCENE_H_
#define MACHINESCENE_H_

#include <QGraphicsScene>
#include "./graphicsitem.h"
#include "./arrow.h"
#include "./machinemodel.h"

class Machine;

class MachineScene : public QGraphicsScene {
    Q_OBJECT

  public:
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

    MachineScene(Machine * m, QObject *parent = 0);

    void calcLayers();
    void setMachineModel(MachineModel * m) { machineModel = m; }
    void selectTransition(QString n);
    void selectState(QString n);
    void addTransitionTransition(QString agentName,
        Transition * t, int isForeign = 0);
    void setGraphicsView(QGraphicsView * g) { myGraphicsView = g; }


  signals:
    void functionSelected(bool b);

  public slots:
    void updateStateName(State * s);
    void updateTransitionName(Transition * t);
    void updateInput(Transition * t);
    void updateOutput(Transition * t);
    void deleteSelectedFunction();

  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

  private:
    GraphicsItem * addTransitionString(QString agentName,
        Transition * t, int flag);
    void addMessageCommunication(GraphicsItem *t,
        MessageComm * m, bool isInput);
    void removeTransitionFunction(GraphicsItem * t);
    void zoomIn();
    void zoomOut();
    void addState(GraphicsItem * s);
    void addTransition(GraphicsItem * s);
    void addArrow(Arrow * t);
    qreal scaleFactor;
    Mode myMode;
    QGraphicsLineItem *line;
    int num_states;
    int num_transitions;
    int num_messages;
    MachineModel * machineModel;
    int mytype;
    Machine * rootMachine;
    QStringList agentNames;
    bool zoomOn;
    QGraphicsView * myGraphicsView;
    QList<GraphicsItem *> statesAndTransitions;
    QList<GraphicsItem *> messages;
    GraphicsItem * selectedFunction;
};

#endif  // MACHINESCENE_H_
