/*!
 * \file messagedelegate.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of message delegate
 */
#include <QtGui>
#include "./messagedelegate.h"

MessageDelegate::MessageDelegate(QStringList mts, QObject *parent)
    : QItemDelegate(parent) {
    messageTypes = mts;
}


QWidget *MessageDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const {
    if (index.column() == 0) {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems(messageTypes);

        return editor;
    } else {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void MessageDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const {
    if (index.column() == 0) {
        QString value = index.data().toString();

        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        for (int i = 0; i < comboBox->count(); i++) {
            if (comboBox->itemText(i) == value)
                comboBox->setCurrentIndex(i);
        }
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}

void MessageDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    if (index.column() == 0) {
            QComboBox *comboBox = static_cast<QComboBox*>(editor);
            QString value = comboBox->currentText();

            model->setData(index, value, Qt::EditRole);
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}

void MessageDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const {
    editor->setGeometry(option.rect);
}
