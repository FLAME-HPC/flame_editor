#include "graphicsgrup.h"
#include <QDebug>

GraphicsGrup::GraphicsGrup()
{
    graphicsList = new QList<FEGraphicsItem*>;
    gap = 20;
    sub = false;
}

GraphicsGrup::GraphicsGrup(bool b)
{
    graphicsList = new QList<FEGraphicsItem*>;
    gap = 20;
    sub = true;
}

GraphicsGrup::GraphicsGrup(FEGraphicsItem *item)
{
    graphicsList = new QList<FEGraphicsItem*>;
    gap = 20;
    add(item);
}

GraphicsGrup::~GraphicsGrup()
{
    finalArrangement();
    graphicsList->clear();
    delete graphicsList;
}

void GraphicsGrup::add(FEGraphicsItem *item)
{
    qreal gapy = gap;
    if(graphicsList->count() > 0 && graphicsList->last()->mytype == EmptyState)
        gapy = -15;
    if(myBoundingRect.isNull())
    {
        if(sub)
        {
            item->setPos(0, 0);
            sub = false;
        }
        QRectF bounding = item->boundingRect();
        QPointF p = item->pos();
        myBoundingRect = QRectF(p.x() - bounding.width() / 2.0, p.y() - bounding.height() / 2.0, bounding.width(), bounding.height());
        graphicsList->append(item);
    }
    else
    {
        //item->setPos(0, 0);
        QRectF boundingItem = graphicsList->last()->boundingRect();
        QRectF bounding = item->boundingRect();
        QPointF p = item->pos();
        bounding = QRectF(0, 0, bounding.width(), bounding.height());
        //qDebug()<<bounding<<p;
        qreal x, y, xd, yd;
        x = myBoundingRect.x() + myBoundingRect.width() / 2.0;
        y = myBoundingRect.bottom() + gapy + bounding.height();
        bounding.moveTopLeft(QPointF(x - bounding.width() / 2.0, y - bounding.height() / 2.0 + gapy));
        xd = x - bounding.x();
        yd = y - bounding.y();
        item->setPos(x, y);
        graphicsList->append(item);
        myBoundingRect = myBoundingRect.unite(bounding);
        //qDebug()<<bounding;
    }
    //qDebug()<<myBoundingRect.bottom()<<graphicsList->count();
}

void GraphicsGrup::add(GraphicsGrup &grupLeft, GraphicsGrup &grupRight)
{
    qreal gapy = graphicsList->last()->boundingRect().width() / 2.0;
    qreal x, y, x1, y1, x2, y2;
    x = myBoundingRect.x() + myBoundingRect.width() / 2.0;
    y = myBoundingRect.bottom() + gap;
    QRectF r1 = grupLeft.boundingRect();
    QRectF r2 = grupRight.boundingRect();
    qreal yLeft = r1.height() < r2.height()?(r2.height()-r1.height())/2.0:0;
    qreal yRight = r2.height() < r1.height()?(r1.height()-r2.height())/2.0:0;
    qDebug()<<myBoundingRect.bottom();
    x1 = x - r1.right() - gapy;
    y1 = y + grupLeft.graphicsList->first()->boundingRect().height() * 2 + yLeft;
    x2 = x + gapy;
    y2 = y + grupRight.graphicsList->first()->boundingRect().height() * 2 + yRight;
    grupLeft.move(x1 - r1.x(), y1 - r1.y());
    grupRight.move(x2 - r2.x(), y2 - r2.y());
    r1 = grupLeft.boundingRect();
    r2 = grupRight.boundingRect();
    //qDebug()<<r1;
    myBoundingRect = myBoundingRect.unite(r1);
    myBoundingRect = myBoundingRect.unite(r2);

    while(grupLeft.graphicsList->count() > 0)
    {
        graphicsList->append(grupLeft.graphicsList->value(0));
        grupLeft.graphicsList->removeAt(0);
    }

    while(grupRight.graphicsList->count() > 0)
    {
        graphicsList->append(grupRight.graphicsList->value(0));
        grupRight.graphicsList->removeAt(0);
    }
}

void GraphicsGrup::add(GraphicsGrup &grupRight)
{
    grups.append(&grupRight);
    positionItem.append(graphicsList->count() - 1);
}

void GraphicsGrup::finalArrangement()
{
    if(grups.count() > 0)
    {
        int i = positionItem.last();
        positionItem.removeLast();
        qreal t, x, y, x1, y1;
        x = graphicsList->at(i)->boundingRect().right();
        for(int ii = i + 1;ii<graphicsList->count();ii++)
        {
            t = graphicsList->at(ii)->boundingRect().right();
            if(t > x)
                x = t;
        }
        y = graphicsList->at(i)->pos().y();
        QRectF r = grups.last()->boundingRect();
        x1 = x + gap + myBoundingRect.right();
        y1 = y + 0*grups.last()->graphicsList->first()->boundingRect().height() * 2;
        GraphicsGrup *grup = grups.last();
        grups.removeLast();
        grup->move(x1 - r.x(), y1 - r.y());
        r = grup->boundingRect();
        while(grup->graphicsList->count() > 0)
        {
            graphicsList->append(grup->graphicsList->value(0));
            grup->graphicsList->removeAt(0);
        }
        delete grup;
        QList<QRectF> boundingList;
        boundingList.append(r);
        finalArrangement(boundingList);
        while(boundingList.count() > 0)
        {
            myBoundingRect = myBoundingRect.unite(boundingList.last());
            boundingList.removeLast();
        }
    }
}

void GraphicsGrup::finalArrangement(QList<QRectF> &boundingList)
{
    while(grups.count() > 0)
    {
        int i = positionItem.last();
        positionItem.removeLast();
        qreal t, x, y, x1, y1;
        x = boundingList.first().right();
        y = graphicsList->at(i)->pos().y();
        GraphicsGrup *grup = grups.last();
        grups.removeLast();
        QRectF r = grup->boundingRect();
        x1 = x + gap + myBoundingRect.right()*0;
        y1 = y + 0*grup->graphicsList->first()->boundingRect().height() * 2;
        grup->move(x1 - r.x(), y1 - r.y());
        r = grup->boundingRect();
        boundingList.insert(0, r);
        while(grup->graphicsList->count() > 0)
        {
            graphicsList->append(grup->graphicsList->value(0));
            grup->graphicsList->removeAt(0);
        }
        delete grup;
    }
}

void GraphicsGrup::move(qreal x, qreal y)
{
    QPointF p(x, y);
    for(int i = 0;i < graphicsList->count();i++)
    {
        FEGraphicsItem *g = graphicsList->value(i);
        g->setPos(g->pos() + p);
    }
    myBoundingRect.setTopLeft(myBoundingRect.topLeft() + p);
    myBoundingRect.setBottomRight(myBoundingRect.bottomRight() + p);
}
