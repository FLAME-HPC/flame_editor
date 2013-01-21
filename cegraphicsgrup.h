#ifndef CEGRAPHICSGRUP_H
#define CEGRAPHICSGRUP_H

#include "./cegraphicsitem.h"
#include <QList>
#include <QRectF>
#include <QTransform>

class CEGraphicsGrup
{
public:
    CEGraphicsGrup();
    CEGraphicsGrup(bool b);
    CEGraphicsGrup(CEGraphicsItem *item);
    ~CEGraphicsGrup();

    QRectF boundingRect() const { return myBoundingRect;}

    void add(CEGraphicsItem *item);
    void add(CEGraphicsGrup &grupLeft, CEGraphicsGrup &grupRight);
    void add(CEGraphicsGrup &grupRight);
    void finalArrangement();
    void finalArrangement(QList<QRectF> &boundingList);

    CEGraphicsItem* item() const
    {
        if(graphicsList->count() > 0)
            return graphicsList->last();
        else
            return 0;
    }

    void move(qreal x, qreal y);

private:
    QList<CEGraphicsItem*> *graphicsList;
    QRectF myBoundingRect;
    qreal gap;
    bool sub;

    QList<CEGraphicsGrup*> grups;
    QList<int> positionItem;
};

#endif // GRAPHICSGRUP_H
