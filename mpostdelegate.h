#ifndef MPOSTDELEGATE_H
#define MPOSTDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include "memorymodel.h"

class MpostDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    MpostDelegate(MemoryModel * m = 0, QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    MemoryModel * memory;
};

#endif // MPOSTDELEGATE_H
