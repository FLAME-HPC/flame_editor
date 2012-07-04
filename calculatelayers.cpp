/*!
 * \file calculatelayers.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of graph layout algorithm
 */
#include "./machinescene.h"
#include "./machine.h"

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
