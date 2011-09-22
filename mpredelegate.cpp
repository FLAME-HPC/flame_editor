#include "mpredelegate.h"
#include "mpredialog.h"
#include "mpre.h"

#include <QtGui>

MpreDelegate::MpreDelegate(MemoryModel * m, QObject *parent)
    : QItemDelegate(parent)
{
    memory = m;
}

void MpreDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
 {
    if (qVariantCanConvert<Mpre>(index.data()))
    {
         Mpre mpre = qVariantValue<Mpre>(index.data());
        //painter->drawText(option.rect, mpre.toString());

         if (option.state & QStyle::State_Selected)
             painter->fillRect(option.rect, option.palette.highlight());

         /*starRating.paint(painter, option.rect, option.palette,
                          StarRating::ReadOnly);*/
         mpre.paint(painter, option.rect, option.palette,
                          Mpre::ReadOnly);
     }
     else
     {
         QItemDelegate::paint(painter, option, index);
     }
 }


QWidget *MpreDelegate::createEditor(QWidget */*parent*/,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &/*index*/) const
{
        //qDebug() << "MpreDelegate parent: " << windowParent;

        MpreDialog *editor = new MpreDialog(memory);

        connect(editor, SIGNAL(accepted()), this, SLOT(commitAndCloseEditor()));
        connect(editor, SIGNAL(rejected()), this, SLOT(commitAndCloseEditor()));

        //editor->setParent(windowParent);
        editor->setModal(true);
        //editor->move(100, 100);
        //editor->setWindowFlags(WShowModal);

        return editor;
}

void MpreDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    if (qVariantCanConvert<Mpre>(index.data()))
    {
        Mpre mpre = qVariantValue<Mpre>(index.data());
        MpreDialog *dialog = static_cast<MpreDialog*>(editor);

        dialog->setMpre(mpre);
    }
    else
    {
         QItemDelegate::setEditorData(editor, index);
    }
}

void MpreDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    if (qVariantCanConvert<Mpre>(index.data()))
    {
         MpreDialog *dialog = static_cast<MpreDialog*>(editor);
         model->setData(index, qVariantFromValue(dialog->getMpre()));
    }
    else
    {
         QItemDelegate::setModelData(editor, model, index);
    }
}

/*void MpreDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}*/

void MpreDelegate::commitAndCloseEditor()
{
    MpreDialog *editor = qobject_cast<MpreDialog *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
