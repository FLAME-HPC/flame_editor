/*!
 * \file machinetreedelegate.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for the model tree delegate
*/
#ifndef MACHINETREEDELEGATE_H_
#define MACHINETREEDELEGATE_H_

#include <QItemDelegate>

class MachineTreeDelegate : public QItemDelegate {
    Q_OBJECT

  public:
    explicit MachineTreeDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

  private slots:
    void commitAndCloseEditor();
};

#endif  // MACHINETREEDELEGATE_H_
