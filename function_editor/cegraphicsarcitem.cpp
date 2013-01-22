#include "cegraphicsarcitem.h"

CEGraphicsArcItem::CEGraphicsArcItem(QPointF p1, QPointF p2, QGraphicsItem *parent) :
    QGraphicsPathItem(parent)
{
    start = p1;
    stop = p2;
    compute();
}

QRectF CEGraphicsArcItem::boundingRect() const
{
    qreal a = 0.5;
    QRectF r = myPath.boundingRect();
    return r.adjusted(-a,-a,a,a);
}

QPainterPath CEGraphicsArcItem::shape() const
{
    return myPath;
}

void CEGraphicsArcItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::black);
    painter->drawPath(myPath);
    painter->setBrush(Qt::black);
    painter->drawPath(headPath);
}

void CEGraphicsArcItem::compute()
{
    myPath = QPainterPath();
    QPointF p((start.x() + stop.x()) / 2.0, (start.y() + stop.y()) / 2.0);
    double d = Distance(p, start);
    QRectF r(start.x() - d, start.y() - d, d * 2, d * 2);
    double a = Angle(p, stop);
    QPointF p1 = PointOnArc(r, a+30);
    myPath.moveTo(start);
    myPath.quadTo(p1, stop);

    qreal g = 10;
    headPath = QPainterPath();
    QPolygonF po;
    po<<QPointF(-g, -g)<<QPointF(g, -g)<<QPointF(0, 0);
    headPath.addPolygon(po);

    QTransform t;
    t.translate(stop.x(), stop.y());
    t.rotate(a - 90);
    headPath = t.map(headPath);
    //qDebug()<<start<<p1<<stop<<a;
}

double CEGraphicsArcItem::Angle(QPointF o, QPointF p)
{
    float x1, x2, y1, y2;
    x1 = o.x();
    y1 = o.y();
    x2 = p.x();
    y2 = p.y();
    double n = y2 - y1;
    n = n == 0 ? 1 : n;
    double r = ::atan((x2 - x1) / n);
    double d = r * 180 / PI;
    double gr = 0;
    if (x1 <= x2 && y1 == y2)
        gr = 0;
    else
        if (x1 < x2 && y1 < y2)
            gr = 90 - d;
        else
            if (x1 == x2 && y1 <= y2)
                gr = 90;
            else
                if (x1 > x2 && y1 < y2)
                    gr = -d + 90;
                else
                    if (x1 > x2 && y1 == y2)
                        gr = 180;
                    else
                        if (x1 > x2 && y1 > y2)
                            gr = 90 - d + 180;
                        else
                            if (x1 == x2 && y1 > y2)
                                gr = 270;
                            else
                                if (x1 < x2 && y1 > y2)
                                    gr = -d + 270;
    return gr;
}

double CEGraphicsArcItem::Distance(QPointF p1, QPointF p2)
{
    return ::sqrt((p2.x() - p1.x()) * (p2.x() - p1.x()) + (p2.y() - p1.y()) * (p2.y() - p1.y()));
}

QPointF CEGraphicsArcItem::PointOnArc(QRectF rectangleF, double angle)
{
    float Width = rectangleF.width() / 2.0;
    float Height = rectangleF.height() / 2.0;
    float x = rectangleF.x() + Width;
    float y = rectangleF.y() + Height;

    QPointF p = QPointF();
    p.setX((float)(x + Width * ::cos(angle * PI / 180.0)));
    p.setY((float)(y + Height * ::sin(angle * PI / 180.0)));
    return p;
}
