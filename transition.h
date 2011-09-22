#ifndef TRANSITION_H
#define TRANSITION_H

#include "mpre.h"
#include "mpost.h"
#include "state.h"
#include "memorymodel.h"

class Transition
{
public:
    Transition() { myInput = ""; myName = ""; myOutput = ""; }
    Transition(State * cs, QString n, State * ns)
    { myCurrentState = cs; myInput = ""; myMpre = Mpre(), myName = n; myMpost = Mpost(), myOutput = ""; myNextState = ns; }
    Transition(State * cs, QString i, Mpre pre, QString n, Mpost post, QString o, State * ns)
    { myCurrentState = cs; myInput = i; myMpre = pre; myName = n; myMpost = post; myOutput = o; myNextState = ns; }

    void setCurrentState(State * cs) { myCurrentState = cs; }
    State * currentState() const { return myCurrentState; }
    void setInput(QString i) { myInput = i; }
    QString input() const { return myInput; }
    void setMpre(Mpre pre) { myMpre = pre; }
    Mpre mpre() const { return myMpre; }
    void setName(QString n) { myName = n; }
    QString name() const { return myName; }
    void setMpost(Mpost post) { myMpost = post; }
    Mpost mpost() const { return myMpost; }
    void setOutput(QString o) { myOutput = o; }
    QString output() const { return myOutput; }
    void setNextState(State * ns) { myNextState = ns; }
    State * nextState() const { return myNextState; }
    Mpre * getMprePointer() { return &myMpre; }

    bool passesCondition(MemoryModel * memory);
    void updateMemory(MemoryModel * memory);

private:
    State * myCurrentState;
    QString myInput;
    Mpre myMpre;
    QString myName;
    Mpost myMpost;
    QString myOutput;
    State * myNextState;
};

#endif // TRANSITION_H
