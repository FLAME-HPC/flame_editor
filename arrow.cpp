/*!
 * \file arrow.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of graphics arrow in stategraphs
 */
#include <QtGui>
#include <math.h>
#include "./arrow.h"

const qreal Pi = 3.14; /* definition of Pi */

/*!
 * \brief Arrow constructor
 * \param[in] startItem The tail graphics item
 * \param[in] endItem The head graphics item
 * \param[in] parent Not required, is automatically initalised
 * \param[in] scene Not required, is automatically initalised
 *
 * This contructor takes starting and ending graphics items for an arrow to be drawn between them.
 */
Arrow::Arrow(GraphicsItem *startItem, GraphicsItem *endItem,
         QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene) {
    myStartItem = startItem;
    myEndItem = endItem;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    myColor = Qt::black;
    setPen(QPen(myColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    offset = 0;
    number = 1;
    total = 1;
    foreign = false;
    showHead = true;
    isCommunication = false;
    myTransition = 0;
}

/*!
 * \brief Set a new name
 * \param[in] n The new name
 */
void Arrow::setName(QString n) {
    name = n;
}

/*!
 * \brief Set a new \c Mpre
 * \param[in] m The new Mpre
 */
void Arrow::setMpre(Mpre m) {
    mpre = m;
}

/*!
 * \brief Set a new \c Mpost
 * \param[in] mp The new Mpost
 */
void Arrow::setMpost(Mpost mp) {
    mpost = mp;
}

QRectF Arrow::boundingRect() const {
    // qreal extra = ((pen().width() + 20 + offset) / 2.0);
    qreal extra = ((pen().width() + 20 + offset));

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath Arrow::shape() const {
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void Arrow::updatePosition() {
    QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    setLine(line);
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *) {
    if (myStartItem->collidesWithItem(myEndItem))
        return;

    QPen myPen = pen();
    myPen.setColor(myColor);
    qreal arrowSize = 10;
    painter->setPen(myPen);

    if (foreign) painter->setPen(Qt::gray);
    else
        painter->setPen(isSelected() ? Qt::red : Qt::black);
    if (isCommunication)
        painter->setPen(isSelected() ? Qt::darkGreen : Qt::green);
    painter->setRenderHint(QPainter::Antialiasing);

    // new
    QPainterPath endPath = myEndItem->shape();
    float dif = 0.1;
    QPointF intersectionPoint;
    QPointF ip;
    QLineF centerLine2(myStartItem->scenePos(), myEndItem->scenePos());



    for (float i = 0; i < 1.0; i+=dif) {
        float e = i+dif;
        if (e > 0.99) e = 0;
        QLineF myLine(endPath.pointAtPercent(i), endPath.pointAtPercent(e));
        QLineF myLine2(myLine.x1()+centerLine2.x2(),
                        myLine.y1()+centerLine2.y2(),
                        myLine.x2()+centerLine2.x2(),
                        myLine.y2()+centerLine2.y2());
        int rc = centerLine2.intersect(myLine2, &ip);
        if (rc == QLineF::BoundedIntersection) {
            intersectionPoint = ip;
            break;
        }
    }

    setLine(QLineF(intersectionPoint, myStartItem->pos()));



    double angle = ::acos(line().dx() / line().length());
    // double angle2 = angle;
    if (line().dy() >= 0)
        angle = (Pi * 2) - angle;

    QPainterPath path;
    QPointF half(line().x1() + (line().dx()/2), line().y1() + (line().dy()/2));
    path.moveTo(line().p1());
    // QPointF c2(line().x1()+(line().dx()/2.0)+(sin(angle2)*offset),
    //     line().y1()+(line().dy()/2.0)+(cos(angle2)*offset));
    // offset = 20;

    offset = (total-1)*-5+(number-1)*10;

    QPointF c2 = half + QPointF(sin(angle + Pi) * offset,
            cos(angle + Pi) * offset);
    // QPointF c2 = half - QPointF(sin(angle + Pi) * offset,
    //     cos(angle + Pi) * offset);
    path.quadTo(c2, line().p2());
    // path.lineTo(c2);
    // use path to work out angle
    QLineF l(line().p1(), c2);
    double angle3 = ::acos(l.dx() / l.length());
    angle = angle3;
    if (l.dy() >= 0)
        angle = (Pi * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) *
                arrowSize, cos(angle + Pi / 3) * arrowSize);
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) *
                arrowSize, cos(angle + Pi - Pi / 3) * arrowSize);

        /* The clear() function for QVector has a
         * compile warning under Qt 4.5 */
        #if QT_VERSION >= 0x040600  // If Qt version is 4.6 or higher
        arrowHead.clear();
        #else
        arrowHead = QPolygonF();
        #endif

        arrowHead << line().p1();
        if (showHead) arrowHead << arrowP1 << arrowP2;

        // painter->drawLine(line());
        painter->drawPath(path);
        if (foreign) painter->setBrush(Qt::gray);
        else
            painter->setBrush(isSelected() ? Qt::red : Qt::black);
        if (isCommunication)
            painter->setBrush(isSelected() ? Qt::darkGreen : Qt::green);
        painter->drawPolygon(arrowHead);

        /*if(myTransition != 0)
        {
            if(myTransition->condition().enabled == true)
            {
                painter->drawText(half, myTransition->condition().toString());
            }
        }*/
}
