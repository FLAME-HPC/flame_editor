/*!
 * \file commdelegate.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for message communication delegate
 */
#ifndef COMMDELEGATE_H_
#define COMMDELEGATE_H_

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include "./machine.h"

class CommDelegate : public QItemDelegate {
    Q_OBJECT

  public:
    CommDelegate(Machine * machine, QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
            const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option,
            const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
            const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
            const QModelIndex &index) const;

  private slots:
    void commitAndCloseEditor();

  private:
    Machine * machine;
};

#endif  // COMMDELEGATE_H_
