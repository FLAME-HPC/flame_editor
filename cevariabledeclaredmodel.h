#ifndef CEVARIABLEDECLAREDMODEL_H
#define CEVARIABLEDECLAREDMODEL_H

#include <QtGui>
#include <QAbstractTableModel>
#include "./cevariabledeclared.h"

class CEVariableDeclaredModel  : public QAbstractTableModel
{
    Q_OBJECT

public:
    CEVariableDeclaredModel(QObject *parent = 0)
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
    QList<CEVariableDeclared> getVariables() { return variables; }
    QStringList getNames(); // { return stringListName; }

    void replaceValue(int index, double value);
    void addVariable();

    QList<CEVariableDeclared> getVariables() const { return variables;}
    void setVariables(QList<CEVariableDeclared> *list);

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
    QList<CEVariableDeclared> variables;
    QStringList *memoryVariableNames;
};

#endif // VARIABLEDECLAREDMODEL_H
