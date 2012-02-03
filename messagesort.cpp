/*!
 * \file messagesort.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of message sort
 */
#include "./messagesort.h"

MessageSort::MessageSort() {
    isRandom = false;
    isSort = false;
    key = "";
    order = "";
}

void MessageSort::paint(QPainter *painter, const QRect &rect,
                       const QPalette &/*palette*/, EditMode /*mode*/) const {
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);

    QString s;
    if (isRandom)
        s.append("random");
    else
        if (isSort) {
       s.append("sort(");
       s.append(key);
       s.append(", ");
       s.append(order);
       s.append(")");
    }
    painter->drawText(rect, s);

    painter->restore();
}
