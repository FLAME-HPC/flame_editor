/*!
 * \file mpostdelegate.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for mpost delegate
*/
#ifndef MPOSTDELEGATE_H_
#define MPOSTDELEGATE_H_

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include "./memorymodel.h"

class MpostDelegate : public QItemDelegate {
    Q_OBJECT

  public:
    MpostDelegate(MemoryModel * m = 0, QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                const QModelIndex &index) const;
    QWidget *createEditor(QWidget *, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

  private slots:
    void commitAndCloseEditor();

  private:
    MemoryModel * memory;
};

#endif  // MPOSTDELEGATE_H_
