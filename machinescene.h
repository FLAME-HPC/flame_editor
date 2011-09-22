#ifndef MACHINESCENE_H
#define MACHINESCENE_H

#include <QGraphicsScene>
#include "stateitem.h"
#include "arrow.h"
#include "machinemodel.h"

class MachineScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

    MachineScene(MachineModel * m, QObject *parent = 0);
    void runLayoutEngine();
    //void setMachineOld(MachineModel * m);
    void selectTransition(QString n);
    void selectState(QString n);
    void addTransitionString(QString cs, QString n, QString ns);
    //void addStateString(QString n, bool s);

signals:
    void updateStatus(QString s);
    void addedTransition(QString cs, QString name, QString ns);
    void addedState(QString name, bool start);

public slots:
    void addState(StateItem * s);
    void addTransition(Arrow * t);
    void updateStateName(QString oldName, QString newName);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    Mode myMode;
    QGraphicsLineItem *line;
    int num_states;
    int num_transitions;
    MachineModel * machine;
};

#endif // MACHINESCENE_H
