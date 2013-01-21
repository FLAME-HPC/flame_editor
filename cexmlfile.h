#ifndef CEXMLFILE_H
#define CEXMLFILE_H

#include <QObject>
#include <QString>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QMap>
#include "./cegraphicsitem.h"
#include "./cefiletype.h"

namespace CEXMLConstants{
    const QString SXMODEL("xmodel");
    const QString SCODE("code");
    const QString SBLOCKIF("if");
    const QString SBLOCKWHILE("while");
    const QString SBLOCKFOR("for");
    const QString SBLOCKFALSE("false");
    const QString SBLOCKTRUE("true");
    const QString SBLOCK("block");
    const QString SCONDITION("condition");
    const QString SEXPRESSION("expression");
    const QString SSTATE("state");
    const QString SNAME("name");
    const QString STRANSITION("transition");
    const QString STATEMENT("statement");
    const QString STATEMENTS("statements");
    const QString SOP("op");
    const QString SFUNCTION("function");
    const QString SPARAMETERS("parameters");
    const QString SPARAMETER("parameter");
    const QString SIDENTIFIER("identifier");
    const QString SPARENTHESIS("parenthesis");
    const QString SNUMBER("number");
    const QString SFUNCTIONNAME("functionName");
    const QString SDECLARATIONS("declarations");
    const QString SVARIABLE("variable");
    const QString STYPE("type");
}

class CEXMLFile : public CEFileType
{
//    Q_OBJECT
public:
    CEXMLFile(QString fileName);
    CEXMLFile(QString fileName, QString FunctionName);
    ~CEXMLFile();
    bool containsOperator(QString o);
    QString getOperators(QString o);
    bool open(CEOpenMode om);
    void writeDeclarations(QList<CEVariableDeclared> *list);
    void writeState(CEGraphicsItem *g);
    void writeTransition(CEGraphicsItem *g);
    void writeStartIF(CEGraphicsItem *g);
    void writeStopIF();
    void writeStartTrue();
    void writeStopTrue();
    void writeStartFalse();
    void writeStopFalse();
    void writeStartWHILE(CEGraphicsItem *g);
    void writeEndWHILE(CEGraphicsItem *g);
    void writeStartFOR(CEGraphicsItem *g);
    void writeEndFOR(CEGraphicsItem *g);
    void writeStartElement(QString name);
    void writeStopElement();
    void writeTextElement(QString name, QString v);


    void read(QList<CEGraphicsItem*> &itemsList);

    QString getFunctionName() const { return sFunctionName;}
    QList<CEVariableDeclared>* getVariables() { return variables;}

private:
    QXmlStreamWriter *docToWrite;
    QDomDocument *docToRead;
    QFile *file;
    //QString _fileName;
    QMap<QString, QString> operators;

    QList<CEVariableDeclared> *variables;

    void setOperators();
    void reverseOperators();

    bool openToWrite();
    bool openToRead();

    void writeVariable(CEVariableDeclared v);

    void read(QList<CEGraphicsItem*> &itemsList, QList<CEGraphicsItem*> &items, QDomNode node, int level);
    CEGraphicsItem* readIF(QList<CEGraphicsItem*> &itemsList, QList<CEGraphicsItem*> &items, QDomNode node, int level);
    CEGraphicsItem* readWhile(QList<CEGraphicsItem*> &itemsList, QList<CEGraphicsItem*> &items, QDomNode node, int level);
    CEGraphicsItem* readFor(QList<CEGraphicsItem*> &itemsList, QList<CEGraphicsItem*> &items, QDomNode node, int level);
    CEGraphicsItem* readState(QDomNode node, CEItemType type);
    CEGraphicsItem* readTransition(QDomNode node);
    CEGraphicsItem* readCondition(QDomNode node, CEItemType type);
    void readStatement(QDomNode node, QString &statement);
    QString readTextElement(QDomNode node, QString name);
    QString readTextElement(QDomNode node);
    void readDeclarations(QDomNode node);
    CEVariableDeclared readVariable(QDomNode node);
};


#endif // XMLFILE_H
