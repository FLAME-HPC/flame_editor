/*!
 * \file mpredelegate.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of mpre delegate
 */
#include <QtGui>
#include "./mpredelegate.h"
#include "./mpredialog.h"
#include "./condition.h"
#include "./messagemodel.h"

MpreDelegate::MpreDelegate(Machine * m, Communication * comm , QObject *parent)
    : QItemDelegate(parent) {
    machine = m;
    memory = m->memoryModel;
    communication = comm;
}

void MpreDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const {
    if (qVariantCanConvert<Condition>(index.data())) {
         Condition condition = qVariantValue<Condition>(index.data());

         /*if (option.state & QStyle::State_Selected)
             painter->fillRect(option.rect, option.palette.highlight());
         condition.paint(painter, option.rect, option.palette,
                          Condition::ReadOnly);*/

         QStyle &style = *QApplication::style();
            painter->save();

            QStyleOptionViewItemV4 opt = option;
            // initStyleOption(&opt, index);

            // opt.textElideMode = Qt::ElideMiddle; //Qt::ElideRight;
            // this->elidedText()

            style.drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, 0);

            QRect textRect = style.subElementRect(QStyle::SE_ItemViewItemText,
                    &opt, 0);

            // following code borrowed from qcommonstyle.cpp
            QPalette::ColorGroup cg =
                    opt.state & QStyle::State_Enabled ?
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

            QStringList slist = condition.toString().split("\n");
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


QWidget *MpreDelegate::createEditor(QWidget */*parent*/,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &index) const {
        MpreDialog *editor;
        if (communication) editor =
            new MpreDialog(machine,
            &communication->messageModel->
            getMessages()[index.row()].messageType);
        else
            editor = new MpreDialog(machine);

        connect(editor, SIGNAL(accepted()), this, SLOT(commitAndCloseEditor()));
        connect(editor, SIGNAL(rejected()), this, SLOT(commitAndCloseEditor()));

        // editor->setParent(windowParent);
        editor->setModal(true);
        // editor->move(100, 100);
        // editor->setWindowFlags(WShowModal);

        return editor;
}

void MpreDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const {
    if (qVariantCanConvert<Condition>(index.data())) {
        Condition condition = qVariantValue<Condition>(index.data());
        MpreDialog *dialog = static_cast<MpreDialog*>(editor);

        dialog->setCondition(condition);
    } else {
         QItemDelegate::setEditorData(editor, index);
    }
}

void MpreDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    if (qVariantCanConvert<Condition>(index.data())) {
         MpreDialog *dialog = static_cast<MpreDialog*>(editor);
         model->setData(index, qVariantFromValue(dialog->getCondition()));
    } else {
         QItemDelegate::setModelData(editor, model, index);
    }
}

/*void MpreDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}*/

void MpreDelegate::commitAndCloseEditor() {
    MpreDialog *editor = qobject_cast<MpreDialog *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
