#ifndef MPREDELEGATE_H
#define MPREDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include "memorymodel.h"

class MpreDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    MpreDelegate(MemoryModel * m, QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    /*void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;*/

private slots:
    void commitAndCloseEditor();

private:
    MemoryModel * memory;
};

#endif // MPREDELEGATE_H
