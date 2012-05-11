/*!
 * \file machinescene.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of the graphics Scene for the stategraph
 */
#include <QtGui>
#include "./machinescene.h"
//#include "./machine.h"
#include "./codedialog.h"

MachineScene::MachineScene(QObject *parent)
    : QGraphicsScene(parent) {
    /* Initialise variables */
    codeDialog = parent;
    line = 0;
    itemToMove = 0;
    itemToColaps = 0;
    itemSelect = 0;
    num_states = 0;
    num_transitions = 0;

    /* Set a size for the scene */
    setSceneRect(0, 0, 500, 500);

    /* Add the start state */
    /* Create GraphicsItem object */
    GraphicsItem * state = new GraphicsItem;
    /* Add it to the scene */
    addState(state);
    //state->setToolTip(tr("po"));
    //state->setName(tr("9i9o"));
    /* Set the position in the scene */
    state->setPos(250.0, 210.0);

    //state = new GraphicsItem;
    //addState(state);
    //state->setPos(250.0, 250.0);
}

void MachineScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    /* Initialise pointers to any graphics item at the
     * start or end of the drawing line when using the mouse */
    GraphicsItem * startItem = 0;
    GraphicsItem * endItem = 0;

    qDebug()<<(itemToColaps !=0 )<< (itemToMove != 0);
    if(itemToMove != 0 && itemToColaps != 0){
        if(itemToMove->getGraphicsItemParent() != 0){
            Arrow * arrow = qgraphicsitem_cast<Arrow*>( itemToMove->getGraphicsItemParent()->getGraphicsItem());
            arrow->setEndItem(itemToColaps);
            arrow->updatePosition();
        }
        removeItem(itemToMove);
        delete itemToMove;
        itemToMove = 0;
        itemToColaps->setColor((QColor)Qt::black);
        itemToColaps = 0;
        invalidate();
    }
    if(itemToMove != 0){
        itemToMove = 0;
    }

    /* If there exists a line being drawn */
    if (line != 0) {
        /* Create a list of graphics items at the start and end points
         * of the line */
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        /* Remove and delete line as no longer needed */
        removeItem(line);
        delete line;
        line = 0;

        /* Make startItem and endItem equal to any graphics items at
         * the start and end points of the line */
        if (startItems.count() > 0 &&
                startItems.first()->type() == GraphicsItem::Type)
            startItem = qgraphicsitem_cast<GraphicsItem *>(startItems.first());
        if (endItems.count() > 0 &&
                endItems.first()->type() == GraphicsItem::Type)
            endItem = qgraphicsitem_cast<GraphicsItem *>(endItems.first());

        /* If the startItem is a state and the endItem is nothing then
         * create a new state at the end of the line and assign to endItem */
        if (startItem->mytype == 0 && endItem == 0) {
            /* Create GraphicsItem object */
        }

        /* If the startItem and endItem are not null and are not
         * the same graphics items (i.e. not the same state)
         * (This is wrong as a transition can go back to the same state) */
        if (startItem != 0 && endItem == 0) {
                if(startItem->countGraphicsItem() == 0 ) {
                    /* Create a transition graphics item */
                    /* Create GraphicsItem object */
                    GraphicsItem * state = new GraphicsItem;
                    /* Add it to the scene */
                    addState(state);
                    /* Set the position in the scene */
                    state->setPos(mouseEvent->scenePos());
                    /* Make the endItem equal to the new state */
                    endItem = state;
                    GraphicsItem * transition = new GraphicsItem;
                    /* Add it to the scene */
                    addTransition(transition);
                    /* Set the position in the scene */
                    /* The position is half way between the states */
                    transition->setPos((startItem->x() + state->x())/2.0,
                                       (startItem->y() + state->y())/2.0);

                    transition->setGraphicsItemParent(startItem);
                    state->setGraphicsItemParent(transition);

                    /* Create an arrow pointer to add new arrows */
                    Arrow * arrow;
                    /* Add an arrow from the current state to the function */
                    arrow = new Arrow(startItem, transition);
                    //arrow->drawHead(false); /* Don't draw the arrow head */
                    addArrow(arrow);
                    arrow->updatePosition();
                    startItem->addGraphicsItem(arrow);
                    /* Add an arrow from the function to the next state */
                    arrow = new Arrow(transition, state);
                    addArrow(arrow);
                    arrow->updatePosition();
                    transition->addGraphicsItem(arrow);
                } else
                if(startItem->countGraphicsItem() == 1 && startItem->isToDiamond() == false){
                    GraphicsItem * state = new GraphicsItem;
                    /* Add it to the scene */
                    addState(state);
                    /* Set the position in the scene */
                    state->setPos(mouseEvent->scenePos());
                    /* Make the endItem equal to the new state */
                    endItem = state;
                    Arrow * arrow1 = qgraphicsitem_cast<Arrow*>( startItem->getGraphicsItem());
                    startItem->removeGraphicsItem(arrow1);
                    startItem->setToDiamond();
                    GraphicsItem * diamond = new GraphicsItem;
                    addDiamond(diamond);
                    arrow1->setStartItem(diamond);
                    arrow1->updatePosition();
                    diamond->setPos(startItem->x(), startItem->y() + startItem->width() + 10);
                    GraphicsItem * tr = arrow1->getEndItem();
                    tr->setGraphicsItemParent(diamond);
                    diamond->setGraphicsItemParent(startItem);
                    Arrow * arrow = new Arrow(startItem, diamond);
                    addArrow(arrow);
                    arrow->updatePosition();
                    startItem->addGraphicsItem(arrow);
                    diamond->addGraphicsItem(arrow1);

                    GraphicsItem * transition = new GraphicsItem;
                    /* Add it to the scene */
                    addTransition(transition);
                    /* Set the position in the scene */
                    /* The position is half way between the states */
                    transition->setPos((diamond->x() + state->x())/2.0,
                                       (diamond->y() + state->y())/2.0);
                    transition->setGraphicsItemParent(diamond);
                    state->setGraphicsItemParent(transition);
                    arrow = new Arrow(diamond, transition);
                    addArrow(arrow);
                    arrow->updatePosition();
                    diamond->addGraphicsItem(arrow);

                    arrow = new Arrow(transition, state);
                    addArrow(arrow);
                    arrow->updatePosition();
                    transition->addGraphicsItem(arrow);
                } else {
                    QMessageBox::warning(qobject_cast<QWidget*>( this->parent()), tr(""), tr("Not allowed to add a new transition!"));
                }
        }
    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void MachineScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    /* If a line is being drawn (i.e. right mouse button down */
    if (line != 0) {
        /* Redraw the line to the new mouse position */
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else {
        if(itemToMove != 0 && itemToMove->mytype == 0){
            itemToMove->setPos(mouseEvent->scenePos());
            QPointF p = mouseEvent->scenePos();
            QRectF r = itemToMove->boundingRect();
            qDebug()<<r;
            r = QRectF(p.x() - r.width()/2.0, p.y() - r.height()/2.0, r.width(), r.height());
            QList<QGraphicsItem *> itemList = items(r, Qt::IntersectsItemBoundingRect);
            itemList.removeOne(itemToMove);
            qDebug()<<"opo0"<<r<<p;
            GraphicsItem * it = 0;
            bool b = false;
            for(int i = 0;i < itemList.count();i++){
                if (qgraphicsitem_cast<GraphicsItem *>(itemList[i]) &&
                        qgraphicsitem_cast<GraphicsItem *>(itemList[i])->mytype == 0) {
                    it = qgraphicsitem_cast<GraphicsItem *>(itemList[i]);
                        if(it == itemToColaps){
                            it = 0;
                            b = true;
                        }
                }
            }
            if(it != 0){
                if(itemToColaps != 0){
                    itemToColaps->setColor((QColor)Qt::black);
                }
                itemToColaps = it;
                itemToColaps->setColor((QColor)Qt::green);
                invalidate();
            } else if(!b && itemToColaps != 0){
                itemToColaps->setColor((QColor)Qt::black);
                itemToColaps = 0;
                invalidate();
            }
            qDebug()<<(itemToColaps !=0 )<< (itemToMove != 0);
        } else
            if(itemToMove != 0){
                itemToMove->setPos(mouseEvent->scenePos());
            }
        //QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void MachineScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    /* Make qitem equal to any graphics item at the mouse position */
    QGraphicsItem * qitem = itemAt(mouseEvent->scenePos());
    CodeDialog *cd = qobject_cast<CodeDialog *>(codeDialog);

    /* If left mouse click proceed with event,
     * which is passed to scene items to select/move them */
    if (mouseEvent->button() == Qt::LeftButton) {
        /* If qitem is not null then make it selected */
        if (qitem != 0) {
            clearSelection();
            qitem->setSelected(true);
            qDebug()<<"ok";
            if (qgraphicsitem_cast<GraphicsItem *>(qitem)) {
                itemToMove =
                qgraphicsitem_cast<GraphicsItem *>(qitem);
                if(itemToMove->mytype == 1){
                    if(itemSelect == 0){
                        itemSelect = itemToMove;
                        cd->setShowList(true);
                    } else if(itemToMove != itemSelect){
                        itemSelect = itemToMove;
                    }
                } else {
                    if(cd->getShowList()){
                        cd->setShowList(false);
                    }
                    itemSelect = 0;
                }
            }

        }
        else if(itemSelect != 0){
            if(cd->getShowList()){
                cd->setShowList(false);
            }
            itemSelect = 0;
        }

        QGraphicsScene::mousePressEvent(mouseEvent);
    }

    /* If right mouse button start line */
    if (mouseEvent->button() == Qt::RightButton) {
        /* If there is an item at mouse position */
        if (qitem != 0) {
            /* If graphics item is a GraphicsItem */
            if (qgraphicsitem_cast<GraphicsItem *>(qitem)) {
                GraphicsItem * sitem =
                qgraphicsitem_cast<GraphicsItem *>(qitem);

                /* If the graphics item is a state */
                if(sitem->mytype==0) {
                    /* Create a line */
                    line = new QGraphicsLineItem(
                            QLineF(mouseEvent->scenePos(),
                                    mouseEvent->scenePos()));
                    line->setPen(QPen(Qt::black, 1));
                    addItem(line);
                }
            }
        }
    }
}

void MachineScene::addState(GraphicsItem * s) {
    s->setState(QString("state %1").arg(num_states));
    addItem(s);
    num_states++;
}

void MachineScene::addTransition(GraphicsItem * t) {
    t->setTransition(QString("transition %1").arg(num_transitions));
    addItem(t);
    num_transitions++;
}

void MachineScene::addDiamond(GraphicsItem *t)
{
    t->setDiamond(QString("condition %1").arg(num_transitions));
    addItem(t);
    num_transitions++;
}

void MachineScene::addArrow(Arrow * t) {
    addItem(t);
    /* Draw arrows underneath states and functions */
    t->setZValue(-1.0);
}
