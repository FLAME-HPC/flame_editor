#ifndef CEVARIABLE_H
#define CEVARIABLE_H

#include <QString>

class CEVariable
{
public:
    CEVariable();
    CEVariable(QString n, QString t) { this->name = n; this->type = t; ivalue = 0; dvalue = 0.0; }
    double getValue();
    void setValue(double v);

    QString name;
    QString type;
    int ivalue;
    double dvalue;
};

#endif // VARIABLE_H
