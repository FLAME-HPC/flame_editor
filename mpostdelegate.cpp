#include "mpostdelegate.h"
#include "mpostdialog.h"
#include "mpost.h"

#include <QtGui>

MpostDelegate::MpostDelegate(MemoryModel * m, QObject *parent)
    : QItemDelegate(parent)
{
    memory = m;
}

QWidget *MpostDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &/*index*/) const
{
    QTextEdit *editor = new QTextEdit(parent);
    return editor;
}

void MpostDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
 {
    if (qVariantCanConvert<Mpost>(index.data()))
    {
         Mpost mpost = qVariantValue<Mpost>(index.data());
        //painter->drawText(option.rect, mpre.toString());

         if (option.state & QStyle::State_Selected)
             painter->fillRect(option.rect, option.palette.highlight());

         /*starRating.paint(painter, option.rect, option.palette,
                          StarRating::ReadOnly);*/
         mpost.paint(painter, option.rect, option.palette,
                          Mpost::ReadOnly);
     }
     else
     {
         QItemDelegate::paint(painter, option, index);
     }
 }

void MpostDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    if (qVariantCanConvert<Mpost>(index.data()))
    {
        Mpost mpost = qVariantValue<Mpost>(index.data());
        QTextEdit * textEdit = static_cast<QTextEdit*>(editor);
        //lineEdit->setText(mpost.getText());
        textEdit->setText(mpost.getText());
    }
    else
    {
         QItemDelegate::setEditorData(editor, index);
    }
}

void MpostDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    if (qVariantCanConvert<Mpost>(index.data()))
    {
        QTextEdit *textEdit = static_cast<QTextEdit*>(editor);
        Mpost mpost;
        mpost.setMemory(this->memory);
        mpost.setText(textEdit->toPlainText());// lineEdit->text());
        model->setData(index, qVariantFromValue(mpost));
    }
    else
    {
         QItemDelegate::setModelData(editor, model, index);
    }
}

void MpostDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}
