/*!
 * \file machinescene.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of the graphics Scene for the stategraph
 */
#include <QtGui>
#include "./femachinescene.h"
//#include "./machine.h"
#include "./codedialog.h"

FEMachineScene::FEMachineScene(QObject *parent)
    : QGraphicsScene(parent) {
    /* Initialise variables */
    codeDialog = parent;
    line = 0;
    arc = 0;
    itemToMove = 0;
    itemToColaps = 0;
    itemSelect = 0;
    itemArc = 0;
    num_states = 0;
    num_transitions = 0;
    num_conditions = 0;
    namePosition = -1;

    editAction = new QAction(tr("Edit"), this);
    connect(editAction,SIGNAL(triggered()), this, SLOT(edit()));

    splitAction = new QAction(tr("Split"), this);
    connect(splitAction,SIGNAL(triggered()), this, SLOT(split()));

    /* Set a size for the scene */
    //setSceneRect(0, 0, 500, 500);

    //GraphicsArcItem *e = new GraphicsArcItem(QPointF(100,100), QPointF(100,200));
    //addItem(e);

    //GraphicsItem *g = new GraphicsItem(EmptyState);
    //g->setPos(100, 100);
    //addItem(g);

    /* Add the start state */
    /* Create GraphicsItem object */
    FEGraphicsItem * state = new FEGraphicsItem(State, "start");
    /* Set the position in the scene */
    state->setPos(50.0, 100.0);
    itemStart = state;
    /* Add it to the scene */
    addItem(state);
    newTransition(state);
    //addWHILEorFOR(state);
    //addIF(state);
    //state->setToolTip(tr("po"));

    changed = false;

    arrangeGraphicsItem();
}

void FEMachineScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    /* Initialise pointers to any graphics item at the
     * start or end of the drawing line when using the mouse */
    FEGraphicsItem * startItem = 0;
    FEGraphicsItem * endItem = 0;

    //qDebug()<<(itemToColaps !=0 )<< (itemToMove != 0);
    if(itemToMove != 0 && itemToColaps != 0){
        if(itemToMove->getGraphicsItemParents() != 0){
            FEArrow * arrow = qgraphicsitem_cast<FEArrow*>( itemToMove->getGraphicsItemParents()->getGraphicsItem());
            arrow->setEndItem(itemToColaps);
            arrow->updatePosition();
            itemToColaps->addGraphicsItemParents(itemToMove->getGraphicsItemParents());
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

    if(itemArc != 0){
        if(itemSelect->getColor() == Qt::blue)
        {
            itemSelect->setSelected(true);
            itemSelect->setColor(Qt::black);
        }
        itemArc->setColor(Qt::black);
        if(itemSelect == itemArc && testWHILE(itemSelect))
        {
            addWHILEorFOR(itemSelect);
            arrangeGraphicsItem();
            changed = true;
        }
        else if(testIF(itemSelect, itemArc))
        {
            QMessageBox mb (qobject_cast<QWidget*>( this->parent()));
            mb.setIcon(QMessageBox::Information);
            mb.setText("On what branch you want the new transition?");
            QPushButton *b1 = new QPushButton(tr("true"), qobject_cast<QWidget*>( this->parent()));
            QPushButton *b2 = new QPushButton(tr("false"), qobject_cast<QWidget*>( this->parent()));
            mb.addButton(b1, QMessageBox::YesRole);
            mb.addButton(b2, QMessageBox::NoRole);
            int r = mb.exec();
            bool b;
            if(r == 1)
                b = true;
            else
                b = false;
            addIF(itemSelect, b);
            arrangeGraphicsItem();
            changed = true;
        }
        itemArc = 0;
        invalidate();
    }

    /* If there exists a line being drawn */
    if (arc != 0) {
        /* Create a list of graphics items at the start and end points
         * of the line */
        QList<QGraphicsItem *> startItems = items(arc->getStart());
        if (startItems.count() && startItems.first() == arc)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(arc->getStop());
        if (endItems.count() && endItems.first() == arc)
            endItems.removeFirst();

        /* Remove and delete line as no longer needed */
        removeItem(arc);
        delete arc;
        arc = 0;
        invalidate();

        /* Make startItem and endItem equal to any graphics items at
         * the start and end points of the line */
        if (startItems.count() > 0 &&
                startItems.first()->type() == FEGraphicsItem::Type)
            startItem = qgraphicsitem_cast<FEGraphicsItem *>(startItems.first());
        if (endItems.count() > 0 &&
                endItems.first()->type() == FEGraphicsItem::Type)
            endItem = qgraphicsitem_cast<FEGraphicsItem *>(endItems.first());

        /* If the startItem is a state and the endItem is nothing then
         * create a new state at the end of the line and assign to endItem */
        //if (startItem->mytype == 0 && endItem == 0) {
            /* Create GraphicsItem object */
        //}

        /* If the startItem and endItem are not null and are not
         * the same graphics items (i.e. not the same state)
         * (This is wrong as a transition can go back to the same state) */
        if (startItem != 0 && endItem == 0) {
                if(startItem->countGraphicsItem() == 0 ) {
                } else
                if(startItem->countGraphicsItem() == 1 && startItem->isDiamond() == false){
                } else {
                    QMessageBox::warning(qobject_cast<QWidget*>( this->parent()), tr(""), tr("Not allowed to add a new transition!"));
                }
        }
    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

/*
void FEMachineScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(itemSelect != 0){
        CodeDialog *cd = qobject_cast<CodeDialog *>(codeDialog);
        if(itemSelect->mytype == 0)
            cd->setSelectItem("State neme: ", itemSelect->getName());
        if(itemSelect->mytype == 1){
            cd->setShowList(true);
            cd->setSelectItem("Transition neme: ", itemSelect->getName());
        }
        if(itemSelect->mytype == 3){
            cd->setShowList(true);
            cd->setSelectItem("Condition neme: ", itemSelect->getName());
        }
        cd->setShowSelectItem(true);
    }
}
*/

void FEMachineScene::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Delete && itemSelect != 0){
        if(itemSelect->mytype == 3){
            if(itemSelect->countGraphicsItem() == 2){
                FEGraphicsItem *state1 = new FEGraphicsItem;
                FEArrow *arrow1 = qgraphicsitem_cast<FEArrow*>(itemSelect->getGraphicsItem(0));
                itemSelect->removeGraphicsItem(arrow1);
                state1->setPos(itemSelect->pos() + QPointF(-50, 0));
                state1->addGraphicsItem(arrow1);
                arrow1->setStartItem(state1);
                addState(state1);

                FEGraphicsItem *state2 = new FEGraphicsItem;
                FEArrow *arrow2 = qgraphicsitem_cast<FEArrow*>(itemSelect->getGraphicsItem(0));
                itemSelect->removeGraphicsItem(arrow2);
                state2->setPos(itemSelect->pos() + QPointF(50, 0));
                state1->addGraphicsItem(arrow2);
                arrow2->setStartItem(state2);
                addState(state2);

                arrow1->updatePosition();
                arrow2->updatePosition();

                if(itemSelect->getGraphicsItemParents()->isDiamond())
                    itemSelect->getGraphicsItemParents()->setDiamond(false);

                FEArrow *arrow3 = qgraphicsitem_cast<FEArrow*>(itemSelect->getGraphicsItemParents()->getGraphicsItem(0));
                itemSelect->getGraphicsItemParents()->removeGraphicsItem(arrow3);

                removeItem(arrow3);
                delete arrow3;
                removeItem(itemSelect);
                delete itemSelect;
            } else if(itemSelect->countGraphicsItem() == 1){
                FEArrow *arrow1 = qgraphicsitem_cast<FEArrow*>(itemSelect->getGraphicsItem(0));

                if(itemSelect->getGraphicsItemParents()->isDiamond())
                    itemSelect->getGraphicsItemParents()->setDiamond(false);

                FEArrow *arrow2 = qgraphicsitem_cast<FEArrow*>(itemSelect->getGraphicsItemParents()->getGraphicsItem(0));
                itemSelect->getGraphicsItemParents()->removeGraphicsItem(arrow2);
                itemSelect->getGraphicsItemParents()->addGraphicsItem(arrow1);
                arrow1->setStartItem(itemSelect->getGraphicsItemParents());

                removeItem(arrow2);
                delete arrow2;
                removeItem(itemSelect);
                delete itemSelect;
            }
        }
        else if(itemSelect->mytype == 1){
            FEGraphicsItem *item;
            FEArrow *arrow = qgraphicsitem_cast<FEArrow*>(itemSelect->getGraphicsItem(0));
            itemSelect->removeGraphicsItem(arrow);
            item = arrow->getEndItem();
            if(item->mytype == 0 && item->countGraphicsItem() == 0){
                removeItem(item);
                delete item;
            }
            removeItem(arrow);
            delete arrow;
            arrow = 0;
            item = itemSelect->getGraphicsItemParents();
            qDebug()<<item->countGraphicsItem();
            int i;
            for(i = 0; i < item->countGraphicsItem() && arrow == 0;i++){
                FEArrow *a = qgraphicsitem_cast<FEArrow*>(item->getGraphicsItem(i));
                if(a->getEndItem() == itemSelect)
                    arrow = a;
            }
            item->removeGraphicsItem(arrow);
            removeItem(arrow);
            delete arrow;
            if(item->mytype == 3 && item->countGraphicsItem() == 0){

                if(item->getGraphicsItemParents()->isDiamond())
                    item->getGraphicsItemParents()->setDiamond(false);

                FEArrow *arrow1 = qgraphicsitem_cast<FEArrow*>(item->getGraphicsItemParents()->getGraphicsItem(0));
                item->getGraphicsItemParents()->removeGraphicsItem(arrow1);

                removeItem(arrow1);
                delete arrow1;
                removeItem(item);
                delete item;
            }
            removeItem(itemSelect);
            delete itemSelect;
        }
        qDebug()<<e->key();
    }
}

void FEMachineScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    moveMouse = true;
    changed = true;
    /* If a line is being drawn (i.e. right mouse button down */
    if (arc != 0) {
        /* Redraw the line to the new mouse position */
        arc->setStop(mouseEvent->scenePos());
        if(itemSelect->getColor() == Qt::blue)
        {
            itemSelect->setSelected(true);
            itemSelect->setColor(Qt::black);
        }
        QGraphicsItem * qitem = itemAt(mouseEvent->scenePos());
        if (qgraphicsitem_cast<FEGraphicsItem *>(qitem)) {
            FEGraphicsItem *sitem = qgraphicsitem_cast<FEGraphicsItem *>(qitem);
            if(sitem->mytype == State || sitem->mytype == FinalState || sitem->mytype == EmptyState ||
                    sitem->mytype == SolidState){
                if(itemArc != 0 && itemArc != sitem)
                {
                    itemArc->setColor(Qt::black);
                    itemArc = sitem;
                }
                else if(itemArc == 0)
                    itemArc = sitem;
                if(itemSelect == itemArc && testWHILE(itemSelect))
                {
                    itemSelect->setSelected(false);
                    itemSelect->setColor(Qt::blue);
                }
                else if(testIF(itemSelect, itemArc))
                    itemArc->setColor(Qt::blue);
            }
        }
        else if(itemArc != 0)
        {
            itemArc->setColor(Qt::black);
            itemArc = 0;
        }
        invalidate();
    } else {
        if(itemToMove != 0 && itemToMove->mytype == 0){
            itemToMove->setPos(mouseEvent->scenePos());
            QPointF p = mouseEvent->scenePos();
            QRectF r = itemToMove->boundingRect();
            //qDebug()<<r;
            r = QRectF(p.x() - r.width()/2.0, p.y() - r.height()/2.0, r.width(), r.height());
            QList<QGraphicsItem *> itemList = items(r, Qt::IntersectsItemBoundingRect);
            itemList.removeOne(itemToMove);
            //qDebug()<<"opo0"<<r<<p;
            FEGraphicsItem * it = 0;
            bool b = false;
            for(int i = 0;i < itemList.count();i++){
                if (qgraphicsitem_cast<FEGraphicsItem *>(itemList[i]) &&
                        qgraphicsitem_cast<FEGraphicsItem *>(itemList[i])->mytype == 0) {
                    it = qgraphicsitem_cast<FEGraphicsItem *>(itemList[i]);
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
            //qDebug()<<(itemToColaps !=0 )<< (itemToMove != 0);
        } else
            if(itemToMove != 0){
                itemToMove->setPos(mouseEvent->scenePos());
            }
        //QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void FEMachineScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if(moveMouse == false)
    {
    QMenu *m = new QMenu(this->views()[0]);
    QMenu *editMenu = 0;
    QAction *whileAction = 0;
    QAction *forAction = 0;
    QAction *refreshAction = new QAction(tr("Refresh"), this);
    m->addAction(refreshAction);
    QGraphicsItem * qitem = itemAt(event->scenePos());
    if(!moveMouse && qitem !=0 && (itemSelect = qgraphicsitem_cast<FEGraphicsItem *>(qitem)))
    {
        if(itemSelect->mytype == State || itemSelect->mytype == FinalState || itemSelect->mytype == Transition
                || itemSelect->mytype == ConditionIf || itemSelect->mytype == ConditionWhile
                || itemSelect->mytype == ConditionFor || itemSelect->mytype == ConditionWhileFor)
        {
            clearSelection();
            itemSelect->setSelected(true);
            m->addSeparator();
            m->addAction(itemSelect->getName());
            m->addSeparator();
            if(itemSelect->mytype == ConditionWhileFor)
            {
                editMenu = new QMenu(tr("Edit"), this->views()[0]);
                whileAction = new QAction(tr("while"), this);
                forAction = new QAction(tr("for"), this);
                editMenu->addAction(whileAction);
                editMenu->addAction(forAction);
                m->addMenu(editMenu);
                //connect(whileAction, SIGNAL(triggered()), this, SLOT(edit()));
                //connect(forAction, SIGNAL(triggered()), this, SLOT(edit()));
            }
            else
                m->addAction(editAction);
            if(itemSelect->mytype == Transition && itemSelect->getAssignmentList()->count() == 0)
                m->addAction(splitAction);
            qDebug()<<"QMenu iiiiiiiiiiooooooooooop[[[oooooooooooooo";
        }
    }
    QAction *a = m->exec(event->screenPos());
    if(whileAction != 0 && a == whileAction)
    {
        itemSelect->changeCondition(ConditionWhile);
        edit();
    }
    else if(forAction != 0 && a == forAction)
    {
        itemSelect->changeCondition(ConditionFor);
        edit();
    }
    bool b = a == refreshAction;
    if(b) arrangeGraphicsItem();
    if(whileAction != 0)
        delete whileAction;
    if(forAction != 0)
        delete forAction;
    if(editMenu != 0)
        delete editMenu;
    delete refreshAction;
    delete m;
    }
}

void FEMachineScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    //QGraphicsScene::mousePressEvent(mouseEvent);
    //mouseEvent->accept();
    //return;
    moveMouse = false;
    /* Make qitem equal to any graphics item at the mouse position */
    QGraphicsItem * qitem = itemAt(mouseEvent->scenePos());
    CodeDialog *cd = qobject_cast<CodeDialog *>(codeDialog);

    if(cd->isShowList() && cd->isCorrect() == false || cd->isShowSelectItem())
        return;

    /* If left mouse click proceed with event,
     * which is passed to scene items to select/move them */
    if (mouseEvent->button() == Qt::LeftButton) {
        /* If qitem is not null then make it selected */
        if (qitem != 0) {
            clearSelection();
            qitem->setSelected(true);
            //qDebug()<<"ok";
            if (qgraphicsitem_cast<FEGraphicsItem *>(qitem)) {
                itemToMove = qgraphicsitem_cast<FEGraphicsItem *>(qitem);
                    if(itemSelect == 0){
                        itemSelect = itemToMove;
                    } else if(itemToMove != itemSelect){
                        itemSelect = itemToMove;
                    }
            }

        }
        else if(itemSelect != 0){
            //if(cd->getShowList())
            //    cd->setShowList(false);
            //if(cd->getShowSelectItem())
            //    cd->setShowSelectItem(false);
            itemSelect = 0;
        }

        QGraphicsScene::mousePressEvent(mouseEvent);
    }

    /* If right mouse button start line */
    if (mouseEvent->button() == Qt::RightButton) {
        /* If there is an item at mouse position */
        if (qitem != 0) {
            /* If graphics item is a GraphicsItem */
            if (qgraphicsitem_cast<FEGraphicsItem *>(qitem)) {
                FEGraphicsItem * sitem =
                qgraphicsitem_cast<FEGraphicsItem *>(qitem);

                /* If the graphics item is a state */
                if(sitem->mytype == State || sitem->mytype == EmptyState || sitem->mytype == SolidState) {
                    if(itemSelect != sitem)
                    {
                        clearSelection();
                        itemSelect = sitem;
                        itemSelect->setSelected(true);
                    }
                    /* Create a line */
                    arc = new GraphicsArcItem(
                            mouseEvent->scenePos(), mouseEvent->scenePos());
                    arc->setPen(QPen(Qt::black, 1));
                    addItem(arc);
                }
            }
        }
    }
}

void FEMachineScene::addState(FEGraphicsItem * s) {
    if(s->mytype == State)
    {
        QString ss = QString("state %1").arg(num_states++);
        s->setName(ss);
        nameList.append(ss);
    }
    addItem(s);
}

void FEMachineScene::addTransitionItem(FEGraphicsItem * t) {
    QString s = QString("transition %1").arg(num_transitions++);
    t->setName(s);
    nameList.append(s);
    addItem(t);
}

void FEMachineScene::addCondition(FEGraphicsItem *t)
{
    QString s = QString("condition %1").arg(num_conditions++);
    t->setName(s);
    nameList.append(s);
    addItem(t);
}

void FEMachineScene::addArrow(FEArrow * t) {
    addItem(t);
    /* Draw arrows underneath states and functions */
    t->setZValue(-1.0);
}

void FEMachineScene::newState(FEGraphicsItem *state)
{
    if(state->mytype == SolidState)
    {
    }
}

bool FEMachineScene::check()
{
    bool ok = true;
    bool b = true;
    FEGraphicsItem *state = itemStart;
    do{
        if(state->mytype == 3)
        {
            if(state->countGraphicsItem() == 2)
            {
                FEArrow *a1 = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(0));
                FEArrow *a2 = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(1));
                FEGraphicsItem *r1 = check(a1->getEndItem());
                FEGraphicsItem *r2 = check(a2->getEndItem());
                if(r1 != 0 && r2 != 0 && r1 == r2)
                    state = r1;
                else
                    ok = b = false;
            }
            else
                ok = b = false;
        }
        else
            if(state->countGraphicsItem() > 0)
            {
                FEArrow *a = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(0));
                state = a->getEndItem();
            }
            else
                b = false;
    }while(b);
    return ok;
}

bool FEMachineScene::readFile(QString fileName)
{
    XMLFile *x = new XMLFile(fileName);
    bool ok = false;
    if(x->open(Read))
    {
        ok = true;
        QList<QGraphicsItem *> item = items();
        while(item.count() > 0)
        {
            QGraphicsItem *i = item.first();
            removeItem(i);
            delete i;
            item.removeAt(0);
        }
        QList<FEGraphicsItem*> itemsList;
        x->read(itemsList);
        CodeDialog *cd = qobject_cast<CodeDialog *>(codeDialog);
        cd->setFunctionName(x->getFunctionName());
        num_states = 0;
        num_transitions = 0;
        num_conditions = 0;
        nameList.clear();
        if(itemsList.count() > 0)
            itemStart = itemsList.first();
        itemStart->setPos(100, 100);
        while(itemsList.count() > 0)
        {
            FEGraphicsItem *i = itemsList.first();
            itemsList.removeAt(0);
            addItem(i);
            for(int j=0;j<i->countGraphicsItem();j++)
            {
                FEArrow *a = qgraphicsitem_cast<FEArrow *>(i->getGraphicsItem(j));
                addArrow(a);
            }
            if(i->mytype == State)
            {
                num_states++;
                nameList.append(i->getName());
            }
            if(i->mytype == Transition)
            {
                num_transitions++;
                nameList.append(i->getName());
            }
            if(i->mytype == ConditionIf || i->mytype == ConditionWhile || i->mytype == ConditionFor)
            {
                num_conditions++;
                nameList.append(i->getName());
            }
        }
        arrangeGraphicsItem();
        changed = false;
    }
    delete x;
    return ok;
}

FEGraphicsItem *FEMachineScene::check(FEGraphicsItem *g)
{
    bool b = true;
    FEGraphicsItem *state = g;
    do{
        if(state->countGraphicsItemParents() > 1)
            b = false;
        else
            if(state->mytype == 3)
            {
                if(state->countGraphicsItem() == 2)
                {
                    FEArrow *a1 = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(0));
                    FEArrow *a2 = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(1));
                    FEGraphicsItem *r1 = check(a1->getEndItem());
                    FEGraphicsItem *r2 = check(a2->getEndItem());
                    if(r1 != 0 && r2 != 0 && r1 == r2)
                        state = r1;
                    else
                    {
                        state = 0;
                        b = false;
                    }
                }
                else
                {
                    state = 0;
                    b = false;
                }
            }
            else
                if(state->countGraphicsItem() > 0)
                {
                    FEArrow *a = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(0));
                    state = a->getEndItem();
                }
                else
                {
                    state = 0;
                    b = false;
                }
    }while(b);
    return state;
}

bool FEMachineScene::saveFile(QString fileName, QString sFunctionName, int type)
{
    FileType *x = 0;
    if(type == 0)
        x = new XMLFile(fileName, sFunctionName);
    else
        x = new CFile(fileName, sFunctionName);
    bool ok = false;
    if(x->open(Write))
    {
        ok = true;
        bool b = true;
        FEGraphicsItem *state = itemStart;
        map = new QMap<FEGraphicsItem *, int>();
        saveFile(itemStart, x);
        delete map;
        if(type == 0)
            changed = false;
    }
    delete x;
    return ok;
}

FEGraphicsItem *FEMachineScene::saveFile(FEGraphicsItem *g, FileType *x)
{
    QStack<FEGraphicsItem*> stack;
    bool b = true;
    FEGraphicsItem *state = g;
    do{
        if(state->countGraphicsItemParents() > 1)
        {
            if(map->contains(state))
            {
                int i = map->value(state);
                qDebug()<<map->contains(state)<<i;
                if(i > 0)
                {
                    {
                        map->remove(state);
                        map->insert(state, i - 1);
                        b = false;
                    }
                }
            }
            else
            {
                map->insert(state, state->countGraphicsItemParents() * 2 - 3);
                b = false;
            }
        }
        if(b)
        {
            if(state->mytype == ConditionIf)
            {
                    //x->writeState(state);
                    //state = ((Arrow*)(qgraphicsitem_cast<Arrow *>(state->getGraphicsItem(0))))->getEndItem();
                    //qDebug()<<state->mytype;
                    FEArrow *a1 = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(0));
                    FEArrow *a2 = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(1));
                    x->writeStartIF(state);
                    x->writeStartTrue();
                    FEGraphicsItem *r2 = saveFile(a2->getEndItem(), x);
                    x->writeStopTrue();
                    x->writeStartFalse();
                    FEGraphicsItem *r1 = saveFile(a1->getEndItem(), x);
                    x->writeStopFalse();
                    x->writeStopIF();
                    state = r1;
                    //qDebug()<<state->mytype;
                    //saveFile(state, x);
            }
            else if(state->mytype == ConditionWhile)
            {
                FEArrow *a1 = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(0));
                stack.push(a1->getEndItem());
                FEArrow *a2 = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(1));
                x->writeStartWHILE(state);
                FEGraphicsItem *r1 = saveFile(a2->getEndItem(), x);
                state = r1;
                state->setName("while state");
                x->writeEndWHILE(state);
                if(stack.isEmpty() == false)
                    state = stack.pop();
            }
            else if(state->mytype == ConditionFor)
            {
                FEArrow *a1 = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(0));
                stack.push(a1->getEndItem());
                FEArrow *a2 = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(1));
                x->writeStartFOR(state);
                FEGraphicsItem *r1 = saveFile(a2->getEndItem(), x);
                state = r1;
                state->setName("for state");
                x->writeEndFOR(state);
                if(stack.isEmpty() == false)
                    state = stack.pop();
            }
            else if(state->mytype == SolidState)
                b = false;
            else
            {
                if(state->mytype == State || state->mytype == FinalState)
                    x->writeState(state);
                else
                    if(state->mytype == Transition)
                        x->writeTransition(state);
                for(int i=0;i < state->countGraphicsItem();i++)
                {
                    FEArrow *a = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(i));
                    stack.push(a->getEndItem());
                }
                if(stack.isEmpty() == false)
                    state = stack.pop();
                else
                    b = false;
            }
        }
    }while(b);
    return state;
}

void FEMachineScene::arrangeGraphicsItem()
{
    map = new QMap<FEGraphicsItem *, int>();
    GraphicsGrup *grup = new GraphicsGrup();
    arrangeGraphicsItem(*grup, itemStart);
    delete map;
    delete grup;
    invalidate();
}

FEGraphicsItem* FEMachineScene::arrangeGraphicsItem(GraphicsGrup &item, FEGraphicsItem *g)
{
    bool b = true;
    FEGraphicsItem *state = g;
    do{
        if(state->countGraphicsItemParents() > 1)
        {
            if(map->contains(state))
            {
                int i = map->value(state);
                qDebug()<<map->contains(state)<<i;
                if(i > 0)
                {
                    {
                        map->remove(state);
                        map->insert(state, i - 1);
                        b = false;
                    }
                }
            }
            else
            {
                map->insert(state, state->countGraphicsItemParents() * 2 - 3);
                b = false;
            }
        }
        qDebug()<<state->countGraphicsItem();
        if(b)
        {
            if(state->mytype == ConditionIf)
            {
                item.add(state);
                    FEArrow *a1 = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(0));
                    FEArrow *a2 = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(1));
                    GraphicsGrup *g1 = new GraphicsGrup(true);
                    FEGraphicsItem *r1 = arrangeGraphicsItem(*g1, a1->getEndItem());
                    g1->finalArrangement();
                    GraphicsGrup *g2 = new GraphicsGrup(true);
                    FEGraphicsItem *r2 = arrangeGraphicsItem(*g2, a2->getEndItem());
                    g2->finalArrangement();
                    item.add(*g1, *g2);
                    delete g1;
                    delete g2;
                    state = r1;
            }
            else if(state->mytype == ConditionWhileFor || state->mytype == ConditionWhile || state->mytype == ConditionFor)
            {
                item.add(state);
                FEArrow *a1 = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(0));
                FEArrow *a2 = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(1));
                //GraphicsGrup *g1 = new GraphicsGrup(true);
                //GraphicsItem *r1 = arrangeGraphicsItem(*g1, a1->getEndItem());
                //g1->finalArrangement();
                GraphicsGrup *g2 = new GraphicsGrup(true);
                FEGraphicsItem *r2 = arrangeGraphicsItem(*g2, a2->getEndItem());
                g2->finalArrangement();
                item.add(*g2);
                state = a1->getEndItem();
                //b=false;
            }
            else if(state->mytype == SolidState)
            {
                item.add(state);
                b = false;
            }
            else
            {
                if(state->countGraphicsItem() == 1)
                {
                    FEArrow *a = qgraphicsitem_cast<FEArrow *>(state->getGraphicsItem(0));
                    item.add(state);
                    state = a->getEndItem();
                }
                else
                {
                    item.add(state);
                    b = false;
                }
            }
        }
    }while(b);
    return state;
}

void FEMachineScene::edit()
{
    if(itemSelect != 0){
        CodeDialog *cd = qobject_cast<CodeDialog *>(codeDialog);
        QString s = itemSelect->getName();
        namePosition = nameList.indexOf(s);
        if(itemSelect->mytype == State || itemSelect->mytype == FinalState)
            cd->setSelectItem("State neme: ", s);
        if(itemSelect->mytype == Transition){
            cd->setShowList(true);
            cd->setSelectItem("Transition neme: ", s);
        }
        if(itemSelect->mytype == ConditionIf || itemSelect->mytype == ConditionWhile || itemSelect->mytype == ConditionFor){
            cd->setShowList(true);
            cd->setSelectItem("Condition neme: ", s);
        }
        cd->setShowSelectItem(true);
        changed = true;
    }
    qDebug()<<"edit";
}

void FEMachineScene::newTransition(FEGraphicsItem *state)
{
    FEGraphicsItem *transition = new FEGraphicsItem(Transition);
    FEGraphicsItem *state1 = new FEGraphicsItem(FinalState, "Final");
    QPointF p = state->pos();
    QRectF r = state->boundingRect();
    addTransitionItem(transition);
    addItem(state1);
    FEArrow *arrow1 = new FEArrow(state ,transition);
    addArrow(arrow1);
    state->addGraphicsItem(arrow1);
    FEArrow *arrow2 = new FEArrow(transition, state1);
    addArrow(arrow2);
    transition->addGraphicsItem(arrow2);
    transition->addGraphicsItemParents(state);
    state1->addGraphicsItemParents(transition);
}

void FEMachineScene::addTransition(FEGraphicsItem *item)
{
    FEArrow *arrow = qgraphicsitem_cast<FEArrow*>(item->getGraphicsItem(0));
    FEGraphicsItem *oldState = arrow->getEndItem();
    FEGraphicsItem *newTransition = new FEGraphicsItem(Transition);
    FEGraphicsItem *newState = new FEGraphicsItem;
    arrow->setEndItem(newState);
    arrow->updatePosition();
    FEArrow *arrow1 = new FEArrow(newState ,newTransition);
    addArrow(arrow1);
    newState->addGraphicsItem(arrow1);
    FEArrow *arrow2 = new FEArrow(newTransition, oldState);
    addArrow(arrow2);
    newTransition->addGraphicsItem(arrow2);
    newState->setGraphicsItemParents(item);
    newTransition->setGraphicsItemParents(newState);
    oldState->setGraphicsItemParents(newTransition);
    addState(newState);
    addTransitionItem(newTransition);
}

void FEMachineScene::addIF(FEGraphicsItem *state, bool place)
{
    FEArrow *arrow = qgraphicsitem_cast<FEArrow*>(state->getGraphicsItem(0));
    FEGraphicsItem *transition = arrow->getEndItem();
    FEGraphicsItem *stateFinal = ((FEArrow*)qgraphicsitem_cast<FEArrow*>(transition->getGraphicsItem(0)))->getEndItem();
    stateFinal->removeGraphicsItemParents(transition);
    FEArrow *a = qgraphicsitem_cast<FEArrow*>(transition->getGraphicsItem(0));
    transition->removeGraphicsItem(a);
    removeItem(a);
    delete a;
    FEGraphicsItem *transitionTrue = 0;
    FEGraphicsItem *transitionFalse = 0;
    if(place)
    {
        transitionTrue = new FEGraphicsItem(Transition);
        transitionFalse = transition;
        addTransitionItem(transitionTrue);
    }
    else
    {
        transitionTrue = transition;
        transitionFalse = new FEGraphicsItem(Transition);
        addTransitionItem(transitionFalse);
    }
    FEGraphicsItem *condition = new FEGraphicsItem(ConditionIf);
    arrow->setEndItem(condition);
    condition->setGraphicsItemParents(state);
    transitionTrue->setGraphicsItemParents(condition);
    transitionFalse->setGraphicsItemParents(condition);
    FEGraphicsItem *stateTrue = new FEGraphicsItem(EmptyState);
    addState(stateTrue);
    FEGraphicsItem *stateFalse = new FEGraphicsItem(EmptyState);
    addState(stateFalse);
    FEArrow *arrowStateTrue = new FEArrow(condition, stateTrue);
    FEArrow *arrowTransitionTrue = new FEArrow(stateTrue, transitionTrue);
    FEArrow *arrowStateFalse = new FEArrow(condition, stateFalse);
    FEArrow *arrowTransitionFalse = new FEArrow(stateFalse, transitionFalse);
    FEArrow *arrow2 = new FEArrow(transitionTrue, stateFinal);
    FEArrow *arrow3 = new FEArrow(transitionFalse, stateFinal);
    arrowStateTrue->setName("false");
    arrowStateFalse->setName("true");
    condition->addGraphicsItem(arrowStateTrue);
    stateTrue->addGraphicsItem(arrowTransitionTrue);
    transitionTrue->addGraphicsItem(arrow2);
    //transitionTrue->addGraphicsItemParents(stateTrue);
    condition->addGraphicsItem(arrowStateFalse);
    stateFalse->addGraphicsItem(arrowTransitionFalse);
    transitionFalse->addGraphicsItem(arrow3);
    //transitionFalse->addGraphicsItemParents(stateFalse);
    stateFinal->addGraphicsItemParents(transitionTrue);
    stateFinal->addGraphicsItemParents(transitionFalse);
    addCondition(condition);
    addArrow(arrowStateTrue);
    addArrow(arrowStateFalse);
    addArrow(arrow2);
    addArrow(arrow3);
    addArrow(arrowTransitionTrue);
    addArrow(arrowTransitionFalse);
}

void FEMachineScene::addWHILEorFOR(FEGraphicsItem *state)
{
    FEArrow *arrow = qgraphicsitem_cast<FEArrow*>(state->getGraphicsItem(0));
    FEGraphicsItem *stateOld = arrow->getEndItem();
    stateOld->removeGraphicsItemParents(state);
    FEGraphicsItem *condition = new FEGraphicsItem(ConditionWhileFor);
    FEGraphicsItem *stateConditionEmpty = new FEGraphicsItem(EmptyState);
    FEGraphicsItem *transition = new FEGraphicsItem(Transition);
    FEGraphicsItem *stateSolid = new FEGraphicsItem(SolidState, "while state");
    FEGraphicsItem *stateEmpty = new FEGraphicsItem(EmptyState);

    FEArrow *arrowEmptyState = new FEArrow(condition, stateEmpty);
    arrowEmptyState->setName("exit");
    FEArrow *arrowOldState = new FEArrow(stateEmpty, stateOld);
    FEArrow *arrow2 = new FEArrow(condition, stateConditionEmpty);
    FEArrow *arrow3 = new FEArrow(stateConditionEmpty, transition);
    FEArrow *arrow4 = new FEArrow(transition, stateSolid);

    arrow->setEndItem(condition);
    //state->addGraphicsItem(arrowOldState);
    condition->addGraphicsItem(arrowEmptyState);
    condition->addGraphicsItem(arrow2);
    stateConditionEmpty->addGraphicsItem(arrow3);
    transition->addGraphicsItem(arrow4);

    stateEmpty->addGraphicsItem(arrowOldState);

    condition->setGraphicsItemParents(state);
    stateConditionEmpty->addGraphicsItemParents(condition);
    transition->setGraphicsItemParents(stateConditionEmpty);
    stateSolid->setGraphicsItemParents(transition);

    stateEmpty->addGraphicsItemParents(condition);
    stateOld->addGraphicsItemParents(stateEmpty);

    addCondition(condition);
    addState(stateConditionEmpty);
    addTransitionItem(transition);
    addItem(stateSolid);
    addState(stateEmpty);
    addArrow(arrowEmptyState);
    addArrow(arrowOldState);
    addArrow(arrow2);
    addArrow(arrow3);
    addArrow(arrow4);
}

bool FEMachineScene::testIF(FEGraphicsItem *stateStart, FEGraphicsItem *stateStop)
{
    bool b = stateStart->mytype == State || stateStart->mytype == EmptyState;
    if(b)
    {
        if(stateStart->countGraphicsItem() == 0)
            return false;
        FEGraphicsItem *state = ((FEArrow*)(qgraphicsitem_cast<FEArrow*>(stateStart->getGraphicsItem(0))))->getEndItem();
        b = state->mytype == Transition;
        if(b)
        {
            if(stateStart->countGraphicsItem() == 0)
                return false;
            state = ((FEArrow*)(qgraphicsitem_cast<FEArrow*>(state->getGraphicsItem(0))))->getEndItem();
            b = state->mytype == State || state->mytype == FinalState || state->mytype == SolidState;
            if(b)
            {
                b = state == stateStop;
            }
        }
    }
    return b;
}

bool FEMachineScene::testWHILE(FEGraphicsItem *stateStart)
{
    bool b = stateStart->mytype == State || stateStart->mytype == EmptyState || stateStart->mytype == SolidState;
    if(b)
    {
        //qDebug()<<stateStart->countGraphicsItem();
        b = stateStart->countGraphicsItem() == 1;
    }
    return b;
}

void FEMachineScene::split()
{
    addTransition(itemSelect);
    arrangeGraphicsItem();
    changed = true;
    //arrangeGraphicsItem();invalidate();
    qDebug()<<"split";
}
