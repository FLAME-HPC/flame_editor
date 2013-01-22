/*!
 * \file machinescene.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for the graphics scene for stategraphs
*/
#ifndef CEMACHINESCENE_H_
#define CEMACHINESCENE_H_

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QAction>
#include "./cegraphicsitem.h"
#include "./cearrow.h"
#include "./cefiletype.h"
#include "./cecfile.h"
#include "./cexmlfile.h"
#include "./cegraphicsarcitem.h"
#include "./cegraphicsgrup.h"
#include "./cevariabledeclared.h"

class CEMachineScene : public QGraphicsScene {
    Q_OBJECT

  public:
    CEMachineScene(QObject *parent = 0);

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

    CEItemType itemType() const { return itemSelect->mytype;}

    void changeCondition(CEItemType item) { itemSelect->changeCondition(item);}

    bool check();
    bool readFile(QString fileName, QList<CEVariableDeclared> &variables);
    bool saveFile(QString fileName, QList<CEVariableDeclared> *variables, QString sFunctionName, int type);
    void arrangeGraphicsItem();

    bool isChanged() const { return changed;}

  protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);

    void keyReleaseEvent(QKeyEvent *event);

  private:
    /* Used to help add items to the graphics scene */
    void addState(CEGraphicsItem * s);
    void addTransitionItem(CEGraphicsItem * s);
    void addCondition(CEGraphicsItem * s);
    void addArrow(CEArrow * t);

    void newState(CEGraphicsItem *state);
    void newTransition(CEGraphicsItem *state);
    void addTransition(CEGraphicsItem *item);
    void addIF(CEGraphicsItem *state, bool place);
    void addWHILEorFOR(CEGraphicsItem *state);
    bool testIF(CEGraphicsItem *stateStart, CEGraphicsItem *stateStop);
    bool testWHILE(CEGraphicsItem *stateStart);

    QAction *editAction;
    QAction *splitAction;

    CEGraphicsItem *check(CEGraphicsItem *g);
    CEGraphicsItem *saveFile(CEGraphicsItem *g, CEFileType *x);

    CEGraphicsItem *arrangeGraphicsItem(CEGraphicsGrup &item, CEGraphicsItem *g);

    QMap<CEGraphicsItem *, int> *map;

    QObject *codeDialog;

    /* Used to draw line when moving mouse */
    QGraphicsLineItem *line;
    CEGraphicsArcItem *arc;
    CEGraphicsItem *itemToMove;
    CEGraphicsItem *itemToColaps;
    CEGraphicsItem *itemStart;
    CEGraphicsItem *itemArc;

    CEGraphicsItem *itemSelect;
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

signals:
    void functionCodeDialog(CEGraphicsItem*);
};


#endif  // MACHINESCENE_H_
