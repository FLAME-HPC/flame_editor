#ifndef CECFILE_H
#define CECFILE_H

#include <QString>
#include <QStack>
#include "QFile"
#include <QTextStream>
#include "./cefiletype.h"

namespace CECFileConstants{
    const QString NewLine("\r\n");
    const QString OpenBracket("{");
    const QString CloseBracket("}");
}

class CECFile : public CEFileType
{
public:
    CECFile(QString fileName, QString FunctionName);
    ~CECFile();
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

private:
    enum BlockType{
        None,
        NewBlock,
        Else
    };

    struct Block{
        BlockType type;
        int lineNumber;
        int assignmentNumbers;
        Block()
        {
            type = None;
            lineNumber = -1;
            assignmentNumbers = 0;
        }
        Block(BlockType t, int n)
        {
            type = t;
            lineNumber = n;
            assignmentNumbers = 0;
        }
        void add(int n)
        {
            assignmentNumbers += n;
        }
    };

    QFile *file;
    QTextStream *text;
    int openBrackets;
    int indentationNumber;
    QString indentationText;
    QStringList assignmentList;
    QStack<Block> blocks;

    void creatIndentationText(int n);
    void add(int n);
    void append(QString s);
    void writeVariable(CEVariableDeclared v);
};


#endif // CFILE_H
