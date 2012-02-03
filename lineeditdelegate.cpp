/*!
 * \file lineeditdelegate.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of text line edit delegate
 */
#include <QLineEdit>
#include "./lineeditdelegate.h"

LineEditDelegate::LineEditDelegate(QObject * parent)
    : QItemDelegate(parent) {
}

QWidget *LineEditDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &/*index*/) const {
    QLineEdit *editor = new QLineEdit(parent);
    return editor;
}

void LineEditDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const {
    QString value = index.data().toString();

    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value);
}

void LineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString value = lineEdit->text();

    model->setData(index, value, Qt::EditRole);
}

void LineEditDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const {
    editor->setGeometry(option.rect);
}
