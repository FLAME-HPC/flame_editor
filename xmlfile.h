#ifndef XMLFILE_H
#define XMLFILE_H

#include <QObject>
#include <QString>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QMap>
#include "./graphicsitem.h"
#include "./filetype.h"

namespace XMLConstants{
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

class XMLFile : public FileType
{
//    Q_OBJECT
public:
    XMLFile(QString fileName);
    XMLFile(QString fileName, QString FunctionName);
    ~XMLFile();
    bool containsOperator(QString o);
    QString getOperators(QString o);
    bool open(OpenMode om);
    void writeDeclarations(QList<VariableDeclared> *list);
    void writeState(GraphicsItem *g);
    void writeTransition(GraphicsItem *g);
    void writeStartIF(GraphicsItem *g);
    void writeStopIF();
    void writeStartTrue();
    void writeStopTrue();
    void writeStartFalse();
    void writeStopFalse();
    void writeStartWHILE(GraphicsItem *g);
    void writeEndWHILE(GraphicsItem *g);
    void writeStartFOR(GraphicsItem *g);
    void writeEndFOR(GraphicsItem *g);
    void writeStartElement(QString name);
    void writeStopElement();
    void writeTextElement(QString name, QString v);


    void read(QList<GraphicsItem*> &itemsList);

    QString getFunctionName() const { return sFunctionName;}
    QList<VariableDeclared>* getVariables() { return variables;}

private:
    QXmlStreamWriter *docToWrite;
    QDomDocument *docToRead;
    QFile *file;
    //QString _fileName;
    QMap<QString, QString> operators;

    QList<VariableDeclared> *variables;

    void setOperators();
    void reverseOperators();

    bool openToWrite();
    bool openToRead();

    void writeVariable(VariableDeclared v);

    void read(QList<GraphicsItem*> &itemsList, QList<GraphicsItem*> &items, QDomNode node, int level);
    GraphicsItem* readIF(QList<GraphicsItem*> &itemsList, QList<GraphicsItem*> &items, QDomNode node, int level);
    GraphicsItem* readWhile(QList<GraphicsItem*> &itemsList, QList<GraphicsItem*> &items, QDomNode node, int level);
    GraphicsItem* readFor(QList<GraphicsItem*> &itemsList, QList<GraphicsItem*> &items, QDomNode node, int level);
    GraphicsItem* readState(QDomNode node, ItemType type);
    GraphicsItem* readTransition(QDomNode node);
    GraphicsItem* readCondition(QDomNode node, ItemType type);
    void readStatement(QDomNode node, QString &statement);
    QString readTextElement(QDomNode node, QString name);
    QString readTextElement(QDomNode node);
    void readDeclarations(QDomNode node);
    VariableDeclared readVariable(QDomNode node);
};


#endif // XMLFILE_H
