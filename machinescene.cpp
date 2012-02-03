/*!
 * \file machinescene.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of the graphics Scene for the stategraph
 */
#include <QtGui>
#include "./machinescene.h"
#include "./machine.h"

MachineScene::MachineScene(Machine * m, QObject *parent)
    : QGraphicsScene(parent) {
    myMode = MoveItem;  // InsertItem;
    line = 0;
    num_states = 0;
    num_transitions = 0;
    num_messages = 0;
    rootMachine = m;
    mytype = 0;
    zoomOn = false;
    myGraphicsView = 0;
    scaleFactor = 1.15;  // How fast we zoom
    selectedFunction = 0;
}

void MachineScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    GraphicsItem * startItem = 0;
    GraphicsItem * endItem = 0;

    if (line != 0) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;

        if (startItems.count() > 0 &&
                startItems.first()->type() == GraphicsItem::Type)
            startItem = qgraphicsitem_cast<GraphicsItem *>(startItems.first());
        if (endItems.count() > 0 &&
                endItems.first()->type() == GraphicsItem::Type)
            endItem = qgraphicsitem_cast<GraphicsItem *>(endItems.first());

        // If start = state and no end item
        if (startItem->mytype == 0 && endItem == 0) {
            QString s;
            s.append("state_");
            s.append(QString("%1").arg(num_states));
            num_states++;

            State * state = machineModel->addState(s);

            endItem = new GraphicsItem;
            endItem->setState(state);
            endItem->agentName = startItem->agentName;
            addItem(endItem);
            endItem->setPos(mouseEvent->scenePos());
            // emit( this->addedState(s) );
            statesAndTransitions.append(endItem);
        }
        // If start = transition and no end item
        if (startItem->mytype == 1 && endItem == 0) {
            /*endItem = new GraphicsItem;
            QString s;
            s.append("message_");
            s.append(QString("%1").arg(num_messages));
            num_messages++;
            endItem->mytype = 2;
            endItem->setName(s);
            //endItem->agentName = startItem->agentName;
            addItem(endItem);
            endItem->setPos(mouseEvent->scenePos());
            messages.append(endItem);*/
        }

        if (startItem != 0 && endItem != 0 && startItem != endItem) {
            // If both states from same agent
            if (startItem->mytype == 0 && endItem->mytype == 0 &&
                    startItem->agentName == endItem->agentName) {
                startItem->foreign = false; startItem->update();
                endItem->foreign = false; endItem->update();

                QString s = "transition_";
                s.append(QString("%1").arg(num_transitions));

                Transition * t = machineModel->addTransition(s,
                        startItem->state(), endItem->state());

                GraphicsItem * fitem = new GraphicsItem;
                fitem->setTransition(t);
                fitem->agentName = startItem->agentName;
                addTransition(fitem);
                fitem->setPos((startItem->x() + endItem->x())/2.0,
                        (startItem->y() + endItem->y())/2.0);

                Arrow * arrow = new Arrow(startItem, fitem);
                startItem->addTransitionArrow(arrow);
                fitem->addTransitionArrow(arrow);
                arrow->drawHead(false);
                addArrow(arrow);
                arrow->updatePosition();

                arrow = new Arrow(fitem, endItem);
                fitem->addTransitionArrow(arrow);
                endItem->addTransitionArrow(arrow);
                addArrow(arrow);
                arrow->updatePosition();

                // emit( this->addedTransition(fitem) );
            }

            // If start = transition and end = message
            if (startItem->mytype == 1 && endItem->mytype == 2) {
                Arrow * arrow = new Arrow(startItem, endItem);
                startItem->addMessageArrow(arrow);
                endItem->addMessageArrow(arrow);
                arrow->isCommunication = true;
                addArrow(arrow);
                arrow->updatePosition();

                machineModel->addMessageToTransition(startItem->transition,
                        false, endItem->getName());
            }

            // If start = message and end = transition
            if (startItem->mytype == 2 && endItem->mytype == 1) {
                Arrow * arrow = new Arrow(startItem, endItem);
                startItem->addMessageArrow(arrow);
                endItem->addMessageArrow(arrow);
                arrow->isCommunication = true;
                addArrow(arrow);
                arrow->updatePosition();

                machineModel->addMessageToTransition(
                        endItem->transition, true, startItem->getName());
            }
        }
    }

    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void MachineScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (line != 0) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);

        /*QGraphicsItem * qitem = itemAt(mouseEvent->scenePos());
        if (qitem == 0)
        {
            GraphicsItem * state = qgraphicsitem_cast<GraphicsItem *>(qitem);
            state->setSelected(true);
            state->update();
        }*/
    } else {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void MachineScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    selectedFunction = 0;
    emit(functionSelected(false));

    QGraphicsItem * qitem = itemAt(mouseEvent->scenePos());

    /* If left mouse click proceed with event,
     * which is passed to scene items to select/move them */
    if (mouseEvent->button() == Qt::LeftButton) {
        if (qitem != 0) {
            clearSelection();
            qitem->setSelected(true);

            if (qgraphicsitem_cast<GraphicsItem *>(qitem)) {
                GraphicsItem * sitem =
                        qgraphicsitem_cast<GraphicsItem *>(qitem);
                // if function and not foreign
                if (sitem->mytype == 1 && !sitem->foreign) {
                    selectedFunction = sitem;
                    emit(functionSelected(true));
                }
            }
        }

        QGraphicsScene::mousePressEvent(mouseEvent);
    }

    /* If right mouse button add state or start line */
    if (mouseEvent->button() == Qt::RightButton) {
        if (rootMachine->type == 1) {  // agent
            // qitem = itemAt(mouseEvent->scenePos());
            /* If there is no item at mouse position */
            if (qitem == 0) {
                /* Create new state, add to scene, and set position */
    /*            GraphicsItem *state = new GraphicsItem;
                QString s;

                s.append("state_");
                s.append(QString("%1").arg(num_states));
                num_states++;
                state->setName(s);
                addItem(state);
                state->setPos(mouseEvent->scenePos());
                //machine->addState(state);
                emit( this->addedState(s, state->getStartState()) );
    */
            } else {
                // GraphicsItem * state = qgraphicsitem_cast<GraphicsItem *>
                // (qitem);
                // state->setSelected(true);
                // state->update();

                /* If graphics item is a state */
                if (qgraphicsitem_cast<GraphicsItem *>(qitem)) {
                    GraphicsItem * sitem =
                            qgraphicsitem_cast<GraphicsItem *>(qitem);

                    // if item is a state
                    // if(sitem->mytype==0)
                    // {
                        int count = 0;
                        for (int i = 0; i < sitem->
                        getTransitionArrows().size(); i ++) {
                            Arrow * a = sitem->getTransitionArrows().at(i);
                            if (a->startItem() == sitem) count++;
                        }

                        // if(count < 2)
                        // {
                            line = new QGraphicsLineItem(
                                    QLineF(mouseEvent->scenePos(),
                                            mouseEvent->scenePos()));
                            line->setPen(QPen(Qt::black, 1));
                            addItem(line);
                        // }
                        // else
                        // {
                        //    emit( updateStatus(
            //    "States cannot have more than 2 outgoing transitions") );
                        // }
                    // }
                }
                // else qDebug() << "arrow clicked";
            }
        }
    }
}

void MachineScene::wheelEvent(QGraphicsSceneWheelEvent *event) {
    // Get the position of the mouse before scaling, in scene coords
//    QPointF pointBeforeScale(mapToScene(event->pos()));

    // Get the original screen centerpoint
//    QPointF screenCenter = GetCenter(); //CurrentCenterPoint;
    // (visRect.center());

    // Scale the view ie. do the zoom
    if (zoomOn) {
        if (event->delta() > 0) {
            zoomIn();
        } else {
            zoomOut();
        }
    }

    // Get the position after scaling, in scene coords
//    QPointF pointAfterScale(mapToScene(event->pos()));

    // Get the offset of how the screen moved
//    QPointF offset = pointBeforeScale - pointAfterScale;

    // Adjust to the new center for correct zooming
//    QPointF newCenter = screenCenter + offset;
//    SetCenter(newCenter);
}

void MachineScene::zoomIn() {
    if (myGraphicsView) myGraphicsView->scale(scaleFactor, scaleFactor);
}

void MachineScene::zoomOut() {
    if (myGraphicsView) myGraphicsView->
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
}

void MachineScene::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Z:
            zoomOn = true;
            break;
        case Qt::Key_Minus:
            zoomOut();
            break;
        case Qt::Key_Equal:
            zoomIn();
            break;
        default:
            event->ignore();
            break;
    }
}

void MachineScene::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Z:
            zoomOn = false;
            break;
        default:
            event->ignore();
            break;
    }
}

void calcState(GraphicsItem * sitem, QList<int> * layerWidth, int layerHeight) {
    // Set state position, width after the current layer width
    sitem->setPos(layerWidth->at(sitem->layer)+(sitem->width()/2.0),
            20.0 + (sitem->layer*layerHeight));
    // Update the layer width to be after the current state
    layerWidth->replace(sitem->layer,
            (layerWidth->at(sitem->layer) + sitem->width()+10));
    // Set layered flag to be 1
    sitem->layered = 1;

    /*for(int j = 0; j < sitem->getTransitions().size(); j++)
    {
        Arrow * a = sitem->getTransitions().at(j);

        if(sitem->layered < 0) calcState(a->endItem(), layerWidth);
    }*/
}

void MachineScene::calcLayers() {
    // WARNING: does not currently handle loops

    selectedFunction = 0;
    int done;
    int noLayers;
    int noStatesInLayer;
    int largestWidth;
    int globalLargestWidth = 0;
    int globalHighestLayer = 0;
    int layerHeight = 80;
    QList<int> layerWidth;
    QList< QList<GraphicsItem *> > statesLayer;
    QList<int> agentWidth;
    QList<int> messageWidth;

    /* Initialise all graphics items
       layer = -1 to show the item has no layerWidth
       layered = -1 to show the item has not been layered yet */
    for (int i = 0; i < statesAndTransitions.count(); i++) {
        statesAndTransitions[i]->layer = -1;
        statesAndTransitions[i]->layered = -1;
    }
    for (int i = 0; i < messages.count(); i++) {
        messages[i]->layer = -1;
        messages[i]->layered = -1;
    }

    /* For each agent type */
    for (int agent = 0; agent < agentNames.count(); agent++) {
        done = 0;
        noLayers = 0;
        largestWidth = 0;
        layerWidth.clear();
        statesLayer.clear();
        agentWidth.append(0);

        int tcount;
        int highestn;
        /* For each state or transition count the number of arrows
         * going to the same state */
        for (int i = 0; i < statesAndTransitions.count(); i++) {
            GraphicsItem * sitem = statesAndTransitions[i];

            if (sitem->agentName == agentNames[agent]) {  // If the item is
                // from the current agent type
                for (int j = 0; j < sitem->
                getTransitionArrows().size(); j++) {  // for each arrow
                    tcount = 0;
                    highestn = 0;
                    Arrow * a = sitem->getTransitionArrows().at(j);
                    if (a->startItem() == sitem) {  // If the arrow is incoming
                        for (int k = 0; k < sitem->
                        getTransitionArrows().size(); k++) {
                            Arrow * a2 = sitem->getTransitionArrows().at(k);
                            if (a != a2) {
                                if (a->endItem() == a2->endItem()) {
                                    tcount++;
                                    if (highestn < a2->number) highestn =
                                            a2->number;
                                }
                            }
                        }
                        a->total = tcount+1;
                        a->number = highestn+1;
                    }
                }
            }
        }

        while (done == 0) {
            done = 1;
            noStatesInLayer = 0;
            QList<GraphicsItem*> list;
            statesLayer.append(list);

            for (int i = 0; i < statesAndTransitions.count(); i++) {
                GraphicsItem * sitem = statesAndTransitions[i];

                if (sitem->agentName == agentNames[agent]) {
                    bool include = false;

                    if (sitem->layer == -1) {
                        int count_in = 0;

                        // Find first state
                        if (noLayers == 0) {
                            for (int j = 0; j < sitem->
                            getTransitionArrows().size(); j++) {
                                Arrow * a = sitem->getTransitionArrows().at(j);
                                if (a->endItem() == sitem &&
                                    (a->startItem()->layer == -1 ||
                                        a->startItem()->layer == noLayers))
                                    count_in++;
                            }
                            if (count_in == 0) {
                                include = true;
                            }
                        } else {  // Find next states from current layer
                            for (int j = 0; j < sitem->
                            getTransitionArrows().size(); j++) {
                                Arrow * a =
                                        sitem->getTransitionArrows().at(j);
                                if (a->endItem() == sitem &&
                                        a->startItem()->layer == noLayers-1) {
                                    include = true;
                                }
                            }
                        }
                    } else if (sitem->layer == -2) {
                        include = true;
                    }

                    if (include) {
                        done = 0;
                        sitem->layer = noLayers;
                        noStatesInLayer++;
                        statesLayer[noLayers].append(sitem);

    /* Messages */
/*  if(sitem->mytype == 0) // if state
    {
        for(int j = 0; j < sitem->getTransitions().size(); j++)
        {
            Arrow * a = sitem->getTransitions().at(j);

            if(a->endItem()->mytype == 1) // If state next transition
            // has incoming message
            {
                for(int k = 0; k < messages.count(); k++)
                {
                    if(messages[k]->layer == -1)
                    {
                        for(int l = 0; l < messages[k]->
                        getTransitions().size(); l++)
                        {
                            Arrow * a2 = messages[k]->getTransitions().at(l);
                            if(a2->endItem() == a->endItem())
                            {
                                messages[k]->layer = noLayers;
                                noStatesInLayer++;
                                statesLayer[noLayers].append(messages[k]);
                            }
                        }
                    }
                }
            }
            if(a->startItem()->mytype == 1) // If state before transition
            // has outgoing message
            {
                for(int k = 0; k < messages.count(); k++)
                {
                    for(int l = 0; l < messages[k]->getTransitions().size();
                    l++)
                    {
                        if(messages[k]->layer == -1)
                        {
                            Arrow * a2 = messages[k]->getTransitions().at(l);
                            if(a2->startItem() == a->startItem())
                            {
                                messages[k]->layer = noLayers;
                                noStatesInLayer++;
                                statesLayer[noLayers].append(messages[k]);
                            }
                        }
                    }
                }
            }
        }
    }*/
                    }
                }
            }

            if (done == 0) {
                // Give an order to the states in the same layer
                int posCount;
                for (int j = 0; j < statesLayer[noLayers].count(); j++) {
                    statesLayer[noLayers][j]->aveX = 0;
                    posCount = 0;
                    for (int k = 0; k <statesLayer[noLayers][j]->
                    getTransitionArrows().size(); k++) {
                        Arrow * a = statesLayer[noLayers][j]->
                                getTransitionArrows().at(k);
                        // If the end state of the transition is the
                        // current state
                        if (a->endItem() == statesLayer[noLayers][j]) {
                           statesLayer[noLayers][j]->aveX  +=
                                   a->startItem()->x();
                        }
                    }
                    if (posCount > 0)
                        statesLayer[noLayers][j]->aveX =
                                statesLayer[noLayers][j]->aveX/posCount;
                }

                // Calculate the x-axis position of the states from left
                // side (no centring)
                layerWidth.append(0);

                // Position states in order that their previous transition
                // states were in
                GraphicsItem * lowest = 0;
                for (int j = 0; j < statesLayer[noLayers].count(); j++) {
                    int lowestX = -1;
                    for (int k = 0; k < statesLayer[noLayers].count(); k++) {
                        if (statesLayer[noLayers][k]->aveX > -1) {
                            if (lowestX == -1) {
                                lowestX = statesLayer[noLayers][k]->aveX;
                                lowest = statesLayer[noLayers][k];
                            } else if (statesLayer[noLayers][k]->aveX <=
                                    lowestX) {
                                lowestX = statesLayer[noLayers][k]->aveX;
                                lowest = statesLayer[noLayers][k];
                            }
                        }
                    }

                    // Calculate position
                    calcState(lowest, &layerWidth, layerHeight);
                    lowest->aveX = -1;
                }

                // Remove the last gap between states as there is not another
                // state to add
                layerWidth[noLayers] -= 10.0;
                // messageWidth.append(layerWidth[noLayers]);

                if (layerWidth[noLayers] > largestWidth)
                    largestWidth = layerWidth[noLayers];

                // If a state has incoming transitions that have not been
                // layered
                // then remove the state from the current layer and add
                // to the next layer
                for (int j = 0; j < statesLayer[noLayers].count(); j++) {
                    bool flag = false;
                    for (int k = 0; k < statesLayer[noLayers][j]->
                    getTransitionArrows().size(); k++) {
                        Arrow * a = statesLayer[noLayers][j]->
                                getTransitionArrows().at(k);
                        // If the end state of the transition is the current
                        // state and
                        // the start state does not yet have a layer or is
                        // in the current layer then...
                        if (a->endItem() == statesLayer[noLayers][j] &&
                                (a->startItem()->layer == noLayers ||
                                        a->startItem()->layer == -1))
                            flag = true;
                    }
                    if (flag) {
                        // Add the state to the next layer
                        statesLayer[noLayers][j]->layer = -2;  // noLayers+1;
                        statesLayer[noLayers][j]->layered = -1;
                        statesLayer[noLayers].removeAt(j);
                        j--;
                    }
                }

                noLayers++;
            }
        }

        ///////////

        // qDebug() << largestWidth;
//        for(int i = 0; i < noLayers; i++)
//            if(messageWidth[i] > largestWidth) largestWidth = messageWidth[i];
        // qDebug() << largestWidth;
        // largestWidth += 20;

        if (largestWidth > globalLargestWidth) globalLargestWidth =
                largestWidth;
        if (noLayers > globalHighestLayer) globalHighestLayer = noLayers;

        agentWidth[agent] = largestWidth;
        // nextWidth = largestWidth + ();

        // Centre the states
        for (int i = 0; i < noLayers; i++) {
            for (int j = 0; j < statesLayer[i].count(); j++) {
                statesLayer[i][j]->moveBy((largestWidth/2.0) -
                        (layerWidth[i]/2.0), 0.0);
            }
        }
    }

    int rollingWidth = 0;
    if (agentNames.count() > 0) rollingWidth = agentWidth[0] + 10;
    /* Move agent items so they are separated */
    if (agentWidth.count() > 0) {
        for (int agent = 1; agent < agentNames.count(); agent++) {
            for (int i = 0; i < statesAndTransitions.count(); i++) {
                GraphicsItem * sitem = statesAndTransitions[i];

                if (sitem->agentName == agentNames[agent]) {
                    sitem->moveBy(rollingWidth, 0.0);
                }
            }

            rollingWidth += agentWidth[agent] + 10;
        }
    }

    // Handle messages
    // if(agentWidth.count() == 1)
    // {
        for (int i = 0; i < globalHighestLayer; i++)
            messageWidth.append(rollingWidth);

        /*QList<int> agentMessageCount;
        for(int i = 0; i < agentNames.count(); i++)
        agentMessageCount.append(0);*/

        for (int i = 0; i < messages.count(); i++) {
            /*for(int j = 0; j < agentMessageCount.size(); j++)
             * agentMessageCount[j] = 0;*/
            int lastOutputLayer = -1;
            int firstInputLayer = -1;
            for (int j = 0; j < messages[i]->getMessageArrows().size(); j++) {
                Arrow * a = messages[i]->getMessageArrows().at(j);
                if (a->endItem() == messages[i]) {
                    if (lastOutputLayer == -1 ||
                            lastOutputLayer < a->startItem()->layer)
                        lastOutputLayer = a->startItem()->layer;
                    /*for(int agent = 0; agent < agentNames.count(); agent++)
                        if(a->startItem()->agentName == agentNames[agent])
                        agentMessageCount[agent]++;*/
                }
                if (a->startItem() == messages[i]) {
                    if (firstInputLayer == -1 ||
                            firstInputLayer > a->endItem()->layer)
                        firstInputLayer = a->endItem()->layer;
                    /*for(int agent = 0; agent < agentNames.count(); agent++)
                        if(a->endItem()->agentName == agentNames[agent]) agentMessageCount[agent]++;*/
                }
            }

            /*qDebug() << messages[i]->getName();
            for(int j = 0; j < agentMessageCount.size(); j++) qDebug() << "\t" << agentNames[j] << agentMessageCount[j];*/

            int messageLayer;
            if (lastOutputLayer != -1) messageLayer = lastOutputLayer+1;
            else
                if (firstInputLayer != -1) messageLayer = firstInputLayer-1;
                else
                    messageLayer = 0;
            messages[i]->setPos(messageWidth[messageLayer]+10.0+
                    (messages[i]->width()/2.0), 20.0 +
                    ((messageLayer)*layerHeight));
            messageWidth[messageLayer] += 10.0+messages[i]->width();
            // statesLayer[messageLayer].append(messages[i]);
            if (messageWidth[messageLayer] > rollingWidth)
                rollingWidth = messageWidth[messageLayer];
        }
    // }

    setSceneRect(0, 0, rollingWidth, 20.0+(globalHighestLayer*layerHeight));

    /* Update line positions with respect to state positions */
    for (int i = 0; i < items().size(); i++) {
        QGraphicsItem * qitem = items().at(i);

        if (qgraphicsitem_cast<Arrow *>(qitem)) {
            Arrow * sitem = qgraphicsitem_cast<Arrow *>(qitem);
            sitem->updatePosition();
        }
    }

    update();
}

void MachineScene::addMessageCommunication(GraphicsItem *t, MessageComm * m,
        bool isInput) {
    // Find the associated message state item if not make one
    GraphicsItem * message = 0;
    for (int j = 0; j < messages.size(); j++) {
        GraphicsItem * sitem1 = messages.at(j);
        if (sitem1->mytype == 2 && sitem1->getName() == m->messageType)
            message = sitem1;
    }
    if (message == 0) {
        message = new GraphicsItem;
        message->setMessage(m);
        addItem(message);
        messages.append(message);
        num_messages++;
        int shift = 40;
        if (isInput) shift *= -1;
        message->setPos(t->x(), t->y()+shift);
    }

    Q_ASSERT(message != 0);

    Arrow * arrow;
    if (isInput) arrow = new Arrow(message, t);
    else
        arrow = new Arrow(t, message);
    arrow->isCommunication = true;
    message->addMessageArrow(arrow);
    t->addMessageArrow(arrow);
    addArrow(arrow);
}

void MachineScene::addTransitionTransition(QString agentName,
        Transition * t, int isForeign) {
    GraphicsItem * func = addTransitionString(agentName, t, isForeign);

    if (!isForeign) {
        Q_ASSERT(func != 0);

        // For all input messages
        for (int i = 0; i < t->input().messageModel->messages.count(); i++) {
            addMessageCommunication(func,
                    &(t->input().messageModel->messages[i]), true);
        }

        for (int i = 0; i < t->output().messageModel->messages.count(); i++) {
            addMessageCommunication(func,
                    &(t->output().messageModel->messages[i]), false);
        }
    }
}

GraphicsItem * MachineScene::addTransitionString(QString agentName,
        Transition * t, int flag) {
    QString csn = t->currentState()->name();
    QString n = t->name();
    QString nsn = t->nextState()->name();
    GraphicsItem * cs = 0;
    GraphicsItem * ns = 0;
    GraphicsItem * func = 0;
    Arrow * arrow = 0;

    // add agent name to list
    if (!agentNames.contains(agentName)) agentNames.append(agentName);

    for (int j = 0; j < statesAndTransitions.size(); j++) {
        GraphicsItem * sitem1 = statesAndTransitions.at(j);

        if (sitem1->agentName == agentName) {
            if (sitem1->mytype == 0) {
                if (sitem1->getName() == csn) cs = sitem1;
                if (sitem1->getName() == nsn) ns = sitem1;
            } else {
                if (sitem1->mytype == 1 && sitem1->getName() == n &&
                        sitem1->preState == csn && sitem1->postState == nsn)
                func = sitem1;
            }
        }
     }

    if (cs == 0) {
        GraphicsItem *state = new GraphicsItem;
        state->setState(t->currentState());
        state->agentName = agentName;
        cs = state;
        addState(state);
        if (flag == 1) state->foreign = true;
    }
    if (ns == 0) {
        GraphicsItem *state = new GraphicsItem;
        state->setState(t->nextState());
        state->agentName = agentName;
        ns = state;
        addState(state);
        if (flag == 1) state->foreign = true;
    }

    if (func == 0) {
        func = new GraphicsItem;
        func->setTransition(t);
        func->agentName = agentName;
        if (flag == 1) func->foreign = true;
        addTransition(func);

        arrow = new Arrow(cs, func);
        cs->addTransitionArrow(arrow);
        func->addTransitionArrow(arrow);
        if (flag == 1) arrow->foreign = true;
        arrow->drawHead(false);
        addArrow(arrow);

        arrow = new Arrow(func, ns);
        func->addTransitionArrow(arrow);
        ns->addTransitionArrow(arrow);
        if (flag == 1) arrow->foreign = true;
        addArrow(arrow);
    }

    return func;
}

void MachineScene::addState(GraphicsItem * s) {
    addItem(s);
    statesAndTransitions.append(s);
    num_states++;
}

void MachineScene::addTransition(GraphicsItem * t) {
    addItem(t);
    statesAndTransitions.append(t);
    num_transitions++;
}

void MachineScene::addArrow(Arrow * t) {
    addItem(t);
    t->setZValue(-1.0);
}

void MachineScene::updateStateName(State *s) {
    for (int i = 0; i < statesAndTransitions.size(); i++) {
        GraphicsItem * sitem1 = statesAndTransitions.at(i);

        if (sitem1->state() == s) {
            sitem1->nameChanged();
        }
    }
    this->update();
}

void MachineScene::updateTransitionName(Transition * t) {
    for (int i = 0; i < statesAndTransitions.size(); i++) {
        GraphicsItem * sitem1 = statesAndTransitions.at(i);

        if (sitem1->transition == t) {
            sitem1->nameChanged();
        }
    }
    this->update();
}

void MachineScene::updateInput(Transition *t) {
    // Find the graphics item that corresponds to the transition
    for (int i = 0; i < statesAndTransitions.size(); i++) {
        GraphicsItem * transitionItem = statesAndTransitions.at(i);

        if (transitionItem->transition == t) {
            // Add missing arrows
            // For each transition input message
            for (int k = 0; k < t->input().messageModel->messages.count();
                    k++) {
                MessageComm * message = &(t->input().messageModel->
                        messages[k]);
                // Look to see if it already exists
                bool isFound = false;
                for (int j = 0; j < transitionItem->getMessageArrows().size();
                        j++) {
                    GraphicsItem * messageItem =
                            transitionItem->getMessageArrows().at(j)->
                            startItem();
                    if (messageItem->mytype == 2 &&
                            messageItem->getName() == message->messageType)
                        isFound = true;
                }
                if (!isFound)
                    addMessageCommunication(transitionItem, message, true);
            }

            // Remove redundant arrows
            QList<Arrow*> arrowsToDelete;
            for (int j = 0; j < transitionItem->getMessageArrows().size();
                    j++) {
                Arrow * arrow = transitionItem->getMessageArrows()[j];
                GraphicsItem * messageItem = arrow->startItem();
                // If the arrow is an input arrow
                // I.e. the startItem is a message GraphicsItem
                if (messageItem->mytype == 2) {
                    bool isFound = false;
                    for (int k = 0; k < t->input().messageModel->
                    messages.count(); k++) {
                        MessageComm * message = &(t->input().messageModel->
                                messages[k]);
                        if (messageItem->getName() == message->messageType)
                            isFound = true;
                    }
                    if (!isFound) {
                        arrowsToDelete.append(arrow);
                        // remove arrow from message
                        messageItem->removeMessageArrow(arrow);
                        // Check to see if the message state item is redundant
                        if (messageItem->getMessageArrows().size() == 0) {
                            removeItem(messageItem);
                            messages.removeOne(messageItem);
                            delete messageItem;
                        }
                    }
                }
            }
            // Delete arrows
            for (int i = 0; i < arrowsToDelete.size(); i++) {
                Arrow * arrow = arrowsToDelete[i];
                // Remove arrow
                transitionItem->removeMessageArrow(arrow);
                removeItem(arrow);
                delete arrow;
            }
        }
    }
    this->update();
}

void MachineScene::updateOutput(Transition *t) {
    for (int i = 0; i < statesAndTransitions.size(); i++) {
        GraphicsItem * transitionItem = statesAndTransitions.at(i);

        if (transitionItem->transition == t) {
            // Add missing arrows
            // For each transition input message
            for (int k = 0; k < t->output().messageModel->messages.count();
                    k++) {
                MessageComm * message = &(t->output().messageModel->
                        messages[k]);
                // Look to see if it already exists
                bool isFound = false;
                for (int j = 0; j < transitionItem->getMessageArrows().size();
                        j++) {
                    GraphicsItem * messageItem =
                            transitionItem->getMessageArrows().at(j)
                            ->endItem();
                    if (messageItem->mytype == 2 &&
                            messageItem->getName() == message->messageType)
                        isFound = true;
                }
                if (!isFound)
                    addMessageCommunication(transitionItem, message, false);
            }

            // Remove redundant arrows
            QList<Arrow*> arrowsToDelete;
            for (int j = 0; j < transitionItem->
            getMessageArrows().size(); j++) {
                Arrow * arrow = transitionItem->getMessageArrows()[j];
                GraphicsItem * messageItem = arrow->endItem();
                // If the arrow is an input arrow
                // I.e. the startItem is a message GraphicsItem
                if (messageItem->mytype == 2) {
                    bool isFound = false;
                    for (int k = 0; k < t->output().messageModel->
                    messages.count(); k++) {
                        MessageComm * message = &(t->output().messageModel->
                                messages[k]);
                        if (messageItem->getName() == message->messageType)
                            isFound = true;
                    }
                    if (!isFound) {
                        arrowsToDelete.append(arrow);
                        // remove arrow from message
                        messageItem->removeMessageArrow(arrow);
                        // Check to see if the message state item is redundant
                        if (messageItem->getMessageArrows().size() == 0) {
                            removeItem(messageItem);
                            messages.removeOne(messageItem);
                            delete messageItem;
                        }
                    }
                }
            }
            // Delete arrows
            for (int i = 0; i < arrowsToDelete.size(); i++) {
                Arrow * arrow = arrowsToDelete[i];
                // Remove arrow
                transitionItem->removeMessageArrow(arrow);
                removeItem(arrow);
                delete arrow;
            }
        }
    }
    this->update();
}

void MachineScene::selectState(QString n) {
    clearSelection();

    for (int j = 0; j < items().size(); j++) {
        if (qgraphicsitem_cast<GraphicsItem *>(items().at(j))) {
            GraphicsItem * sitem1 =
                    qgraphicsitem_cast<GraphicsItem *>(items().at(j));

            if (sitem1->getName() == n) {
                sitem1->setSelected(true);
            }
        }
    }
    update();
}

void MachineScene::selectTransition(QString n) {
    clearSelection();

    for (int j = 0; j < items().size(); j++) {
        if (qgraphicsitem_cast<Arrow *>(items().at(j))) {
            Arrow * a = qgraphicsitem_cast<Arrow *>(items().at(j));

            if (a->getName() == n) {
                a->setSelected(true);
            }
        }
    }
    update();
}

void MachineScene::removeTransitionFunction(GraphicsItem *t) {
    // Remove message arrows
    t->transition->setInput(Communication(1));
    updateInput(t->transition);
    t->transition->setOutput(Communication(0));
    updateOutput(t->transition);
    // Remove transition arrows
    QList<Arrow*> arrowsToDelete;
    for (int i = 0; i < t->getTransitionArrows().size(); i++) {
        Arrow * arrow = t->getTransitionArrows()[i];
        arrowsToDelete.append(arrow);
        GraphicsItem * state;
        if (arrow->startItem() == t) state = arrow->endItem();
        else
            state = arrow->startItem();
        state->removeTransitionArrow(arrow);
        if (state->getTransitionArrows().size() == 0) {
            statesAndTransitions.removeOne(state);
            removeItem(state);
            delete state;
        }
    }
    for (int i = 0; i < arrowsToDelete.size(); i++) {
        Arrow * arrow = arrowsToDelete[i];
        // Remove arrow
        t->removeTransitionArrow(arrow);
        removeItem(arrow);
        delete arrow;
    }
    statesAndTransitions.removeOne(t);
    removeItem(t);
    delete t;
}

void MachineScene::deleteSelectedFunction() {
    Q_ASSERT(selectedFunction != 0);

    // Remove function from transition table
    machineModel->deleteTransition(selectedFunction->transition);
    // Remove function from scene
    removeTransitionFunction(selectedFunction);
}
