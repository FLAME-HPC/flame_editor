/*!
 * \file machinetree.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of model tree
 */
#include "./machinetree.h"
#include "./machine.h"

MachineTree::MachineTree(QObject *parent)
     : QAbstractItemModel(parent) {
    QList<QVariant> rootData;
    rootData << "Title" << "Summary";
    rootItem = new Machine(-1);
    /*Machine * m = new Machine(0, rootItem);
    Machine * p = new Machine(0, rootItem);
    m->appendChild(new Machine(1, m));
    m->appendChild(new Machine(2, m));
    rootItem->appendChild(m);
    rootItem->appendChild(p);*/
    // setupModelData(data.split(QString("\n")), rootItem);
}

MachineTree::~MachineTree() {
    delete rootItem;
}

int MachineTree::columnCount(const QModelIndex &/*parent*/) const {
    return 3;
    /*if (parent.isValid())
        return static_cast<Machine*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();*/
}

QVariant MachineTree::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    Machine *item = static_cast<Machine*>(index.internalPointer());

    if (role == Qt::DisplayRole) {
        return item->data(index.column());
    } else if (role == Qt::CheckStateRole) {
        if (item->isSubModel && index.column() == 0) {
            if (item->isEnabled()) return Qt::Checked;
            else
                return Qt::Unchecked;
        }
    }

    return QVariant();
}

bool MachineTree::setData(const QModelIndex &index, const QVariant &value,
        int role) {
    Machine *item = static_cast<Machine*>(index.internalPointer());

    if (index.isValid()) {  // && role == Qt::EditRole)
        if (index.column() == 0) {
            // plots.at(index.row())->setEnable(qVariantValue<bool>(value));
            if (role == Qt::CheckStateRole) {
                item->setEnabled(qVariantValue<bool>(value));
            }
        } else if (index.column() == 1) {
            if (item->type == 0 || item->type == 1 || item->type == 2 ||
                    item->type == 5) {
                item->name = value.toString();
            } else if (item->type == 6) {  // functionFile
                item->name = value.toString();
            } else if (item->type == 4) {  // timeUnit
                item->setTimeUnit(qVariantValue<TimeUnit>(value));
            }
        } else if (index.column() == 2) {
            item->description = value.toString();
        }
         emit dataChanged(index, index);
         return true;
     }
     return false;
}

Qt::ItemFlags MachineTree::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return 0;

    Machine *item = static_cast<Machine*>(index.internalPointer());

    if (item->type == 0 && index.column() == 0)
        return QAbstractItemModel::flags(index) | Qt::ItemIsEnabled |
                Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    // | Qt::ItemIsEditable;
    // If name or datatype/agent/message description
    else if (index.column() == 1 || ( (item->type == 5 || item->type == 1 ||
            item->type == 2) && index.column() == 2))
        return QAbstractItemModel::flags(index) | Qt::ItemIsEnabled |
                Qt::ItemIsSelectable | Qt::ItemIsEditable;
    else
        return QAbstractItemModel::flags(index) | Qt::ItemIsEnabled |
                Qt::ItemIsSelectable;
}

QVariant MachineTree::headerData(int section,
        Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        // return rootItem->data(section);
        if (section == 0) return QString("Type");
        else if (section == 1) return QString("Value");
        else if (section == 2) return QString("Description");
    }

    return QVariant();
}

QModelIndex MachineTree::index(int row, int column,
        const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Machine *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<Machine*>(parent.internalPointer());

    Machine *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

/** \fn MachineTree::getIndex(Machine *m)
 *  \brief Return the index for the machine in the tree model.
 *
 *  Because the model is a tree structure the index of an element
 *  depends on the index of its parent. Therefore this function is
 *  recursive. See http://developer.qt.nokia.com/doc/qt-4.8/model-view-programming.html#id-0935927b-e27e-4ca5-b394-99175f570a41
 *  for more details.
 *
 *  \param *m Pointer to the machine that the index is for
 *
 */
const QModelIndex MachineTree::getIndex(Machine *m) {
    if (m == rootItem) return QModelIndex();

    for (int i = 0; i < m->parent()->childCount(); i++) {
        if (m == m->parent()->child(i)) {
            // qDebug() << "getIndex" << m->name << i;

            return this->index(i, 0, getIndex(m->parent()));
        }
    }
    return QModelIndex();
}

QModelIndex MachineTree::parent(const QModelIndex &index) const {
    if (!index.isValid())
        return QModelIndex();

    Machine *childItem = static_cast<Machine*>(index.internalPointer());
    Machine *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int MachineTree::rowCount(const QModelIndex &parent) const {
    Machine *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<Machine*>(parent.internalPointer());

    return parentItem->childCount();
}

Machine* MachineTree::addMachine() {
    Machine * m = new Machine(0, rootItem);
    rootItem->appendChild(m);
    m->setRootModel(m);

    Machine * a = new Machine(3, m);
    m->appendChild(a);
    a->setRootModel(m->rootModel());
    a->name = "environment";

    return m;
}

void MachineTree::removeMachine(Machine *m) {
    beginRemoveRows(getIndex(m), m->parent()->childItems.indexOf(m),
            m->parent()->childItems.indexOf(m));
    m->parent()->removeChild(m);
    endRemoveRows();
}

Machine * MachineTree::insertMachine(Machine *m, int type) {
    // Find index to place new machine
    // sub model
    // environment
    //  functions files
    //  time units
    //  data types
    // agents
    // messages

    Machine * n = m;
    int index = -1;
    if (type == 4 || type == 5 || type == 6) {
        for (int i = 0; i < m->childCount(); i++)
            if (m->child(i)->type == 3) n = m->child(i);
    }

    // qDebug() << n->name;

    if (type == 3) index = 0;
    for (int i = 0; i < n->childCount(); i++) {
        // qDebug() << i << n->child(i)->type << n->child(i)->name;
        if (type == 0 && n->child(i)->type == 3) {
            index = i;
            break; }
        if (type == 1 && n->child(i)->type == 2) {
            index = i;
            break; }
        if (type == 6 && (n->child(i)->type == 4 || n->child(i)->type == 5)) {
            index = i;
            break; }
        if (type == 4 && n->child(i)->type == 5) {
            index = i;
            break; }
    }
    if (index == -1) index = n->childCount();

    beginInsertRows(getIndex(n), index, index);
    Machine * m1 = new Machine(type, n);
    m1->setRootModel(n->rootModel());
    if (type == 0) {
        m1->isSubModel = true;
        Machine * a = new Machine(3, m1);
        m1->appendChild(a);
        a->setRootModel(m1->rootModel());
    }
    n->insertChild(m1, index);
    endInsertRows();

    return m1;
}
