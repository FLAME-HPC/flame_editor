/*!
 * \file communication.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of message communication
 */
#include "./communication.h"

Communication::Communication() {
    messageModel = new MessageModel(1);
    flag = 1;
}

Communication::Communication(int f) {
    messageModel = new MessageModel(f);
    flag = f;
}

QString Communication::toString() const {
    QString text;
    for (int i = 0; i < messageModel->rowCount(); i++) {
       text.append(messageModel->getMessages()[i].toString());
       text.append("\n");
    }
    return text;
}
