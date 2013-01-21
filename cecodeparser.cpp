#include "cecodeparser.h"
#include "parser/y.tab.h"

#include <QDebug>

extern "C" {

  char *yytext;
  int textToParse(char *p);
  void textToScanner(char *p);
  int yylex();

}

CECodeParser::CECodeParser(CEXMLFile *x):xml(x)
{
}

CECodeParser::~CECodeParser()
{
}

bool CECodeParser::execute(QString statement)
{
    int ii = textToParse(statement.toLocal8Bit().data());
    bool b = ii == 0;
    if(b)
    {
        QList<CEToken> tokensList = getTokensList(statement);
        QStack<CETokenTest> stack;
        while(tokensList.length() > 0)
        {
            CEToken currentToken = tokensList[0];
            tokensList.removeAt(0);
            if(currentToken.val == IDENTIFIER)
            {
                if(tokensList.length() > 0 && tokensList[0].token == "(")
                {
                    tokensList.removeAt(0);
                    xml->writeStartElement(CEXMLConstants::SFUNCTION);
                    xml->writeTextElement(CEXMLConstants::SNAME, currentToken.token);
                    xml->writeStartElement(CEXMLConstants::SPARAMETERS);
                    if(tokensList.length() > 0 && tokensList[0].token == ")")
                        stack.push(CETokenTest(")", 1, 2));
                    else
                    {
                        xml->writeStartElement(CEXMLConstants::SPARAMETER);
                        stack.push(CETokenTest(")", 1, 3));
                    }
                }
                else
                    xml->writeTextElement(CEXMLConstants::SIDENTIFIER, currentToken.token);
            }
            else if(currentToken.token == "(")
            {
                xml->writeStartElement(CEXMLConstants::SPARENTHESIS);
                stack.push(CETokenTest(")", 0, 1));
            }
            else if(currentToken.token == ")" && stack.isEmpty() == false && stack.top().token == ")")
            {
                CETokenTest tt = stack.pop();
                for(int i=0;i<tt.nr;i++)
                    xml->writeStopElement();
            }
            else if(currentToken.token == ",")
            {
                xml->writeStopElement();
                xml->writeStartElement(CEXMLConstants::SPARAMETER);
            }
            else if(xml->containsOperator(currentToken.token))
            {
                xml->writeTextElement(CEXMLConstants::SOP, xml->getOperators(currentToken.token));
            }
            else if(currentToken.val == INTEGER || currentToken.val == FLOATING)
            {
                xml->writeTextElement(CEXMLConstants::SNUMBER, currentToken.token);
            }
            qDebug()<<currentToken.token;
        }
    }
    return b;
}

QList<CEToken> CECodeParser::getTokensList(QString s)
{
    QList<CEToken> list;
    QString a, t;
    int i;
    textToScanner(s.toLocal8Bit().data());
    while(i = yylex())
    {
        t = QString(yytext);
        //qDebug()<<t;
        if(i == IDENTIFIER)
            a.append(t);
        else if(t == ".")
            a.append(".");
        else
        {
            if(a.length() > 0)
            {
                list.append(CEToken(a, IDENTIFIER));
                a = "";
            }
            list.append(CEToken(t, i));
        }
    }
    if(a.length() > 0)
        list.append(CEToken(a, IDENTIFIER));
    return list;
}

QStringList CECodeParser::getIdentifierList(QString s)
{
    QStringList list;
    QString a, t;
    int i;
    textToScanner(s.toLocal8Bit().data());
    while(i = yylex())
    {
        t = QString(yytext);
        if(i == IDENTIFIER)
            a.append(t);
        else if(t == ".")
            a.append(".");
        else if(a.length() > 0)
        {
            list.append(a);
            a = "";
        }
    }
    if(a.length() > 0)
        list.append(a);
    return list;
}

int CECodeParser::setParseToTest(QString s)
{
    return textToParse(s.toLocal8Bit().data());
}

QString CECodeParser::getToken()
{
    return QString(yytext);
}

int CECodeParser::getResult()
{
    return yylval;
}
