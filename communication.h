/*!
 * \file communication.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for message communication
 */
#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include <QMetaType>
#include <QPainter>
#include <QStringList>
#include "./messagemodel.h"

class Communication {
  public:
    enum EditMode { Editable, ReadOnly };

    Communication();
    explicit Communication(int flag);

    QString toString() const;

    MessageModel * messageModel;
    int flag;  // 1 for input, 0 for output
};

Q_DECLARE_METATYPE(Communication)

#endif  // COMMUNICATION_H_
