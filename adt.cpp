/*!
 * \file adt.cpp
 *  \author Simon Coakley
 *  \date 2012
 *  \copyright Copyright (c) 2012 University of Sheffield
 *  \brief Implementation of model data type
 */
#include "./adt.h"

/*!
 * \brief Model data type constructor
 * \param[in] n The data type name, default ""
 * \param[in] d The data type description, default ""
 *
 * This contructor takes a name and a description string.
 * If either are missing they default to an empty string.
 */
ADT::ADT(QString n, QString d) {
    name = n;
    desc = d;
}
