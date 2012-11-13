/*!
 * \file machinescene.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for the graphics scene for stategraphs
*/
#ifndef FEMACHINESCENE_H_
#define FEMACHINESCENE_H_

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QAction>
#include "./fegraphicsitem.h"
#include "./fearrow.h"
#include "./filetype.h"
#include "./cfile.h"
#include "./xmlfile.h"
#include "./graphicsarcitem.h"
#include "./graphicsgrup.h"

class FEMachineScene : public QGraphicsScene {
    Q_OBJECT

  public:
    FEMachineScene(QObject *parent = 0);

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
    bool readFile(QString fileName);
    bool saveFile(QString fileName, QString sFunctionName, int type);
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
    void addState(FEGraphicsItem * s);
    void addTransitionItem(FEGraphicsItem * s);
    void addCondition(FEGraphicsItem * s);
    void addArrow(FEArrow * t);

    void newState(FEGraphicsItem *state);
    void newTransition(FEGraphicsItem *state);
    void addTransition(FEGraphicsItem *item);
    void addIF(FEGraphicsItem *state, bool place);
    void addWHILEorFOR(FEGraphicsItem *state);
    bool testIF(FEGraphicsItem *stateStart, FEGraphicsItem *stateStop);
    bool testWHILE(FEGraphicsItem *stateStart);

    QAction *editAction;
    QAction *splitAction;

    FEGraphicsItem *check(FEGraphicsItem *g);
    FEGraphicsItem *saveFile(FEGraphicsItem *g, FileType *x);

    FEGraphicsItem *arrangeGraphicsItem(GraphicsGrup &item, FEGraphicsItem *g);

    QMap<FEGraphicsItem *, int> *map;

    QObject *codeDialog;

    /* Used to draw line when moving mouse */
    QGraphicsLineItem *line;
    GraphicsArcItem *arc;
    FEGraphicsItem *itemToMove;
    FEGraphicsItem *itemToColaps;
    FEGraphicsItem *itemStart;
    FEGraphicsItem *itemArc;

    FEGraphicsItem *itemSelect;
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


#endif  // FEMACHINESCENE_H_
