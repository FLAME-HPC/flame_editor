/*!
 * \file mpostdelegate.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of mpost delegate
 */
#include <QtGui>
#include "./mpostdelegate.h"
#include "function_editor/codedialog.h"
#include "./mpost.h"
#include "machinemodel.h"

MpostDelegate::MpostDelegate(MemoryModel * m, MachineModel *t, QObject *parent)
    : QItemDelegate(parent) {
    memory = m;
    machine = t;
}

QWidget *MpostDelegate::createEditor(QWidget */*parent*/,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &/*index*/) const {

    CodeDialog *editor = new CodeDialog(memory);

    connect(editor, SIGNAL(accepted()), this, SLOT(commitAndCloseEditor()));
    connect(editor, SIGNAL(rejected()), this, SLOT(commitAndCloseEditor()));

    // editor->setParent(windowParent);
    editor->setModal(true);
    // editor->move(100, 100);
    // editor->setWindowFlags(WShowModal);

    return editor;
}

void MpostDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const {
    if (qVariantCanConvert<Mpost>(index.data())) {
         Mpost mpost = qVariantValue<Mpost>(index.data());
        // painter->drawText(option.rect, mpre.toString());

         if (option.state & QStyle::State_Selected)
             painter->fillRect(option.rect, option.palette.highlight());

         /*starRating.paint(painter, option.rect, option.palette,
                          StarRating::ReadOnly);*/
         mpost.paint(painter, option.rect, option.palette,
                          Mpost::ReadOnly);
     } else {
         QItemDelegate::paint(painter, option, index);
     }
}

void MpostDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const {
    //qDebug() << "MpostDelegate::setEditorData";
    if (qVariantCanConvert<Mpost>(index.data())) {
        Mpost mpost = qVariantValue<Mpost>(index.data());
        //QTextEdit * textEdit = static_cast<QTextEdit*>(editor);
        // lineEdit->setText(mpost.getText());
        // textEdit->setText(mpost.getText());
        CodeDialog *dialog = static_cast<CodeDialog*>(editor);
        dialog->setMpost(mpost);
        dialog->setName(machine->getRowName(index));
    } else {
         QItemDelegate::setEditorData(editor, index);
    }
}

void MpostDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    //qDebug() << "MpostDelegate::setModelData";
    //if (qVariantCanConvert<Mpost>(index.data())) {
        //qDebug() << "1";
        CodeDialog *dialog = static_cast<CodeDialog*>(editor);
        //qDebug() << "2";
        machine->setRowName(index, dialog->getName());
        model->setData(index, qVariantFromValue(dialog->getMpost()), Qt::EditRole);
        //qDebug() << "3";
        //model->setData(machine->index(index.row(), 3), qVariantFromValue(dialog->getName()));
        //qDebug() << "4";
    //} else {
    //     QItemDelegate::setModelData(editor, model, index);
    //}
}

void MpostDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const {
    editor->setGeometry(option.rect);
}

void MpostDelegate::commitAndCloseEditor() {
    CodeDialog *editor = qobject_cast<CodeDialog *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
    qDebug() << "Close CodeDialog";
}
