/*!
 * \file transition.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for transition function
*/
#ifndef TRANSITION_H_
#define TRANSITION_H_

#include "./mpre.h"
#include "./mpost.h"
#include "./state.h"
#include "./memorymodel.h"
#include "./condition.h"
#include "./communication.h"

class Transition {
  public:
    Transition();
    Transition(State * cs, QString n, State * ns);
    Transition(State * cs, Communication i, Mpre pre,
        QString n, Mpost post, Communication o, State * ns);

    void setCurrentState(State * cs) { myCurrentState = cs; }
    State * currentState() const { return myCurrentState; }
    void setInput(Communication i) { myInput = i; }
    Communication input() const { return myInput; }
    void setMpre(Mpre pre) { myMpre = pre; }
    Mpre mpre() const { return myMpre; }
    void setName(QString n) { myName = n; }
    QString name() const { return myName; }
    void setMpost(Mpost post) { myMpost = post; }
    Mpost mpost() const { return myMpost; }
    void setOutput(Communication o) { myOutput = o; }
    Communication output() const { return myOutput; }
    void setNextState(State * ns) { myNextState = ns; }
    State * nextState() const { return myNextState; }
    Mpre * getMprePointer() { return &myMpre; }
    void setCondition(Condition c) { myCondition = c; }
    Condition condition() const { return myCondition; }
    void setDescription(QString s) { myDescription = s; }
    QString description() const { return myDescription; }

    bool passesCondition(MemoryModel * memory);
    void updateMemory(MemoryModel * memory);

  private:
    State * myCurrentState;
    Communication myInput;
    Mpre myMpre;
    QString myName;
    Mpost myMpost;
    Communication myOutput;
    State * myNextState;
    Condition myCondition;
    QString myDescription;
};

#endif  // TRANSITION_H_
