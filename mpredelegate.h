/*!
 * \file mpredelegate.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for mpre delegate
*/
#ifndef MPREDELEGATE_H_
#define MPREDELEGATE_H_

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include "./memorymodel.h"
#include "./machine.h"
#include "./communication.h"

class MpreDelegate : public QItemDelegate {
    Q_OBJECT

  public:
    MpreDelegate(Machine * machine, Communication * comm = 0,
            QObject *parent = 0);

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
    Machine * machine;
    Communication * communication;
};

#endif  // MPREDELEGATE_H_
