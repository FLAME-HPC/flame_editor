#ifndef ADT_H
#define ADT_H

#include <QString>
#include <QList>
#include "variable.h"

class ADT
{
public:
    ADT(QString n = "", QString d = "");

    //void addVariable(QString n, ADT t);
    //void addFundamentalVariable(QString n, int s);

private:
    QString name;
    QString desc;
    //void * data;
    QList<Variable> variables;
};

#endif // ADT_H
