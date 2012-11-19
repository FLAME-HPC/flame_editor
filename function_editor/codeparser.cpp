#include "codeparser.h"
#include "parser/y.tab.h"

#include <QDebug>

extern "C" {

  char *yytext;
  int textToParse(char *p);
  void textToScanner(char *p);
  int yylex();

}

CodeParser::CodeParser(XMLFile *x):xml(x)
{
}

CodeParser::~CodeParser()
{
}

bool CodeParser::execute(QString statement)
{
    int ii = textToParse(statement.toLocal8Bit().data());
    bool b = ii == 0;
    if(b)
    {
        QList<Token> tokensList = getTokensList(statement);
        QStack<TokenTest> stack;
        while(tokensList.length() > 0)
        {
            Token currentToken = tokensList[0];
            tokensList.removeAt(0);
            if(currentToken.val == IDENTIFIER)
            {
                if(tokensList.length() > 0 && tokensList[0].token == "(")
                {
                    tokensList.removeAt(0);
                    xml->writeStartElement(XMLConstants::SFUNCTION);
                    xml->writeTextElement(XMLConstants::SNAME, currentToken.token);
                    xml->writeStartElement(XMLConstants::SPARAMETERS);
                    if(tokensList.length() > 0 && tokensList[0].token == ")")
                        stack.push(TokenTest(")", 1, 2));
                    else
                    {
                        xml->writeStartElement(XMLConstants::SPARAMETER);
                        stack.push(TokenTest(")", 1, 3));
                    }
                }
                else
                    xml->writeTextElement(XMLConstants::SIDENTIFIER, currentToken.token);
            }
            else if(currentToken.token == "(")
            {
                xml->writeStartElement(XMLConstants::SPARENTHESIS);
                stack.push(TokenTest(")", 0, 1));
            }
            else if(currentToken.token == ")" && stack.isEmpty() == false && stack.top().token == ")")
            {
                TokenTest tt = stack.pop();
                for(int i=0;i<tt.nr;i++)
                    xml->writeStopElement();
            }
            else if(currentToken.token == ",")
            {
                xml->writeStopElement();
                xml->writeStartElement(XMLConstants::SPARAMETER);
            }
            else if(xml->containsOperator(currentToken.token))
            {
                xml->writeTextElement(XMLConstants::SOP, xml->getOperators(currentToken.token));
            }
            else if(currentToken.val == INTEGER || currentToken.val == FLOATING)
            {
                xml->writeTextElement(XMLConstants::SNUMBER, currentToken.token);
            }
            qDebug()<<currentToken.token;
        }
    }
    return b;
}

QList<Token> CodeParser::getTokensList(QString s)
{
    QList<Token> list;
    QString a, t;
    int i;
    textToScanner(s.toLocal8Bit().data());
    while(i == yylex())
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
                list.append(Token(a, IDENTIFIER));
                a = "";
            }
            list.append(Token(t, i));
        }
    }
    if(a.length() > 0)
        list.append(Token(a, IDENTIFIER));
    return list;
}

QStringList CodeParser::getIdentifierList(QString s)
{
    QStringList list;
    QString a, t;
    int i;
    textToScanner(s.toLocal8Bit().data());
    while(i == yylex())
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

int CodeParser::setParseToTest(QString s)
{
    return textToParse(s.toLocal8Bit().data());
}

QString CodeParser::getToken()
{
    return QString(yytext);
}
