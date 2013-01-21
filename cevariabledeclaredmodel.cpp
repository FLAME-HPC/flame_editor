#include "cevariabledeclaredmodel.h"

#include <QtGui>

int CEVariableDeclaredModel::rowCount(const QModelIndex &/*parent*/) const
 {
     //return stringListType.count();
    return variables.count();
 }

int CEVariableDeclaredModel::columnCount(const QModelIndex &/*parent*/) const
 {
     return 3;
 }

QVariant CEVariableDeclaredModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (index.row() >= variables.size())
         return QVariant();

     if (role == Qt::DisplayRole)
     {
         if(index.column() == 0) return variables.at(index.row()).getType();
         if(index.column() == 1) return variables.at(index.row()).getName();
         if(index.column() == 2) return variables.at(index.row()).getExpression();
         return QVariant();
     }
     else return QVariant();
 }

QVariant CEVariableDeclaredModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const
 {
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal)
     {
         if(section == 0) return QString("Type");
         else if(section == 1) return QString("Name");
         else //if(section == 2)
             return QString("Expression");
     }
     else
         return QString("Row %1").arg(section);
 }

Qt::ItemFlags CEVariableDeclaredModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return Qt::ItemIsEnabled;

     return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
 }

bool CEVariableDeclaredModel::setData(const QModelIndex &index,
                               const QVariant &value, int role)
 {
     if (index.isValid() && role == Qt::EditRole)
    {
        if(index.column() == 0)
            variables[index.row()].setType(value.toString());
        if(index.column() == 1)
            variables[index.row()].setName(value.toString());
        if(index.column() == 2)
            variables[index.row()].setExpression(value.toString());

         emit dataChanged(index, index);
         return true;
     }
     return false;
 }

bool CEVariableDeclaredModel::insertRows(int position, int rows, const QModelIndex &/*parent*/)
 {
     beginInsertRows(QModelIndex(), position, position+rows-1);
     for (int row = 0; row < rows; ++row)
     {
         QStringList list(getNames());
         QString s;
         int i = this->rowCount();
         do{
             s = QString("variable_%1").arg(i++);
         }while(memoryVariableNames->contains(s) || list.contains(s));

         variables.insert(position, CEVariableDeclared("int", s, ""));
     }

     endInsertRows();
     return true;
 }

bool CEVariableDeclaredModel::removeRows(int position, int rows, const QModelIndex &/*parent*/)
 {
     beginRemoveRows(QModelIndex(), position, position+rows-1);

     for (int row = 0; row < rows; ++row)
     {
         variables.removeAt(position);

         /*stringListType.removeAt(position);
         stringListName.removeAt(position);
         intListValue.removeAt(position);
         doubleListValue.removeAt(position);*/
     }

     endRemoveRows();
     return true;
 }

void CEVariableDeclaredModel::replaceValue(int i, double value)
{
//    variables[i].ivalue = (int)value;
//    variables[i].dvalue = value;
//    //doubleListValue.replace(i, value);

//    QModelIndex myIndex = this->index(i, 2, QModelIndex());

//    emit( this->dataChanged(myIndex, myIndex) );
}

void CEVariableDeclaredModel::addVariable()
{
    insertRow(rowCount());
}

void CEVariableDeclaredModel::setVariables(QList<CEVariableDeclared> *list)
{
    beginResetModel();
    variables.clear();
    while(list->count() > 0)
    {
        variables.append(list->at(0));
        list->removeAt(0);
    }
    endResetModel();
}

QStringList CEVariableDeclaredModel::getNames()
{
    QStringList names;
    for(int i=0; i < variables.count(); i++)
    {
        names.append(variables[i].getName());
    }
    return names;
}
