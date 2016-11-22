/*!
 * \file timeunit.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of time unit
 */
#include "./timeunit.h"

TimeUnit::TimeUnit() {
    unit = "iteration";
    period = 1;
}

QString TimeUnit::toString() {
    QString s;
    s.append(name);
    s.append(", ");
    s.append(unit);
    s.append(", ");
    s.append(QString::number(period));
    return s;
}
