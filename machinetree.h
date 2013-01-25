/*!
 * \file machinetree.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for the model tree
*/
#ifndef MACHINETREE_H_
#define MACHINETREE_H_

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "./machine.h"

class MachineTree : public QAbstractItemModel {
    Q_OBJECT

  public:
    explicit MachineTree(QObject *parent = 0);
    ~MachineTree();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                      int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                       const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    Machine* addMachine();
    Machine* insertMachine(Machine * m, int type);
    void removeMachine(Machine * m);
    const QModelIndex getIndex(Machine * m);

  signals:
    void messageUpdatedSignal(Machine * m);

  private:
    Machine *rootItem;
};

#endif  // MACHINETREE_H_
