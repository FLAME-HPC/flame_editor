#ifndef CFILE_H
#define CFILE_H

#include <QString>
#include <QStack>
#include "QFile"
#include <QTextStream>
#include "./filetype.h"

namespace CFileConstants{
    const QString NewLine("\r\n");
    const QString OpenBracket("{");
    const QString CloseBracket("}");
}

class CFile : public FileType
{
public:
    CFile(QString fileName, QString FunctionName);
    ~CFile();
    bool open(OpenMode om);
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
};


#endif // CFILE_H
