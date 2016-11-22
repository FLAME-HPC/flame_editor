/*!
 * \file memoryvariable.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of memory variable
 */
#include "./memoryvariable.h"

MemoryVariable::MemoryVariable(QString n, QString d, ADT t, int s) {
    myName = n;
    myDesc = d;
    myType = t;
    mySize = s;
    myValue = 0;
}
