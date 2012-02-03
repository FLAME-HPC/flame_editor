/*!
 * \file messagedelegate.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for the message delegate
*/
#ifndef MESSAGEDELEGATE_H_
#define MESSAGEDELEGATE_H_

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QComboBox>
#include <QStringList>

class MessageDelegate : public QItemDelegate {
    Q_OBJECT

  public:
    MessageDelegate(QStringList messageTypes, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

  private:
    QStringList messageTypes;
};

#endif  // MESSAGEDELEGATE_H_
