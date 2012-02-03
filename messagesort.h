/*!
 * \file messagesort.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for the message sort
*/
#ifndef MESSAGESORT_H_
#define MESSAGESORT_H_

#include <QMetaType>
#include <QtGui>
#include <QPainter>

class MessageSort {
  public:
    enum EditMode { Editable, ReadOnly };

    MessageSort();

    void paint(QPainter *painter, const QRect &rect,
                    const QPalette &palette, EditMode mode) const;

    bool isRandom;
    bool isSort;
    QString key;
    QString order;
};

Q_DECLARE_METATYPE(MessageSort)

#endif  // MESSAGESORT_H_
