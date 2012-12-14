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
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QAction>
#include "./graphicsitem.h"
#include "./arrow.h"
#include "./filetype.h"
#include "./cfile.h"
#include "./xmlfile.h"
#include "./graphicsarcitem.h"
#include "./graphicsgrup.h"
#include "./variabledeclared.h"

class MachineScene : public QGraphicsScene {
    Q_OBJECT

  public:
    MachineScene(QObject *parent = 0);

    void setSelectItemText(QString s){
        if(itemSelect != 0){
            itemSelect->setName(s);
            invalidate();
        }
    }

    QStringList *getAssignmentList(){
        if(itemSelect != 0 && (itemSelect->mytype == Transition || itemSelect->mytype == ConditionIf ||
                               itemSelect->mytype == ConditionWhile || itemSelect->mytype == ConditionFor)){
            return itemSelect->getAssignmentList();
        }
        return 0;
    }

    void setName(QString s) {
        if(namePosition > -1)
            nameList[namePosition] = s;
        namePosition = -1;
    }
    bool isNameInList(QString s)
    {
        int i = nameList.indexOf(s);
        if(i == -1 || i == namePosition)
            return false;
        else
            return true;
    }

    ItemType itemType() const { return itemSelect->mytype;}

    void changeCondition(ItemType item) { itemSelect->changeCondition(item);}

    bool check();
    bool readFile(QString fileName, QList<VariableDeclared> &variables);
    bool saveFile(QString fileName, QList<VariableDeclared> *variables, QString sFunctionName, int type);
    void arrangeGraphicsItem();

    bool isChanged() const { return changed;}

  protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    //void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    void keyReleaseEvent(QKeyEvent *event);

  private:
    /* Used to help add items to the graphics scene */
    void addState(GraphicsItem * s);
    void addTransitionItem(GraphicsItem * s);
    void addCondition(GraphicsItem * s);
    void addArrow(Arrow * t);

    void newState(GraphicsItem *state);
    void newTransition(GraphicsItem *state);
    void addTransition(GraphicsItem *item);
    void addIF(GraphicsItem *state, bool place);
    void addWHILEorFOR(GraphicsItem *state);
    bool testIF(GraphicsItem *stateStart, GraphicsItem *stateStop);
    bool testWHILE(GraphicsItem *stateStart);

    QAction *editAction;
    QAction *splitAction;

    GraphicsItem *check(GraphicsItem *g);
    GraphicsItem *saveFile(GraphicsItem *g, FileType *x);

    GraphicsItem *arrangeGraphicsItem(GraphicsGrup &item, GraphicsItem *g);

    QMap<GraphicsItem *, int> *map;

    QObject *codeDialog;

    /* Used to draw line when moving mouse */
    QGraphicsLineItem *line;
    GraphicsArcItem *arc;
    GraphicsItem *itemToMove;
    GraphicsItem *itemToColaps;
    GraphicsItem *itemStart;
    GraphicsItem *itemArc;

    GraphicsItem *itemSelect;
    /* Used to increment numbers for names */
    int num_states;
    int num_transitions;
    int num_conditions;

    QList<QString> nameList;
    int namePosition;

    bool moveMouse;

    bool changed;

private slots:
    void edit();
    void split();
};


#endif  // MACHINESCENE_H_
