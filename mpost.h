#ifndef MPOST_H
#define MPOST_H

#include <QMetaType>
#include <QPainter>
#include "variable.h"
#include "memorymodel.h"

struct statement
{
    QString lhs;
    QString op;
    QString rhs;
    double drhs;
    bool isVariable;
    bool isValid;
};

typedef statement statement;

class Mpost
{
public:
    enum EditMode { Editable, ReadOnly };

    Mpost(QString n = "", QString op = "", int v = 0, bool enabled = false);

    void paint(QPainter *painter, const QRect &rect,
                const QPalette &palette, EditMode mode) const;

    QString name() const { return myName; }
    QString name2() const { return myName2; }
    int value() const { return myValue; }
    void setName(QString n) { myName = n; }
    void setName2(QString n) { myName2 = n; }
    void setValue(int v) { myValue = v; }
    bool enabled() const { return myEnabled; }
    void setEnabled(bool enabled) { myEnabled = enabled; }
    QString op() const { return myOp; }
    void setOp(QString o) { myOp = o; }
    int value2() const { return myValue2; }
    void setValue2(int v) { myValue2 = v; }
    QString getText();
    void setText(QString t);
    void setMemory(MemoryModel * m) { memory = m; }
    QList<statement> getStatements() { return statements; }

private:
    bool myEnabled;
    QString myName;
    QString myName2;
    int myValue;
    QString myOp;
    int myValue2;
    QString text;
    QList<statement> statements;
    MemoryModel * memory;
};

Q_DECLARE_METATYPE(Mpost)

#endif // MPOST_H
