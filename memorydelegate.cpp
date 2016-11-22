/*!
 * \file memorydelegate.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of the model memory delegate
 */
#include <QtGui>
#include "./memorydelegate.h"

MemoryDelegate::MemoryDelegate(QObject *parent)
    : QItemDelegate(parent) {
}

QWidget *MemoryDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const {
    /*if(index.column() == 0)
    {
        QComboBox *editor = new QComboBox(parent);
        editor->insertItem(0, "int");
        editor->insertItem(1, "double");

        return editor;
    }
    else if(index.column() == 2)
    {
        QModelIndex typeIndex = index.model()->index(index.row(), 0, QModelIndex());

        if (typeIndex.data().toString() == "int")
        {
            QSpinBox *editor = new QSpinBox(parent);
            editor->setMinimum(-1000);
            editor->setMaximum(1000);
            return editor;
        }
        //if (typeIndex.data().toString() == "double")
        else
        {
            QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
            editor->setMinimum(-1000);
            editor->setMaximum(1000);
            return editor;
        }
    }
    else */if(index.column() == 0 || index.column() == 1 ||
            index.column() == 2) {
        QLineEdit *editor = new QLineEdit(parent);
        return editor;
    } else {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void MemoryDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const {
    /*if(index.column() == 0)
    {
        QString value = index.data().toString();

        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        for(int i = 0; i < comboBox->count(); i++)
        {
            if(comboBox->itemText(i) == value)
                comboBox->setCurrentIndex(i);
        }
    }
    else if(index.column() == 2)
    {
        QModelIndex typeIndex = index.model()->index(index.row(), 0, QModelIndex());

        if (typeIndex.data().toString() == "int")
        {
            int value = index.data().toInt();

            QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
            spinBox->setValue(value);
        }
        //if (typeIndex.data().toString() == "double")
        else
        {
            double value = index.data().toDouble();

            QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
            spinBox->setValue(value);
        }
    }
    else */if(index.column() == 0 || index.column() == 1 ||
            index.column() == 2) {
        QString value = index.data().toString();

        QLineEdit * lineEdit = static_cast<QLineEdit*>(editor);
        lineEdit->setText(value);
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}

void MemoryDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    /*if(index.column() == 0)
    {
            QComboBox *comboBox = static_cast<QComboBox*>(editor);
            QString value = comboBox->currentText();

            model->setData(index, value, Qt::EditRole);
    }
    else if(index.column() == 2)
    {
        QModelIndex typeIndex = index.model()->index(index.row(), 0, QModelIndex());

        if (typeIndex.data().toString() == "int")
        {
            QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
            spinBox->interpretText();
            int value = spinBox->value();

            model->setData(index, value, Qt::EditRole);
        }
        //if (typeIndex.data().toString() == "double")
        else
        {
            QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
            //spinBox->interpretText();
            double value = spinBox->value();

            model->setData(index, value, Qt::EditRole);
        }
    }
    else */
    if (index.column() == 0 || index.column() == 1 ||
            index.column() == 2) {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString value = lineEdit->text();

        model->setData(index, value, Qt::EditRole);
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}

void MemoryDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const {
    editor->setGeometry(option.rect);
}
