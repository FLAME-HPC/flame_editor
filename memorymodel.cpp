/*!
 * \file memorymodel.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of model memory table
 */
#include <QtGui>
#include "./memorymodel.h"

/*MemoryModel::MemoryModel()
{
}*/

int MemoryModel::rowCount(const QModelIndex &/*parent*/) const {
     // return stringListType.count();
    return variables.count();
}

int MemoryModel::columnCount(const QModelIndex &/*parent*/) const {
     if (myIsModel)
         return 2;
     else
         return 3;
}

QVariant MemoryModel::data(const QModelIndex &index, int role) const {
     if (!index.isValid())
         return QVariant();

     if (index.row() >= variables.size())
         return QVariant();

     if (role == Qt::DisplayRole) {
         if (myIsModel) {
             if (index.column() == 0)
                 return variables.at(index.row()).name;
             if (index.column() == 1)
                 return variables.at(index.row()).description;
         } else {
             if (index.column() == 0)
                 return variables.at(index.row()).type;
                 // stringListType.at(index.row());
             if (index.column() == 1)
                 return variables.at(index.row()).name;
                 // stringListName.at(index.row());
             if (index.column() == 2)
                 return variables.at(index.row()).description;
             /*if(index.column() == 2)
             {
                 //return variables[index.row()].getValue();
                 if(variables.at(index.row()).type == "int")
                     return variables.at(index.row()).ivalue;
                 else
                     return variables.at(index.row()).dvalue;
             }*/
         }
         return QVariant();
     } else {
         return QVariant();
     }
}

QVariant MemoryModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal) {
         if (myIsModel) {
             if (section == 0) return QString("Type");
             else if (section == 1) return QString("Value");
         } else {
             if (section == 0) return QString("Type");
             else if (section == 1) return QString("Name");
             else if (section == 2) return QString("Description");
         }
         return QString("");
     } else {
         return QString("Row %1").arg(section);
     }
}

Qt::ItemFlags MemoryModel::flags(const QModelIndex &index) const {
     if (!index.isValid())
         return Qt::ItemIsEnabled;

     if (myIsModel && index.column() == 0)
         return QAbstractItemModel::flags(index);
     else
         return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool MemoryModel::setData(const QModelIndex &index,
                               const QVariant &value, int role) {
     if (index.isValid() && role == Qt::EditRole) {
        if (isModel()) {
            if (index.column() == 0)
                variables[index.row()].name = value.toString();
            if (index.column() == 1)
                variables[index.row()].description = value.toString();
        } else {
            if (index.column() == 0)
                variables[index.row()].type = value.toString();
                // stringListType.replace(index.row(), value.toString());
            if (index.column() == 1)
                variables[index.row()].name = value.toString();
                // stringListName.replace(index.row(), value.toString());
            if (index.column() == 2)
                variables[index.row()].description = value.toString();
            /*if(index.column() == 2)
            //variables[index.row()].setValue(value.toDouble());
            {
                variables[index.row()].ivalue = value.toInt();
                variables[index.row()].dvalue = value.toDouble();
            }*/
        }

         emit dataChanged(index, index);
         return true;
     }
     return false;
}

bool MemoryModel::insertRows(int position, int rows,
        const QModelIndex &/*parent*/) {
     beginInsertRows(QModelIndex(), position, position+rows-1);

     for (int row = 0; row < rows; ++row) {
         QString s = "name_";
         s.append(QString("%1").arg(this->rowCount()));

         variables.insert(position, Variable(s, "int"));

         /*stringListType.insert(position, "int");
         stringListName.insert(position, s);
         intListValue.insert(position, 0);
         doubleListValue.insert(position, 0);*/
     }

     endInsertRows();
     return true;
}

bool MemoryModel::removeRows(int position, int rows,
        const QModelIndex &/*parent*/) {
     beginRemoveRows(QModelIndex(), position, position+rows-1);

     for (int row = 0; row < rows; ++row) {
         variables.removeAt(position);

         /*stringListType.removeAt(position);
         stringListName.removeAt(position);
         intListValue.removeAt(position);
         doubleListValue.removeAt(position);*/
     }

     endRemoveRows();
     return true;
}

/*void MemoryModel::replaceIntValue(int i, int value)
{
    intListValue.replace(i, value);

    QModelIndex myIndex = this->index(i, 2, QModelIndex());

    emit( this->dataChanged(myIndex, myIndex) );
}*/

void MemoryModel::replaceValue(int i, double value) {
    variables[i].ivalue = static_cast<int>(value);
    variables[i].dvalue = value;

    QModelIndex myIndex = this->index(i, 2, QModelIndex());
    emit(this->dataChanged(myIndex, myIndex));
}

void MemoryModel::replaceValue(QString type, QString value) {
    for (int i = 0; i < variables.size(); i++) {
        if (variables[i].name == type) variables[i].description = value;
        QModelIndex myIndex = this->index(i, 1, QModelIndex());
        emit(this->dataChanged(myIndex, myIndex));
    }
}

void MemoryModel::addVariable(QString t, QString n, QString desc,
        bool constant, double i) {
    // if(myIsModel) qDebug() << n << desc;

    insertRow(rowCount());
    variables[(rowCount()-1)].type = t;
    variables[(rowCount()-1)].name = n;
    variables[(rowCount()-1)].description = desc;
    variables[(rowCount()-1)].constant = constant;
    variables[(rowCount()-1)].ivalue = static_cast<int>(i);
    variables[(rowCount()-1)].dvalue = i;
    /*stringListType.replace(rowCount()-1, t);
    stringListName.replace(rowCount()-1, n);
    intListValue.replace(rowCount()-1, (int)i);
    doubleListValue.replace(rowCount()-1, i);*/
}

QStringList MemoryModel::getNames() {
    QStringList names;
    for (int i = 0; i < variables.count(); i++) {
        names.append(variables[i].name);
    }
    return names;
}
