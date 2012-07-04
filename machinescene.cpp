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
    selectedState = 0;
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
        /* If line exists then update line */
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);

        /* If a state is selected */
        if (selectedState != 0) {
            /* Find item at mouse location */
            QGraphicsItem * qitem = itemAt(mouseEvent->scenePos());
            /* If there is a qgraphics item at the location */
            if (qitem != 0) {
                /* Try and cast qgraphics item to graphics item */
                if (qgraphicsitem_cast<GraphicsItem *>(qitem)) {
                    /* Cast qgraphics item qitem to graphics item sitem */
                    GraphicsItem * sitem =
                            qgraphicsitem_cast<GraphicsItem *>(qitem);
                    /* If item is another state then highlight it */
                    if (sitem->mytype == 0) {
                        if (sitem != selectedState) {
                            qitem->setSelected(true);
                        }
                    }
                }
            }
        }
    }

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void MachineScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    /* Pointer to possible graphics item */
    GraphicsItem * sitem;
    /* Reset selection variables */
    selectedFunction = 0;
    selectedState = 0;
    /* Clear any selected graphics items */
    clearSelection();
    /* Emit signal to say no function is selected */
    emit(functionSelected(false));

    /* If the machine for this graph is an agent (not a model) */
    if (rootMachine->type == 1) {
        /* Find item at mouse location */
        QGraphicsItem * qitem = itemAt(mouseEvent->scenePos());
        /* If there is a qgraphics item at the location */
        if (qitem != 0) {
            /* Set qgraphics item to be selected */
            qitem->setSelected(true);
            /* Try and cast qgraphics item to graphics item */
            if (qgraphicsitem_cast<GraphicsItem *>(qitem)) {
                /* Cast qgraphics item qitem to graphics item sitem */
                sitem = qgraphicsitem_cast<GraphicsItem *>(qitem);

                /* If left mouse click proceed with event,
                 * which is passed to scene items to select/move them */
                if (mouseEvent->button() == Qt::LeftButton) {
                    /* If function and not foreign then set selectedFunction and
                       emit signal */
                    if (sitem->mytype == 1 && !sitem->foreign) {
                        selectedFunction = sitem;
                        emit(functionSelected(true));
                    }
                    /* If state then set selectedState */
                    if (sitem->mytype == 0) selectedState = sitem;
                }

                /* If right mouse button start line */
                if (mouseEvent->button() == Qt::RightButton) {
                    /* Start drawing line */
                    line = new QGraphicsLineItem(
                            QLineF(mouseEvent->scenePos(),
                                    mouseEvent->scenePos()));
                    line->setPen(QPen(Qt::black, 1));
                    addItem(line);
                }
            }
        }
    }

    /* Allow QGraphicsScene to continue processing mouse event */
    QGraphicsScene::mousePressEvent(mouseEvent);
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
        Transition * t, int isForeign, int isEditable) {
    GraphicsItem * func = addTransitionString(
                agentName, t, isForeign, isEditable);

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
        Transition * t, int flag, int isEditable) {
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
        if (isEditable == 0) state->editable = false;
    } else {
        if (flag == 0) cs->foreign = false;
        if (isEditable == 1) cs->editable = true;
    }
    if (ns == 0) {
        GraphicsItem *state = new GraphicsItem;
        state->setState(t->nextState());
        state->agentName = agentName;
        ns = state;
        addState(state);
        if (flag == 1) state->foreign = true;
        if (isEditable == 0) state->editable = false;
    } else {
        if (flag == 0) ns->foreign = false;
        if (isEditable == 1) ns->editable = true;
    }

    if (func == 0) {
        func = new GraphicsItem;
        func->setTransition(t);
        func->agentName = agentName;
        if (flag == 1) func->foreign = true;
        if (isEditable == 0) func->editable = false;
        addTransition(func);

        arrow = new Arrow(cs, func);
        cs->addTransitionArrow(arrow);
        func->addTransitionArrow(arrow);
        if (flag == 1) arrow->foreign = true;
        if (isEditable == 0) arrow->editable = false;
        arrow->drawHead(false);
        addArrow(arrow);

        arrow = new Arrow(func, ns);
        func->addTransitionArrow(arrow);
        ns->addTransitionArrow(arrow);
        if (flag == 1) arrow->foreign = true;
        if (isEditable == 0) arrow->editable = false;
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
    if (selectedFunction == 0) {
        // qDebug() << "selectedFunction == 0";
    } else {
        // Remove function from transition table
        machineModel->deleteTransition(selectedFunction->transition);
        // Remove function from scene
        removeTransitionFunction(selectedFunction);
        // Reset selected function variable
        selectedFunction = 0;
        emit(functionSelected(false));
    }
}

void MachineScene::clearAll() {
    /* Clear all lists */
    agentNames.clear();
    statesAndTransitions.clear();
    messages.clear();
    /* Clear all items */
    clear();
}
