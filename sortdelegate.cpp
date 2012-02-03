/*!
 * \file sortdelegate.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of message sort delegate
 */
#include <QtGui>
#include "./sortdelegate.h"
#include "./sortdialog.h"
#include "./messagesort.h"

SortDelegate::SortDelegate(Machine * m, Communication * comm , QObject *parent)
    : QItemDelegate(parent) {
    machine = m;
    communication = comm;
}

void SortDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const {
    if (qVariantCanConvert<MessageSort>(index.data())) {
         MessageSort sort = qVariantValue<MessageSort>(index.data());
        // painter->drawText(option.rect, Comm.toString());

         if (option.state & QStyle::State_Selected)
             painter->fillRect(option.rect, option.palette.highlight());

         /*starRating.paint(painter, option.rect, option.palette,
                          StarRating::ReadOnly);*/
         sort.paint(painter, option.rect, option.palette,
                          MessageSort::ReadOnly);
     } else {
         QItemDelegate::paint(painter, option, index);
     }
}


QWidget *SortDelegate::createEditor(QWidget */*parent*/,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &index) const {
        SortDialog *editor = new SortDialog(machine,
                &communication->messageModel->
                getMessages()[index.row()].messageType);

        connect(editor, SIGNAL(accepted()), this, SLOT(commitAndCloseEditor()));
        connect(editor, SIGNAL(rejected()), this, SLOT(commitAndCloseEditor()));

        // editor->setParent(windowParent);
        editor->setModal(true);
        // editor->move(100, 100);
        // editor->setWindowFlags(WShowModal);

        return editor;
}

void SortDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const {
    if (qVariantCanConvert<MessageSort>(index.data())) {
        MessageSort sort = qVariantValue<MessageSort>(index.data());
        SortDialog *dialog = static_cast<SortDialog*>(editor);

        dialog->setSort(sort);
    } else {
         QItemDelegate::setEditorData(editor, index);
    }
}

void SortDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    if (qVariantCanConvert<MessageSort>(index.data())) {
         SortDialog *dialog = static_cast<SortDialog*>(editor);
         model->setData(index, qVariantFromValue(dialog->getSort()));
    } else {
         QItemDelegate::setModelData(editor, model, index);
    }
}

/*void CommDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}*/

void SortDelegate::commitAndCloseEditor() {
    SortDialog *editor = qobject_cast<SortDialog *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
