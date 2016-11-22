/*!
 * \file lineeditdelegate.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for text line edit delegate
*/
#ifndef LINEEDITDELEGATE_H_
#define LINEEDITDELEGATE_H_

#include <QItemDelegate>

class LineEditDelegate : public QItemDelegate {
    Q_OBJECT

  public:
    explicit LineEditDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif  // LINEEDITDELEGATE_H_
