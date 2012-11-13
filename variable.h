/*!
 * \file variable.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for memory variable
*/
#ifndef VARIABLE_H_
#define VARIABLE_H_

#include <QString>

// class ADT;

class Variable {
  public:
    Variable();
    Variable(QString n, QString t);
    double getValue();
    void setValue(double v);

    QString name;
    QString type;
    QString description;
    bool constant;
    int ivalue;
    double dvalue;
};

#endif  // VARIABLE_H_
