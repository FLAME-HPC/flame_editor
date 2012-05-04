/*!
 * \file variable.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of model memory variable
 */
#include "./variable.h"

Variable::Variable() {
    description = "";
    constant = false;
}

Variable::Variable(QString n, QString t) {
    this->name = n;
    this->type = t;
    ivalue = 0;
    dvalue = 0.0;
    description = "";
    constant = false;
}

double Variable::getValue() {
    if (type == "int") return static_cast<double>(ivalue);
    if (type == "double") return dvalue;
    else
        return 0.0;
}

void Variable::setValue(double v) {
    ivalue = static_cast<int>(v);
    dvalue = v;
}
