/*!
 * \file datatypedelegate.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of model data type delegate
 */
#include "./datatypedelegate.h"

DataTypeDelegate::DataTypeDelegate(Machine * m, QObject * parent)
    : QItemDelegate(parent) {
    machine = m;
}


QWidget *DataTypeDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &/*index*/) const {
    QComboBox *editor = new QComboBox(parent);
    QStringList dataTypes = machine->getDataTypes();
    for (int i = 0; i < dataTypes.count(); i++) {
        editor->insertItem(i, dataTypes.at(i));
    }
    return editor;
}

void DataTypeDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const {
    QString value = index.data().toString();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    for (int i = 0; i < comboBox->count(); i++) {
        if (comboBox->itemText(i) == value)
            comboBox->setCurrentIndex(i);
    }
}

void DataTypeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();

    model->setData(index, value, Qt::EditRole);
}

void DataTypeDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const {
    editor->setGeometry(option.rect);
}
