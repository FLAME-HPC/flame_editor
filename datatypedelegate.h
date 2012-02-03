/*!
 * \file datatypedelegate.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for model datatype delegate
 */
#ifndef DATATYPEDELEGATE_H_
#define DATATYPEDELEGATE_H_

#include <QItemDelegate>
#include "./machine.h"

class DataTypeDelegate : public QItemDelegate {
    Q_OBJECT

  public:
    DataTypeDelegate(Machine * m, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

  private:
    Machine * machine;
};

#endif  // DATATYPEDELEGATE_H_
