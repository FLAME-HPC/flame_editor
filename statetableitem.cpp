#include "statetableitem.h"

StateTableItem::StateTableItem(StateItem * s)
{
    state = s;
    setText(s->getName());
}

void StateTableItem::setState(StateItem * si)
{
    state = si;
    setText(state->getName());
}

StateItem* StateTableItem::getState()
{
    return state;
}

void StateTableItem::setStateName(QString n)
{
    state->setName(n);
}
