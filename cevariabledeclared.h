#ifndef CEVARIABLEDECLARED_H
#define CEVARIABLEDECLARED_H

#include <QString>

class CEVariableDeclared
{
public:
    CEVariableDeclared();
    CEVariableDeclared(QString t, QString n, QString e) {type = t; name = n; expression = e;}

    QString getType() const { return type;}
    void setType(QString t) { type = t;}
    QString getName() const { return name;}
    void setName(QString n) { name = n;}
    QString getExpression() const { return expression;}
    void setExpression(QString e) { expression = e;}
private:
    QString type;
    QString name;
    QString expression;
};

#endif // VARIABLEDECLARED_H
