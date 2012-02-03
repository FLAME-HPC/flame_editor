/*!
 * \file memorymodel.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for the memory table
*/
#ifndef MEMORYMODEL_H_
#define MEMORYMODEL_H_

#include <QtGui>
#include <QAbstractTableModel>
#include <QStringList>
#include "./variable.h"

class MemoryModel : public QAbstractTableModel {
    Q_OBJECT

  public:
    explicit MemoryModel(QObject *parent = 0)
            : QAbstractTableModel(parent) { myIsModel = false; }

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
    QList<Variable> getVariables() { return variables; }
    QStringList getNames();
    void replaceValue(int index, double value);
    void replaceValue(QString type, QString value);
    void addVariable(QString type, QString name,
        QString desc, bool constant = false, double i = 0.0);
    void setIsModel(bool b) { myIsModel = b; }
    bool isModel() { return myIsModel; }

    QList<Variable> variables;

  private:
    bool myIsModel;  /*!< Flag to show if the memory is for model descriptors */
};

#endif  // MEMORYMODEL_H_
