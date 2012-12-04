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
#include "transition.h"

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
    void addTransitionTransition(Machine * m, Transition * t);
    void setGraphicsView(QGraphicsView * g) { myGraphicsView = g; }
    void clearAll();

  signals:
    void functionSelected(Transition *);
    void machineSelected(Machine *);
    void functionDoubleClicked(Transition *);

  public slots:
    void updateStateName(State * s);
    void updateTransitionName(Transition * t);
    void updateInput(Transition * t);
    void updateOutput(Transition * t);
    void deleteSelectedFunction();
    void zoomIn();
    void zoomOut();
    void machineSelectedSlot(Machine *);
    void edit();
    void move();

  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

  private:
    GraphicsItem * addTransitionString(Machine *m, Transition * t);
    void addMessageCommunication(GraphicsItem *t,
        MessageComm * m, bool isInput);
    void removeTransitionFunction(GraphicsItem * t);
    void removeState(GraphicsItem * s);
    void addState(GraphicsItem * s);
    void addTransition(GraphicsItem * s);
    void addArrow(Arrow * t);
    qreal scaleFactor;
    Mode myMode;
    Arrow *line;
    int num_states;
    int num_transitions;
    int num_messages;
    MachineModel * machineModel;
    int mytype;
    Machine * rootMachine;
    Machine * selectedMachine;
    QStringList agentNames;
    bool zoomOn;
    QGraphicsView * myGraphicsView;
    QList<GraphicsItem *> states_;
    QList<GraphicsItem *> transitions_;
    QList<GraphicsItem *> statesAndTransitions;
    QList<GraphicsItem *> messages;
    GraphicsItem * selectedFunction;
    GraphicsItem * selectedState;
    GraphicsItem * highlightedState_;
    bool edit_;
    bool move_;
};

#endif  // MACHINESCENE_H_
