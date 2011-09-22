#include <QtGui>
#include "statedelegate.h"

StateDelegate::StateDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *StateDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &/*index*/) const
{
    QLineEdit *editor = new QLineEdit(parent);
    return editor;
}

void StateDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QString value = index.data().toString();

    QLineEdit * lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value);
}

void StateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString value = lineEdit->text();

    model->setData(index, value, Qt::EditRole);
}

void StateDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}
