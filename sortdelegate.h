/*!
 * \file sortdelegate.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for message sort delegate
*/
#ifndef SORTDELEGATE_H_
#define SORTDELEGATE_H_

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include "./machine.h"
#include "./communication.h"

class SortDelegate : public QItemDelegate {
    Q_OBJECT

  public:
    SortDelegate(Machine * machine, Communication * comm = 0,
        QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
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
    Communication * communication;
};

#endif  // SORTDELEGATE_H_
