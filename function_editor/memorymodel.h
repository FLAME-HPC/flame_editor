#ifndef MEMORYMODEL_H
#define MEMORYMODEL_H

#include <QtGui>
#include <QAbstractTableModel>
#include <QStringList>
#include "variable.h"

class MemoryModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    MemoryModel(QObject *parent = 0)
            : QAbstractTableModel(parent) { } //count = 0; }

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
    QList<Variable> getVariables() { return variables; }
    QStringList getNames(); // { return stringListName; }

    void replaceValue(int index, double value);
    void addVariable(QString type, QString name, double i);


private:
    QList<Variable> variables;
};

#endif // MEMORYMODEL_H
