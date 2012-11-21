/*!
 * \file machinemodel.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for model machine function transition table
*/
#ifndef MACHINEMODEL_H_
#define MACHINEMODEL_H_

#include <QAbstractTableModel>
#include "./memorymodel.h"
#include "./mpre.h"
#include "./mpost.h"
#include "./transition.h"
#include "./state.h"

class MachineModel : public QAbstractTableModel {
    Q_OBJECT

  public:
    MachineModel(MemoryModel *m, QObject *parent = 0)
         : QAbstractTableModel(parent) { memory = m; }

    Transition * addTransitionString(QString name, QString cs,
        QString ns, Condition c, Mpost mpost,
        Communication input, Communication output, QString desc);
    void setStartStateString(QString n);
    void deleteTransition(Transition * t);
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
    bool insertRows(int position, int rows,
        const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows,
        const QModelIndex &index = QModelIndex());
    QList<State *> getStates() const { return states; }
    QList<Transition *> getTransitions() const { return transitions; }
    Transition * addTransition(QString name, State * current, State * next);
    State * addState(QString name);
    void addMessageToTransition(Transition * t,
        bool isInput, QString messageType);
    QList<Transition *> transitions;
    QString getRowName(const QModelIndex &index);
    const QModelIndex getIndex(Transition * t);

  signals:
     void updateStateName(State * s);
     void updateTransitionName(Transition * t);
     void communicationChanged();
     void updateInput(Transition * t);
     void updateOutput(Transition * t);

  public slots:
     void transitionUpdated(QModelIndex topLeft, QModelIndex bottomRight);

  private:
    MemoryModel * memory;
    QList<State *> states;
};

#endif  // MACHINEMODEL_H_
