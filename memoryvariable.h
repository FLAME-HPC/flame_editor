#ifndef MEMORYVARIABLE_H
#define MEMORYVARIABLE_H

#include <QString>
#include "adt.h"

class MemoryVariable
{
public:
    MemoryVariable(QString n = "", QString d = "", ADT t = ADT(), int s = 1);

    void setName(QString n) { myName = n; }
    QString name() const { return myName; }
    void setType(ADT t) { myType = t; }
    ADT type() const { return myType; }
    void setValue(void * v) { myValue = v; }
    void * value() const { return myValue; }
    void setSize(int s) { mySize = s; }
    int size() const { return mySize; }
    void setDesc(QString d) { myDesc = d; }
    QString desc() const { return myDesc; }

private:
    QString myName;
    QString myDesc;
    ADT myType;
    void * myValue;
    int mySize; /* array size */
};

#endif // MEMORYVARIABLE_H
