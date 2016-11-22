/*!
 * \file texteditdelegate.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of text box edit delegate
 */
#include <QTextEdit>
#include "./texteditdelegate.h"

TextEditDelegate::TextEditDelegate(QObject * parent)
    : QItemDelegate(parent) {
}

QWidget *TextEditDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &/*index*/) const {
    QTextEdit * editor = new QTextEdit(parent);
    return editor;
}

void TextEditDelegate::setEditorData(QWidget *editor,
                                   const QModelIndex &index) const {
    QString value = index.data().toString();

    QTextEdit *textEdit = static_cast<QTextEdit*>(editor);
    textEdit->setText(value);
}

void TextEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                  const QModelIndex &index) const {
    QTextEdit *textEdit = static_cast<QTextEdit*>(editor);
    QString value = textEdit->toPlainText();

    model->setData(index, value, Qt::EditRole);
}

void TextEditDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const {
    editor->setGeometry(option.rect);
}
