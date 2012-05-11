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

class MachineScene : public QGraphicsScene {
    Q_OBJECT

  public:
    MachineScene(QObject *parent = 0);

  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

  private:
    /* Used to help add items to the graphics scene */
    void addState(GraphicsItem * s);
    void addTransition(GraphicsItem * s);
    void addDiamond(GraphicsItem * s);
    void addArrow(Arrow * t);

    QObject *codeDialog;

    /* Used to draw line when moving mouse */
    QGraphicsLineItem *line;
    GraphicsItem *itemToMove;
    GraphicsItem *itemToColaps;

    GraphicsItem *itemSelect;
    /* Used to increment numbers for names */
    int num_states;
    int num_transitions;
};

#endif  // MACHINESCENE_H_
