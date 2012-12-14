#ifndef VARIABLEDECLAREDMODEL_H
#define VARIABLEDECLAREDMODEL_H

#include <QtGui>
#include <QAbstractTableModel>
#include "./variabledeclared.h"

class VariableDeclaredModel  : public QAbstractTableModel
{
    Q_OBJECT

public:
    VariableDeclaredModel(QObject *parent = 0)
            : QAbstractTableModel(parent)
    {
        memoryVariableNames = 0;
    } //count = 0; }

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
    QList<VariableDeclared> getVariables() { return variables; }
    QStringList getNames(); // { return stringListName; }

    void replaceValue(int index, double value);
    void addVariable();

    QList<VariableDeclared> getVariables() const { return variables;}
    void setVariables(QList<VariableDeclared> *list);

    void setMemoryVariableNames(QStringList s)
    {
        if(memoryVariableNames != 0)
        {
            memoryVariableNames->clear();
            memoryVariableNames = 0;
        }
        memoryVariableNames = new QStringList(s);
    }


private:
    QList<VariableDeclared> variables;
    QStringList *memoryVariableNames;
};

#endif // VARIABLEDECLAREDMODEL_H
