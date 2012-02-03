/*!
 * \file texteditdelegate.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for the text box edit delegate
*/
#ifndef TEXTEDITDELEGATE_H_
#define TEXTEDITDELEGATE_H_

#include <QItemDelegate>

class TextEditDelegate : public QItemDelegate {
    Q_OBJECT

  public:
    explicit TextEditDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif  // TEXTEDITDELEGATE_H_
