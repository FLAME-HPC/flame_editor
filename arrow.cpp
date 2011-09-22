#include <QtGui>

#include "arrow.h"
#include <math.h>

const qreal Pi = 3.14;

Arrow::Arrow(StateItem *startItem, StateItem *endItem,
         QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene)
{
    myStartItem = startItem;
    myEndItem = endItem;
    //mpre = new Mpre();
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    myColor = Qt::black;
    setPen(QPen(myColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

void Arrow::setName(QString n)
{
    name = n;
}

void Arrow::setMpre(Mpre m)
{
    mpre = m;

}

void Arrow::setMpost(Mpost mp)
{
    mpost = mp;
}

QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void Arrow::updatePosition()
{
    QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    setLine(line);
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    if (myStartItem->collidesWithItem(myEndItem))
        return;

    QPen myPen = pen();
    myPen.setColor(myColor);
    qreal arrowSize = 10;
    painter->setPen(myPen);
    painter->setBrush(isSelected() ? Qt::red : Qt::black);
    painter->setPen(isSelected() ? Qt::red : Qt::black);
    painter->setRenderHint(QPainter::Antialiasing);

    QLineF centerLine(myStartItem->pos(), myEndItem->pos());
    QPolygonF endPolygon = myEndItem->polygon();
    QPointF p1 = endPolygon.first() + myEndItem->pos();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;
    for (int i = 1; i < endPolygon.count(); ++i) {
    p2 = endPolygon.at(i) + myEndItem->pos();
    polyLine = QLineF(p1, p2);
    QLineF::IntersectType intersectType =
        polyLine.intersect(centerLine, &intersectPoint);
    if (intersectType == QLineF::BoundedIntersection)
        break;
        p1 = p2;
    }

    setLine(QLineF(intersectPoint, myStartItem->pos()));

    double angle = ::acos(line().dx() / line().length());
    if (line().dy() >= 0)
        angle = (Pi * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                        cos(angle + Pi / 3) * arrowSize);
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                        cos(angle + Pi - Pi / 3) * arrowSize);

        arrowHead.clear();
        arrowHead << line().p1() << arrowP1 << arrowP2;

        painter->drawLine(line());
        painter->drawPolygon(arrowHead);
        /*if (isSelected())
        {
            painter->setPen(QPen(myColor, 1, Qt::DashLine));
            QLineF myLine = line();
            myLine.translate(0, 4.0);
            painter->drawLine(myLine);
            myLine.translate(0,-8.0);
            painter->drawLine(myLine);
        }*/
}

/*bool Arrow::passesCondition(MemoryModel* memory)
{
    bool rc;

    if(mpre.enabled() == false) return true;

    // Find variable in memory
    for(int i = 0; i < memory->getNames().size(); i++)
    {
        if(memory->getNames().at(i) == mpre.name())
        {
            if(mpre.op() == "==") rc = (memory->getValues().at(i) == mpre.value());
            if(mpre.op() == "!=") rc = (memory->getValues().at(i) != mpre.value());
            if(mpre.op() == ">") rc = (memory->getValues().at(i) > mpre.value());
            if(mpre.op() == "<") rc = (memory->getValues().at(i) < mpre.value());
            if(mpre.op() == ">=") rc = (memory->getValues().at(i) >= mpre.value());
            if(mpre.op() == "<=") rc = (memory->getValues().at(i) <= mpre.value());

            if(mpre.isNot()) return !rc;
            else return rc;
        }
    }

    return false;
}

void Arrow::updateMemory(MemoryModel* memory)
{
    //qDebug() << "upDateMemory";

    if(mpost.enabled() == false) return;

    // Find variable in memory
    for(int i = 0; i < memory->getNames().size(); i++)
    {
        if(memory->getNames().at(i) == mpost.name())
        {
            //qDebug() << memory->getNames().at(i) << ": " << memory->getValues().at(i);
            if(mpost.op() == "=") memory->replaceIntValue(i, mpost.value());
            if(mpost.op() == "+=") memory->replaceIntValue(i, memory->getValues().at(i) + mpost.value());
            if(mpost.op() == "-=") memory->replaceIntValue(i, memory->getValues().at(i) - mpost.value());
            if(mpost.op() == "*=") memory->replaceIntValue(i, memory->getValues().at(i) * mpost.value());
            if(mpost.op() == "/=") memory->replaceIntValue(i, memory->getValues().at(i) / mpost.value());
            //qDebug() << memory->getNames().at(i) << ": " << memory->getValues().at(i) << " <- " << mpost.value();
        }
    }
}*/
