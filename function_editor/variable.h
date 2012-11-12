#ifndef VARIABLE_H
#define VARIABLE_H

#include <QString>

class Variable
{
public:
    Variable();
    Variable(QString n, QString t) { this->name = n; this->type = t; ivalue = 0; dvalue = 0.0; }
    double getValue();
    void setValue(double v);

    QString name;
    QString type;
    int ivalue;
    double dvalue;
};

#endif // VARIABLE_H
