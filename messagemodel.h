/*!
 * \file messagemodel.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for message table model
*/
#ifndef MESSAGEMODEL_H_
#define MESSAGEMODEL_H_

#include <QtGui>
#include <QAbstractTableModel>
#include <QStringList>
#include "./messagecomm.h"

class MessageModel : public QAbstractTableModel {
    Q_OBJECT

  public:
    MessageModel(int f, QObject *parent = 0)
        : QAbstractTableModel(parent) { flag = f; }

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
        QList<MessageComm> getMessages() { return messages; }
        void addMessage(MessageComm mc);
        void addMessage(QString n);

        QList<MessageComm> messages;
        int flag;
};

#endif  // MESSAGEMODEL_H_
