#include <QtGui>

#include "machinescene.h"

MachineScene::MachineScene(MachineModel * m, QObject *parent): QGraphicsScene(parent)
{
    myMode = MoveItem; //InsertItem;
    line = 0;
    num_states = 0;
    num_transitions = 0;
    machine = m;
}

void MachineScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != 0)
    {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == StateItem::Type &&
            endItems.first()->type() == StateItem::Type &&
            startItems.first() != endItems.first())
        {
            StateItem *startItem =
                qgraphicsitem_cast<StateItem *>(startItems.first());
            StateItem *endItem =
                qgraphicsitem_cast<StateItem *>(endItems.first());

            //qDebug() << startItem->getName() << " -> " << endItem->getName();

            Arrow *arrow = new Arrow(startItem, endItem);
            arrow->setColor(Qt::black);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            QString s = "transition_";
            s.append(QString("%1").arg(num_transitions));
            num_transitions++;
            arrow->setName(s);
            //machine->addTransition(arrow);
            addItem(arrow);
            arrow->updatePosition();
            emit( this->addedTransition(startItem->getName(), s, endItem->getName()) );
        }
    }

    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void MachineScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != 0)
    {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);

        /*QGraphicsItem * qitem = itemAt(mouseEvent->scenePos());
        if (qitem == 0)
        {
            StateItem * state = qgraphicsitem_cast<StateItem *>(qitem);
            state->setSelected(true);
            state->update();
        }*/
    }
    else
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void MachineScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsItem * qitem = itemAt(mouseEvent->scenePos());

    /* If left mouse click proceed with event,
     * which is passed to scene items to select/move them */
    if (mouseEvent->button() == Qt::LeftButton)
    {
        if(qitem != 0)
        {
            qitem->setSelected(true);
        }

        QGraphicsScene::mousePressEvent(mouseEvent);
    }

    /* If right mouse button add state or start line */
    if (mouseEvent->button() == Qt::RightButton)
    {
        //qitem = itemAt(mouseEvent->scenePos());
        /* If there is no item at mouse position */
        if (qitem == 0)
        {
            /* Create new state, add to scene, and set position */
            StateItem *state = new StateItem;
            QString s;
            if(num_states == 0)
            {
                s.append("initial");
                state->setStartState(true);
            }
            else
            {
                s.append("state_");
                s.append(QString("%1").arg(num_states));
            }
            num_states++;
            state->setName(s);
            addItem(state);
            state->setPos(mouseEvent->scenePos());
            //machine->addState(state);
            emit( this->addedState(s, state->getStartState()) );
        }
        else
        {
            //StateItem * state = qgraphicsitem_cast<StateItem *>(qitem);
            //state->setSelected(true);
            //state->update();

            /* If graphics item is a state */
            if (qgraphicsitem_cast<StateItem *>(qitem))
            {
                //qDebug() << "state clicked";

                StateItem * sitem = qgraphicsitem_cast<StateItem *>(qitem);

                int count = 0;
                for(int i = 0; i < sitem->getTransitions().size(); i ++)
                {
                    Arrow * a = sitem->getTransitions().at(i);
                    if(a->startItem() == sitem) count++;
                }

                if(count < 2)
                {
                    line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                                mouseEvent->scenePos()));
                    line->setPen(QPen(Qt::black, 1));
                    addItem(line);
                }
                else
                {
                    emit( updateStatus("States cannot have more than 2 outgoing transitions") );
                }
            }
            //else qDebug() << "arrow clicked";
        }
    }
}

void MachineScene::runLayoutEngine()
{
    bool colliding = true;

    while(colliding)
    {
        colliding = false;

        for(int i = 0; i < items().size(); i++)
        {
            if (qgraphicsitem_cast<StateItem *>(items().at(i)))
            {
                StateItem * sitem1 = qgraphicsitem_cast<StateItem *>(items().at(i));

                for(int j = 0; j < sitem1->collidingItems().size(); j++)
                {
                    if (qgraphicsitem_cast<StateItem *>(sitem1->collidingItems().at(j)))
                    {
                        StateItem * sitem2 = qgraphicsitem_cast<StateItem *>(sitem1->collidingItems().at(j));

                        /* Move items away from each other */
                        sitem1->moveBy(30.0-(sitem1->x() - sitem2->x()), 30.0-(sitem1->y() - sitem2->y()));
                        //qDebug() << sitem1->getName() << " : " << sitem1->x() << " " << sitem1->y();

                        colliding = true;
                    }
                }
            }
        }
    }
}

float myRandom()
{
    return rand()/((double)RAND_MAX + 1) - 0.5;
}

/*void MachineScene::addStateString(QString n, bool s)
{

}*/

void MachineScene::addTransitionString(QString csn, QString n, QString nsn)
{
    StateItem * cs = 0;
    StateItem * ns = 0;

    for(int j = 0; j < items().size(); j++)
    {
        if (qgraphicsitem_cast<StateItem *>(items().at(j)))
        {
            StateItem * sitem1 = qgraphicsitem_cast<StateItem *>(items().at(j));

            if(sitem1->getName() == csn)
                cs = sitem1;
            if(sitem1->getName() == nsn)
                ns = sitem1;
        }
    }

    //Q_ASSERT(cs != 0);
    //Q_ASSERT(ns != 0);
    if(cs == 0)
    {
        StateItem *state = new StateItem;
        state->setName(csn);
        cs = state;
        addState(state);
    }
    if(ns == 0)
    {
        StateItem *state = new StateItem;
        state->setName(nsn);
        ns = state;
        addState(state);
    }

    Arrow *arrow = new Arrow(cs, ns);
    cs->addArrow(arrow);
    ns->addArrow(arrow);
    num_transitions++;
    arrow->setName(n);
    addTransition(arrow);
}

void MachineScene::addState(StateItem * s)
{
    addItem(s);
    s->setPos(width()/2 + 10.0*myRandom(), height()/2 + 10.0*myRandom());
    if(num_states == 0) s->setStartState(true);
    num_states++;
    runLayoutEngine();
}

void MachineScene::addTransition(Arrow * t)
{
    addItem(t);
    t->setColor(Qt::black);
    t->setZValue(-1000.0);
    t->updatePosition();
}

void MachineScene::updateStateName(QString oldName, QString newName)
{
    for(int i = 0; i < items().size(); i++)
    {
        if (qgraphicsitem_cast<StateItem *>(items().at(i)))
        {
            StateItem * sitem1 = qgraphicsitem_cast<StateItem *>(items().at(i));

            if(sitem1->getName() == oldName)
            {
                //qDebug() << "updateStateName " << oldName << " " << newName;
                sitem1->setName(newName);
            }
        }
    }
    this->update();
}

/*void MachineScene::setMachineOld(MachineModel * m)
{
    clear();
    machine = m;
    line = 0;
    num_states = 0;
    num_transitions = 0;

    for(int i = 0; i < machine->getStates().size(); i++)
    {
        StateItem *state = new StateItem;
        state->setName(machine->getStates().at(i)->name());
        addState(state);
    }
    for(int i = 0; i < machine->getTransitions().size(); i++)
    {
        StateItem * cs = 0;
        StateItem * ns = 0;

        for(int j = 0; j < items().size(); j++)
        {
            if (qgraphicsitem_cast<StateItem *>(items().at(j)))
            {
                StateItem * sitem1 = qgraphicsitem_cast<StateItem *>(items().at(j));

                if(sitem1->getName() == machine->getTransitions().at(i)->currentState()->name())
                    cs = sitem1;
                if(sitem1->getName() == machine->getTransitions().at(i)->nextState()->name())
                    ns = sitem1;
            }
        }

        Q_ASSERT(cs != 0);
        Q_ASSERT(ns != 0);

        Arrow *arrow = new Arrow(cs, ns);
        cs->addArrow(arrow);
        ns->addArrow(arrow);
        num_transitions++;
        arrow->setName(machine->getTransitions().at(i)->name());
        addTransition(arrow);
    }
    runLayoutEngine();
}*/

void MachineScene::selectState(QString n)
{
    clearSelection();

    for(int j = 0; j < items().size(); j++)
    {
        if (qgraphicsitem_cast<StateItem *>(items().at(j)))
        {
            StateItem * sitem1 = qgraphicsitem_cast<StateItem *>(items().at(j));

            if(sitem1->getName() == n)
            {
                sitem1->setSelected(true);
                //qDebug() << "transition selected: " << n;
            }
        }
    }
    update();
}

void MachineScene::selectTransition(QString n)
{
    clearSelection();

    for(int j = 0; j < items().size(); j++)
    {
        if (qgraphicsitem_cast<Arrow *>(items().at(j)))
        {
            Arrow * a = qgraphicsitem_cast<Arrow *>(items().at(j));

            if(a->getName() == n)
            {
                a->setSelected(true);
                //qDebug() << "state selected: " << n;
            }
        }
    }
    update();
}
