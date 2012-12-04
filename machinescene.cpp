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
    selectedMachine = m;
    edit_ = true;
    move_ = false;
    highlightedState_ = 0;
}

void MachineScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    GraphicsItem * startItem = 0;
    GraphicsItem * endItem = 0;

    if (selectedState) selectedState->setZValue(0.0);
    if (selectedFunction) selectedFunction->setZValue(0.0);

    if (move_) {
        if (selectedState) {
            selectedState->setZValue(0.0);
            if(highlightedState_) {
                qDebug() << "merge " << highlightedState_->getName() << " and " << selectedState->getName();
                // Move all arrows into and out of selected state in highlighted state
                QList<Arrow *> arrows = selectedState->getTransitionArrows();
                QList<Arrow *>::iterator it;
                for (it = arrows.begin(); it != arrows.end(); ++it) {
                    if ((*it)->myStartItem == selectedState) {
                        (*it)->myStartItem = highlightedState_;
                        (*it)->myEndItem->transition->setCurrentState(highlightedState_->state());
                    }
                    if ((*it)->myEndItem == selectedState) {
                        (*it)->myEndItem = highlightedState_;
                        (*it)->myStartItem->transition->setNextState(highlightedState_->state());
                    }
                    selectedState->removeTransitionArrow((*it));
                    highlightedState_->addTransitionArrow((*it));
                    (*it)->update();
                }
                selectedState->machine->machineModel->removeState(selectedState->state());
                removeState(selectedState);
                selectedState = 0;
                // Solves item jumping when next moved
                mousePressEvent(mouseEvent);
            }
        }

        QList<GraphicsItem *>::iterator it;
        for (it = states_.begin(); it != states_.end(); ++it) {
            (*it)->setHighlighted(false);
        }
        highlightedState_ = 0;
    }

    if (line != 0) {
        QList<QGraphicsItem *> startItems = items(line->line().p2());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p1());
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

            State * state = selectedMachine->machineModel->addState(s);

            endItem = new GraphicsItem;
            endItem->setState(state);
            endItem->agentName = startItem->agentName;
            endItem->machine = startItem->machine;
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

                Transition * t = selectedMachine->machineModel->addTransition(s,
                        startItem->state(), endItem->state());

                GraphicsItem * fitem = new GraphicsItem;
                fitem->setTransition(t);
                fitem->agentName = startItem->agentName;
                fitem->machine = startItem->machine;
                fitem->currentState_ = startItem;
                fitem->nextState_ = endItem;
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

                selectedMachine->machineModel->addMessageToTransition(startItem->transition,
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

                selectedMachine->machineModel->addMessageToTransition(
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
        QLineF newLine(mouseEvent->scenePos(), line->line().p2());
        line->setLine(newLine);

        //qDebug() << line->myColor;

        //qDebug() << mouseEvent->scenePos();

        // If line from a state
        if (selectedState) {
            // Find item at mouse location
            QList<QGraphicsItem *> list = items(mouseEvent->scenePos());
            // If there is a qgraphics item at the location
            GraphicsItem * sitem2 = 0;
            highlightedState_ = 0;
            if (list.size() > 1) {
                GraphicsItem * sitem0 = qgraphicsitem_cast<GraphicsItem *>(list.at(0));
                GraphicsItem * sitem1 = qgraphicsitem_cast<GraphicsItem *>(list.at(1));
                GraphicsItem * sline = qgraphicsitem_cast<GraphicsItem *>(line);
                if (sline == sitem0) sitem2 = sitem1;
                if (sline == sitem1) sitem2 = sitem0;
                if (sitem2) if (sitem2->mytype == 0 && sitem2->machine == selectedState->machine) {
                    sitem2->setHighlighted(true);
                    highlightedState_ = sitem2;
                }
            }

            QList<GraphicsItem *>::iterator it;
            for (it = states_.begin(); it != states_.end(); ++it) {
                if ((*it) != highlightedState_) (*it)->setHighlighted(false);
            }
        }
    }

    // If a state is selected
    if (move_ && selectedState != 0) {
        QList<QGraphicsItem *> list = items(mouseEvent->scenePos());
        GraphicsItem * sitem2 = 0;
        highlightedState_ = 0;
        if (list.size() > 1) {
            GraphicsItem * sitem0 = qgraphicsitem_cast<GraphicsItem *>(list.at(0));
            GraphicsItem * sitem1 = qgraphicsitem_cast<GraphicsItem *>(list.at(1));
            if (selectedState == sitem0) sitem2 = sitem1;
            if (selectedState == sitem1) sitem2 = sitem0;
            if (sitem2) if (sitem2->mytype == 0 && sitem2->machine == selectedState->machine) {
                sitem2->setHighlighted(true);
                highlightedState_ = sitem2;
            }
        }

        QList<GraphicsItem *>::iterator it;
        for (it = states_.begin(); it != states_.end(); ++it) {
            if ((*it) != highlightedState_) (*it)->setHighlighted(false);
        }
    }

    if (move_) QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void MachineScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (mouseEvent->button() == Qt::LeftButton) {
        /* Find item at mouse location and try and cast qgraphics item to graphics item */
        GraphicsItem * sitem = qgraphicsitem_cast<GraphicsItem *>(itemAt(mouseEvent->scenePos()));
        /* If there is a qgraphics item at the location */
        if (sitem != 0) {
            // If function
            if (sitem->mytype == 1) {
                // qDebug() << "double click on " << sitem->getName();
                emit functionDoubleClicked(sitem->transition);
            }
        }
    }

    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void MachineScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    /* Reset selection variables */
    selectedFunction = 0;
    selectedState = 0;
    /* Clear any selected graphics items */
    clearSelection();
    /* Emit signal to say no function is selected */
    emit(functionSelected(0));


    /* Find item at mouse location */
    GraphicsItem * sitem = qgraphicsitem_cast<GraphicsItem *>(itemAt(mouseEvent->scenePos()));
    /* If there is a qgraphics item at the location */
    if (sitem != 0) {


        /* If the machine for this graph is an agent (not a model) */
        //if (rootMachine->type == 1) {

            /* If left mouse click proceed with event,
             * which is passed to scene items to select/move them */
            //if (mouseEvent->button() == Qt::LeftButton) {
                // If sitem machine is different from current one
                if (sitem->machine != selectedMachine) {
                    emit machineSelected(sitem->machine);
                    machineSelectedSlot(sitem->machine);
                    selectedMachine = sitem->machine;
                }
                /* If function and not foreign then set selectedFunction and
                   emit signal */
                if (sitem->mytype == 1) {  // && !sitem->foreign) {
                    selectedFunction = sitem;
                    selectedFunction->setZValue(1.0);
                    emit functionSelected(sitem->transition);
                }
                /* If state then set selectedState */
                if (sitem->mytype == 0) {
                    selectedState = sitem;
                    selectedState->setZValue(1.0);
                }
            //}

            /* If right mouse button start line */
            //if (mouseEvent->button() == Qt::RightButton) {
            if (edit_) {
                /* Start drawing line */
                line = new Arrow(
                        QLineF(mouseEvent->scenePos(),
                                mouseEvent->scenePos()));
                //line->setPen(QPen(Qt::black, 1));
                line->setZValue(0.9);
                addItem(line);
            }
    } else {
        // No item selected
        if (rootMachine != selectedMachine) {
            emit machineSelected(rootMachine);
            machineSelectedSlot(rootMachine);
            selectedMachine = rootMachine;
        }
        selectedFunction = 0;
        selectedState = 0;
    }

    /* Allow QGraphicsScene to continue processing mouse event */
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void MachineScene::machineSelectedSlot(Machine * m) {
    QList<GraphicsItem*>::iterator it;
    if (m == rootMachine) {
        // every item is foreign
        for (it = transitions_.begin(); it != transitions_.end(); ++it) {
            (*it)->foreign = true;
            (*it)->currentState_->foreign = true;
            (*it)->nextState_->foreign = true;
            for (int i = 0; i < (*it)->getTransitionArrows().size(); i++) {
                Arrow * arrow = (*it)->getTransitionArrows()[i];
                arrow->foreign = true;
            }
        }
    } else {
        // first not machine
        for (it = transitions_.begin(); it != transitions_.end(); ++it) {
            if ((*it)->machine != m) {
                (*it)->foreign = true;
                (*it)->currentState_->foreign = true;
                (*it)->nextState_->foreign = true;
                for (int i = 0; i < (*it)->getTransitionArrows().size(); i++) {
                    Arrow * arrow = (*it)->getTransitionArrows()[i];
                    arrow->foreign = true;
                }
            }
        }
        // agent machine
        for (it = transitions_.begin(); it != transitions_.end(); ++it) {
            if ((*it)->machine == m) {
                (*it)->foreign = false;
                (*it)->currentState_->foreign = false;
                (*it)->nextState_->foreign = false;
                for (int i = 0; i < (*it)->getTransitionArrows().size(); i++) {
                    Arrow * arrow = (*it)->getTransitionArrows()[i];
                    arrow->foreign = false;
                }
            }
        }
    }
    this->update();
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

void MachineScene::addTransitionTransition(Machine *m, Transition * t) {
    GraphicsItem * func = addTransitionString(m, t);

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

GraphicsItem * MachineScene::addTransitionString(Machine * m, Transition * t) {
    QString csn = t->currentState()->name();
    QString n = t->name();
    QString nsn = t->nextState()->name();
    GraphicsItem * cs = 0;
    GraphicsItem * ns = 0;
    GraphicsItem * func = 0;
    Arrow * arrow = 0;

    // add agent name to list
    if (!agentNames.contains(m->name)) agentNames.append(m->name);

    for (int j = 0; j < statesAndTransitions.size(); j++) {
        GraphicsItem * sitem1 = statesAndTransitions.at(j);

        if (sitem1->agentName == m->name) {
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
        state->agentName = m->name;
        cs = state;
        addState(state);
        state->machine = m;
        cs = state;
    }
    if (ns == 0) {
        GraphicsItem *state = new GraphicsItem;
        state->setState(t->nextState());
        state->agentName = m->name;
        ns = state;
        addState(state);
        state->machine = m;
        ns = state;
    }

    if (func == 0) {
        func = new GraphicsItem;
        func->setTransition(t);
        func->agentName = m->name;
        func->machine = m;
        func->currentState_ = cs;
        func->nextState_ = ns;
        addTransition(func);

        arrow = new Arrow(cs, func);
        cs->addTransitionArrow(arrow);
        func->addTransitionArrow(arrow);
        arrow->drawHead(false);
        addArrow(arrow);

        arrow = new Arrow(func, ns);
        func->addTransitionArrow(arrow);
        ns->addTransitionArrow(arrow);
        addArrow(arrow);
    }

    return func;
}

void MachineScene::addState(GraphicsItem * s) {
    addItem(s);
    statesAndTransitions.append(s);
    states_.append(s);
    num_states++;
}

void MachineScene::addTransition(GraphicsItem * t) {
    addItem(t);
    statesAndTransitions.append(t);
    transitions_.append(t);
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

void MachineScene::removeState(GraphicsItem * s) {
    states_.removeOne(s);
    statesAndTransitions.removeOne(s);
    removeItem(s);
    delete s;
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
    transitions_.removeOne(t);
    removeItem(t);
    delete t;
}

void MachineScene::deleteSelectedFunction() {
    if (selectedFunction == 0) {
        // qDebug() << "selectedFunction == 0";
    } else {
        // Remove function from transition table
        selectedMachine->machineModel->deleteTransition(selectedFunction->transition);
        // Remove function from scene
        removeTransitionFunction(selectedFunction);
        // Reset selected function variable
        selectedFunction = 0;
        emit(functionSelected(0));
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

void MachineScene::edit() {
    edit_ = true;
    move_ = false;
}

void MachineScene::move() {
    move_ = true;
    edit_ = false;
}
