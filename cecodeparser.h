#ifndef CECODEPARSER_H
#define CECODEPARSER_H

#include <QStringList>
#include <QStack>

#include "./cexmlfile.h"

struct CEToken{
public:
    CEToken():token(""), val(0){}
    CEToken(QString t, int v):token(t), val(v){}
    QString token;
    int val;
};

struct CETokenTest{
public:
    CETokenTest():token(""), val(0), nr(0){}
    CETokenTest(QString t, int v, int n):token(t), val(v), nr(n){}
    QString token;
    int val;
    int nr;
};

class CECodeParser
{
public:
    CECodeParser(CEXMLFile *x);
    ~CECodeParser();
    bool execute(QString statement);
    QList<CEToken> getTokensList(QString s);
    static QStringList getIdentifierList(QString s);
    static int setParseToTest(QString s);
    static QString getToken();
    static int getResult();

private:
    CEXMLFile *xml;
};


#endif // CODEPARSER_H
