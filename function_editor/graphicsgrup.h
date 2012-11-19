#ifndef GRAPHICSGRUP_H
#define GRAPHICSGRUP_H

#include "./fegraphicsitem.h"
#include <QList>
#include <QRectF>
#include <QTransform>

class GraphicsGrup
{
public:
    GraphicsGrup();
    GraphicsGrup(bool);
    GraphicsGrup(FEGraphicsItem *item);
    ~GraphicsGrup();

    QRectF boundingRect() const { return myBoundingRect;}

    void add(FEGraphicsItem *item);
    void add(GraphicsGrup &grupLeft, GraphicsGrup &grupRight);
    void add(GraphicsGrup &grupRight);
    void finalArrangement();
    void finalArrangement(QList<QRectF> &boundingList);

    FEGraphicsItem* item() const
    {
        if(graphicsList->count() > 0)
            return graphicsList->last();
        else
            return 0;
    }

    void move(qreal x, qreal y);

private:
    QList<FEGraphicsItem*> *graphicsList;
    QRectF myBoundingRect;
    qreal gap;
    bool sub;

    QList<GraphicsGrup*> grups;
    QList<int> positionItem;
};

#endif // GRAPHICSGRUP_H
