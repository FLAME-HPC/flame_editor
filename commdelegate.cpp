/*!
 * \file commdelegate.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of message communication delegate
 */
#include <QtGui>
#include <algorithm>
#include "./commdelegate.h"
#include "./communication.h"
#include "./commdialog.h"

CommDelegate::CommDelegate(Machine * m, QObject *parent)
    : QItemDelegate(parent) {
    machine = m;
}

void CommDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const {
    if (qVariantCanConvert<Communication>(index.data())) {
         Communication communication =
                 qVariantValue<Communication>(index.data());
        QStyle &style = *QApplication::style();
           painter->save();

           QStyleOptionViewItemV4 opt = option;

           style.drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, 0);

           QRect textRect = style.subElementRect(
                   QStyle::SE_ItemViewItemText, &opt, 0);

           // following code borrowed from qcommonstyle.cpp
           QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ?
                   QPalette::Normal : QPalette::Disabled;
           if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active)) {
                cg = QPalette::Inactive;
           }
           if (opt.state & QStyle::State_Selected) {
               painter->setPen(QPen(opt.palette.brush(cg,
                       QPalette::HighlightedText), 0));
           } else {
               painter->setPen(QPen(opt.palette.brush(cg, QPalette::Text), 0));
           }
           if (opt.state & QStyle::State_Editing) {
               painter->setPen(QPen(opt.palette.brush(cg, QPalette::Text), 0));
               painter->drawRect(textRect.adjusted(0, 0, -1, -1));
           }

           QFontMetrics metrics(opt.font);
           int height = 0;

           QStringList slist = communication.toString().split("\n");
           for (int i = 0; i < slist.count(); i++) {
             textRect.setTop(textRect.top() + height);
             style.drawItemText(painter, textRect, Qt::AlignLeft,
                     opt.palette, true, elidedText(opt.fontMetrics,
                     textRect.width(), Qt::ElideRight, slist.at(i)));
             QRect bRect = metrics.boundingRect(slist.at(i));
             height = bRect.height();
           }

           painter->restore();
     } else {
         QItemDelegate::paint(painter, option, index);
     }
}

QSize CommDelegate::sizeHint(const QStyleOptionViewItem &option,
        const QModelIndex &index) const {
    if (qVariantCanConvert<Communication>(index.data())) {
         Communication communication =
                 qVariantValue<Communication>(index.data());
         // return communication.sizeHint(option, index);
         int height = 0;
         int width = 0;
         QStringList slist = communication.toString().split("\n");
         for (int i = 0; i < slist.count(); i++) {
             QRect rect = option.fontMetrics.boundingRect(slist.at(i));
             height += rect.height();
             width = std::max(width, rect.width());
         }

         int hmargin = QApplication::style()->pixelMetric(
                 QStyle::PM_FocusFrameHMargin) + 1;
         int vmargin = QApplication::style()->pixelMetric(
                 QStyle::PM_FocusFrameVMargin) + 1;

         return QSize(width+hmargin, height+vmargin);
     } else {
         return QItemDelegate::sizeHint(option, index);
     }
}

QWidget *CommDelegate::createEditor(QWidget */*parent*/,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &/*index*/) const {
        CommDialog *editor = new CommDialog(machine);

        connect(editor, SIGNAL(accepted()), this, SLOT(commitAndCloseEditor()));
        connect(editor, SIGNAL(rejected()), this, SLOT(commitAndCloseEditor()));

        // editor->setParent(windowParent);
        editor->setModal(true);
        // editor->move(100, 100);
        // editor->setWindowFlags(WShowModal);

        return editor;
}

void CommDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const {
    if (qVariantCanConvert<Communication>(index.data())) {
        Communication communication =
                qVariantValue<Communication>(index.data());
        CommDialog *dialog = static_cast<CommDialog*>(editor);

        dialog->setCommunication(communication);
    } else {
         QItemDelegate::setEditorData(editor, index);
    }
}

void CommDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    if (qVariantCanConvert<Communication>(index.data())) {
         CommDialog *dialog = static_cast<CommDialog*>(editor);
         model->setData(index, qVariantFromValue(dialog->getCommunication()));
    } else {
         QItemDelegate::setModelData(editor, model, index);
    }
}

/*void CommDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}*/

void CommDelegate::commitAndCloseEditor() {
    CommDialog *editor = qobject_cast<CommDialog *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
