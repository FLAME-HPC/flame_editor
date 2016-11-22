/*!
 * \file state.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for machine state
*/
#ifndef STATE_H_
#define STATE_H_

#include <QString>

class State {
  public:
    State() { myName = ""; }
    explicit State(QString n) { myName = n; }

    QString name() const { return myName; }
    void setName(QString n) { myName = n; }

  private:
    QString myName;
};

#endif  // STATE_H_
