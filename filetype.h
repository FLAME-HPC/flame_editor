#ifndef FILETYPE_H
#define FILETYPE_H

#include <QString>
#include "./graphicsitem.h"
#include "./variabledeclared.h"

enum OpenMode{
    Read,
    Write
};

class FileType
{
public:
    FileType(QString fileName, QString FunctionName);
    virtual ~FileType();

    virtual bool open(OpenMode om) = 0;
    virtual void writeDeclarations(QList<VariableDeclared> *list) = 0;
    virtual void writeState(GraphicsItem *g) = 0;
    virtual void writeTransition(GraphicsItem *g) = 0;
    virtual void writeStartIF(GraphicsItem *g) = 0;
    virtual void writeStopIF() = 0;
    virtual void writeStartTrue() = 0;
    virtual void writeStopTrue() = 0;
    virtual void writeStartFalse() = 0;
    virtual void writeStopFalse() = 0;
    virtual void writeStartWHILE(GraphicsItem *g) = 0;
    virtual void writeEndWHILE(GraphicsItem *g) = 0;
    virtual void writeStartFOR(GraphicsItem *g) = 0;
    virtual void writeEndFOR(GraphicsItem *g) = 0;

protected:
    QString _fileName;
    QString sFunctionName;
};

#endif // FILETYPE_H
