/*!
 * \file messagecomm.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of message communication
 */
#include "./messagecomm.h"

MessageComm::MessageComm() {
    messageType = "";
}

MessageComm::MessageComm(QString m) {
    messageType = m;
}

QString MessageComm::toString() {
    QString s;
    s.append(messageType);
    if (this->filter.enabled) {
        s.append(" filter(");
        s.append(filter.toString());
        s.append(")");
    }
    if (sort.isRandom) s.append(" random");
    if (sort.isSort) {
        s.append(" sort(");
        s.append(sort.key);
        s.append(", ");
        s.append(sort.order);
        s.append(")");
    }
    return s;
}
