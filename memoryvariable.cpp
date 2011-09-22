#include "memoryvariable.h"

MemoryVariable::MemoryVariable(QString n, QString d, ADT t, int s)
{
    myName = n;
    myDesc = d;
    myType = t;
    mySize = s;
    myValue = 0;
}
