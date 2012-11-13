#ifndef FILETYPE_H
#define FILETYPE_H

#include <QString>
#include "./fegraphicsitem.h"

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
    virtual void writeState(FEGraphicsItem *g) = 0;
    virtual void writeTransition(FEGraphicsItem *g) = 0;
    virtual void writeStartIF(FEGraphicsItem *g) = 0;
    virtual void writeStopIF() = 0;
    virtual void writeStartTrue() = 0;
    virtual void writeStopTrue() = 0;
    virtual void writeStartFalse() = 0;
    virtual void writeStopFalse() = 0;
    virtual void writeStartWHILE(FEGraphicsItem *g) = 0;
    virtual void writeEndWHILE(FEGraphicsItem *g) = 0;
    virtual void writeStartFOR(FEGraphicsItem *g) = 0;
    virtual void writeEndFOR(FEGraphicsItem *g) = 0;

protected:
    QString _fileName;
    QString sFunctionName;
};

#endif // FILETYPE_H
