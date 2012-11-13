#ifndef XMLFILE_H
#define XMLFILE_H

#include <QObject>
#include <QString>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QMap>
#include "./fegraphicsitem.h"
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
    void writeState(FEGraphicsItem *g);
    void writeTransition(FEGraphicsItem *g);
    void writeStartIF(FEGraphicsItem *g);
    void writeStopIF();
    void writeStartTrue();
    void writeStopTrue();
    void writeStartFalse();
    void writeStopFalse();
    void writeStartWHILE(FEGraphicsItem *g);
    void writeEndWHILE(FEGraphicsItem *g);
    void writeStartFOR(FEGraphicsItem *g);
    void writeEndFOR(FEGraphicsItem *g);
    void writeStartElement(QString name);
    void writeStopElement();
    void writeTextElement(QString name, QString v);


    void read(QList<FEGraphicsItem*> &itemsList);

    QString getFunctionName() const { return sFunctionName;}

private:
    QXmlStreamWriter *docToWrite;
    QDomDocument *docToRead;
    QFile *file;
    //QString _fileName;
    QMap<QString, QString> operators;

    void setOperators();
    void reverseOperators();

    bool openToWrite();
    bool openToRead();

    void read(QList<FEGraphicsItem*> &itemsList, QList<FEGraphicsItem*> &items, QDomNode node, int level);
    FEGraphicsItem* readIF(QList<FEGraphicsItem*> &itemsList, QList<FEGraphicsItem*> &items, QDomNode node, int level);
    FEGraphicsItem* readWhile(QList<FEGraphicsItem*> &itemsList, QList<FEGraphicsItem*> &items, QDomNode node, int level);
    FEGraphicsItem* readFor(QList<FEGraphicsItem*> &itemsList, QList<FEGraphicsItem*> &items, QDomNode node, int level);
    FEGraphicsItem* readState(QDomNode node, ItemType type);
    FEGraphicsItem* readTransition(QDomNode node);
    FEGraphicsItem* readCondition(QDomNode node, ItemType type);
    void readStatement(QDomNode node, QString &statement);
    QString readTextElement(QDomNode node, QString name);
    QString readTextElement(QDomNode node);
};


#endif // XMLFILE_H
