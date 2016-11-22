/*!
 * \file messagecomm.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for a message communication
*/
#ifndef MESSAGECOMM_H_
#define MESSAGECOMM_H_

#include <QString>
#include "./condition.h"
#include "./messagesort.h"

class MessageComm {
  public:
    MessageComm();
    explicit MessageComm(QString m);

    QString toString();

    QString messageType;
    Condition filter;
    MessageSort sort;
};

#endif  // MESSAGECOMM_H_
