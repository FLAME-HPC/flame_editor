#ifndef CODEPARSER_H
#define CODEPARSER_H

#include <QStringList>
#include <QStack>

#include "./xmlfile.h"

struct Token{
public:
    Token():token(""), val(0){}
    Token(QString t, int v):token(t), val(v){}
    QString token;
    int val;
};

struct TokenTest{
public:
    TokenTest():token(""), val(0), nr(0){}
    TokenTest(QString t, int v, int n):token(t), val(v), nr(n){}
    QString token;
    int val;
    int nr;
};

class CodeParser
{
public:
    CodeParser(XMLFile *x);
    ~CodeParser();
    bool execute(QString statement);
    QList<Token> getTokensList(QString s);
    static QStringList getIdentifierList(QString s);
    static int setParseToTest(QString s);
    static QString getToken();

private:
    XMLFile *xml;
};


#endif // CODEPARSER_H
