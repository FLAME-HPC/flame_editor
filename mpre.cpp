/*!
 * \file mpre.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of mpre
 */
#include <QtGui>
#include "./mpre.h"

Mpre::Mpre(QString n, QString o, int v, bool enabled, bool neg) {
    myName = n;
    myOp = o;
    myValue = v;
    myEnabled = enabled;
    myNot = neg;
}

void Mpre::paint(QPainter *painter, const QRect &rect,
                        const QPalette &/*palette*/, EditMode /*mode*/) const {
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);

    if (myEnabled) {
        QString s;
        if (myNot) s.append("not ");
        s.append(myName);
        s.append(" ");
        s.append(myOp);
        s.append(" ");
        s.append(QString("%1").arg(myValue));

        painter->drawText(rect, s);
    }

     /*painter->setPen(Qt::NoPen);

     if (mode == Editable) {
         painter->setBrush(palette.highlight());
     } else {
         painter->setBrush(palette.foreground());
     }

     int yOffset = (rect.height() - PaintingScaleFactor) / 2;
     painter->translate(rect.x(), rect.y() + yOffset);
     painter->scale(PaintingScaleFactor, PaintingScaleFactor);

     for (int i = 0; i < myMaxStarCount; ++i) {
         if (i < myStarCount) {
             painter->drawPolygon(starPolygon, Qt::WindingFill);
         } else if (mode == Editable) {
             painter->drawPolygon(diamondPolygon, Qt::WindingFill);
         }
         painter->translate(1.0, 0.0);
     }*/

     painter->restore();
}
