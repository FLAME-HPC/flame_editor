#ifndef STATETABLEITEM_H
#define STATETABLEITEM_H

#include <QTableWidgetItem>
#include "stateitem.h"

class StateTableItem : public QTableWidgetItem
{
public:
    enum { Type = UserType + 16 };
    StateTableItem(StateItem * s);
    void setState(StateItem * si);
    StateItem* getState();
    void setStateName(QString n);

private:
    StateItem * state;
};

#endif // STATETABLEITEM_H
