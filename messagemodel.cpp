/*!
 * \file messagemodel.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of the message table
 */
#include "./messagemodel.h"

int MessageModel::rowCount(const QModelIndex &/*parent*/) const {
    return messages.count();
}

int MessageModel::columnCount(const QModelIndex &/*parent*/) const {
     if (flag == 1)
         return 3;
     else
         if (flag == 0) return 1;
     return 3;
}

QVariant MessageModel::data(const QModelIndex &index, int role) const {
     if (!index.isValid())
         return QVariant();

     if (index.row() >= messages.size())
         return QVariant();

     if (role == Qt::DisplayRole) {
         if (index.column() == 0)
             return messages.at(index.row()).messageType;
             // stringListType.at(index.row());
         if (index.column() == 1)
             return qVariantFromValue(messages.at(index.row()).filter);
         if (index.column() == 2)
             return qVariantFromValue(messages.at(index.row()).sort);
         return QVariant();
     } else {
         return QVariant();
     }
}

QVariant MessageModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal) {
         if (section == 0) return QString("Message Type");
         else if (section == 1) return QString("Filter");
         else if (section == 2) return QString("Sort");
         return QString("");
     } else {
         return QString("Row %1").arg(section);
     }
}

Qt::ItemFlags MessageModel::flags(const QModelIndex &index) const {
     if (!index.isValid())
         return Qt::ItemIsEnabled;

     return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool MessageModel::setData(const QModelIndex &index,
                               const QVariant &value, int role) {
     if (index.isValid() && role == Qt::EditRole) {
        if (index.column() == 0)
            messages[index.row()].messageType = value.toString();
        if (index.column() == 1)
            messages[index.row()].filter = qVariantValue<Condition>(value);
        if (index.column() == 2)
            messages[index.row()].sort = qVariantValue<MessageSort>(value);

         emit dataChanged(index, index);
         return true;
     }
     return false;
}

bool MessageModel::insertRows(int position, int rows,
        const QModelIndex &/*parent*/) {
     beginInsertRows(QModelIndex(), position, position+rows-1);

     for (int row = 0; row < rows; ++row) {
         messages.insert(position, MessageComm());
     }

     endInsertRows();
     return true;
}

bool MessageModel::removeRows(int position, int rows,
        const QModelIndex &/*parent*/) {
     beginRemoveRows(QModelIndex(), position, position+rows-1);

     for (int row = 0; row < rows; ++row) {
         messages.removeAt(position);
     }

     endRemoveRows();
     return true;
}

void MessageModel::addMessage(MessageComm mc) {
    beginInsertRows(QModelIndex(), messages.count(), messages.count());
    messages.insert(messages.count(), mc);
    endInsertRows();
}

void MessageModel::addMessage(QString n) {
    MessageComm mc;
    mc.messageType = n;
    addMessage(mc);
}
