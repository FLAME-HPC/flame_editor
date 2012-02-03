/*!
 * \file adt.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for model datatypes
 */
#ifndef ADT_H_
#define ADT_H_

#include <QString>
#include <QList>
#include "./variable.h"

/*!
 * \brief Model data type class
 *
 * This class holds the information about a user defined abstract data type.
 */
class ADT {
  public:
    ADT(QString = "", QString = "");

  private:
    QString name;   /*!< \brief The name of the data type */
    QString desc;   /*!< \brief The description of the data type */
    /*! \brief The list of variables of the data type */
    QList<Variable> variables;
};

#endif  // ADT_H_
