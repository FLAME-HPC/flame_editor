#include "variabledeclareddelegate.h"

VariableDeclaredDelegate::VariableDeclaredDelegate(QObject *parent)
{
}

QWidget *VariableDeclaredDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
    if(index.column() == 0)
    {
        QComboBox *editor = new QComboBox(parent);
        editor->insertItem(0, "int");
        editor->insertItem(1, "double");

        return editor;
    }
    else if(index.column() == 1)
    {
        QLineEdit *editor = new QLineEdit(parent);
        return editor;
    }
    else if(index.column() == 2)
    {
        QLineEdit *editor = new QLineEdit(parent);
        return editor;
    }
    else return QItemDelegate::createEditor(parent, option, index);
}

void VariableDeclaredDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    if(index.column() == 0)
    {
        QString value = index.data().toString();

        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        for(int i = 0; i < comboBox->count(); i++)
        {
            if(comboBox->itemText(i) == value)
                comboBox->setCurrentIndex(i);
        }
    }
    else if(index.column() == 1)
    {
        QString value = index.data().toString();

        QLineEdit * lineEdit = static_cast<QLineEdit*>(editor);
        lineEdit->setText(value);
    }
    else if(index.column() == 2)
    {
        QString value = index.data().toString();

        QLineEdit * lineEdit = static_cast<QLineEdit*>(editor);
        lineEdit->setText(value);
    }
    else
    {
        QItemDelegate::setEditorData(editor, index);
    }
}

void VariableDeclaredDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    if(index.column() == 0)
    {
            QComboBox *comboBox = static_cast<QComboBox*>(editor);
            QString value = comboBox->currentText();

            model->setData(index, value, Qt::EditRole);
    }
    else if(index.column() == 1)
    {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString value = lineEdit->text();

        model->setData(index, value, Qt::EditRole);
    }
    else if(index.column() == 2)
    {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString value = lineEdit->text();

        model->setData(index, value, Qt::EditRole);
    }
    else
    {
        QItemDelegate::setModelData(editor, model, index);
    }
}

void VariableDeclaredDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}
