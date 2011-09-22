#ifndef MACHINEMODEL_H
#define MACHINEMODEL_H

#include <QAbstractTableModel>
//#include "arrow.h"
//#include "stateitem.h"
#include "statetableitem.h"
#include "memorymodel.h"
#include "mpre.h"
#include "mpost.h"
#include "transition.h"
#include "state.h"

class MachineModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    MachineModel(MemoryModel *m, QObject *parent = 0)
         : QAbstractTableModel(parent) { memory = m; } //{ startState = 0; } //myMemory = new MemoryModel(); }

    //void addState(QString name, bool start);//StateItem *s);
    //void addTransition(QString cs, QString name, QString ns);//Arrow *a);
    void addTransitionString(QString name, QString cs, QString ns, Mpre mpre, Mpost mpost);
    void setStartStateString(QString n);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
     int columnCount(const QModelIndex &parent = QModelIndex()) const;
     QVariant data(const QModelIndex &index, int role) const;
     QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
     /* Functions to provide editing */
     Qt::ItemFlags flags(const QModelIndex &index) const;
     bool setData(const QModelIndex &index, const QVariant &value,
                  int role = Qt::EditRole);
     /* Adding and removing rows */
     bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
     bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
     /*StateItem* getStartState();
     QList<Arrow *> getTransitions();
     void handleUpdatedMpre(Arrow * a);
     //MemoryModel * memory() { return myMemory; }
     bool writeModelXML(QFile * file, MemoryModel * myMemory);*/
     //void setName(QString n) { myName = n; }
     //QString name() const { return myName; }
     QList<State *> getStates() const { return states; }
     QList<Transition *> getTransitions() const { return transitions; }
     State * getStartState() const { return startState; }

signals:
     /*void newState(StateItem * si);
     void newTransition(Arrow * a);*/
     void updateStateName(QString oldName, QString newName);

public slots:
     void addTransition(QString cs, QString name, QString ns);
     void addState(QString name, bool start);
     void transitionUpdated(QModelIndex topLeft, QModelIndex bottomRight);
     int checkValidStartState();

private:
    //QString myName;
    //QList<StateItem *> states;
    //QList<Arrow *> transitions;
    //StateItem * startState;
    MemoryModel * memory;
    QList<State *> states;
    QList<Transition *> transitions;
    State * startState;
};

#endif // MACHINEMODEL_H
