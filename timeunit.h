/*!
 * \file timeunit.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for time unit
*/
#ifndef TIMEUNIT_H_
#define TIMEUNIT_H_

#include <QString>
#include <QMetaType>

class TimeUnit {
  public:
    enum EditMode { Editable, ReadOnly };

    TimeUnit();

    QString toString();

    QString name;
    QString unit;
    int period;
};

Q_DECLARE_METATYPE(TimeUnit)

#endif  // TIMEUNIT_H_
