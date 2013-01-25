#ifndef CEFILETYPE_H
#define CEFILETYPE_H

#include <QString>
#include <QByteArray>
#include "./cegraphicsitem.h"
#include "./cevariabledeclared.h"

enum CEOpenMode{
    Read,
    Write
};

class CEFileType
{
public:
    CEFileType(QByteArray *stream, QString FunctionName);
    virtual ~CEFileType();

    virtual bool open(CEOpenMode om) = 0;
    virtual void writeDeclarations(QList<CEVariableDeclared> *list) = 0;
    virtual void writeState(CEGraphicsItem *g) = 0;
    virtual void writeTransition(CEGraphicsItem *g) = 0;
    virtual void writeStartIF(CEGraphicsItem *g) = 0;
    virtual void writeStopIF() = 0;
    virtual void writeStartTrue() = 0;
    virtual void writeStopTrue() = 0;
    virtual void writeStartFalse() = 0;
    virtual void writeStopFalse() = 0;
    virtual void writeStartWHILE(CEGraphicsItem *g) = 0;
    virtual void writeEndWHILE(CEGraphicsItem *g) = 0;
    virtual void writeStartFOR(CEGraphicsItem *g) = 0;
    virtual void writeEndFOR(CEGraphicsItem *g) = 0;

protected:
    QByteArray *_stream;
    QString sFunctionName;
};

#endif // FILETYPE_H
