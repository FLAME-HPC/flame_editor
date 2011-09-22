#ifndef STATE_H
#define STATE_H

#include <QString>

class State
{
public:
    State() { myName = ""; start = false; }
    State(QString n) { myName = n; start = false; }
    State(QString n, bool s) { myName = n; start = s; }

    QString name() const { return myName; }
    void setName(QString n) { myName = n; }
    bool startState() const { return start; }
    void setStartState(bool s) { start = s; }

private:
    QString myName;
    bool start;
};

#endif // STATE_H
