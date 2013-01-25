#include "cexmlfile.h"
#include <QtGui>
#include "./cecodeparser.h"
#include "./cearrow.h"

CEXMLFile::CEXMLFile(QString fileName) : CEFileType(0, "")
{
    docToWrite = 0;
    docToRead = 0;
    file = 0;
    _fileName = fileName;
}

CEXMLFile::CEXMLFile(QByteArray *stream, QString FunctionName) : CEFileType(stream, FunctionName)
{
    docToWrite = 0;
    docToRead = 0;
    variables = 0;
    file = 0;
}

CEXMLFile::~CEXMLFile()
{
    if(docToWrite != 0)
    {
        docToWrite->writeEndElement();
        docToWrite->writeEndElement();
        docToWrite->writeEndDocument();

        delete docToWrite;
    }

    if(file != 0)
    {
        file->close();
        delete file;
    }

    if(docToRead != 0)
        delete docToRead;
    if(variables != 0)
    {
        variables->clear();
        delete variables;
    }
}

bool CEXMLFile::containsOperator(QString o)
{
    return operators.contains(o);
}

QString CEXMLFile::getOperators(QString o)
{
    if(operators.contains(o))
        return operators[o];
    else
        return "";
}

bool CEXMLFile::open(CEOpenMode om)
{
    if(om == Write)
        return openToWrite();
    else if(om == Read)
        return openToRead();
    else return false;
}

void CEXMLFile::setOperators()
{
    operators.insert("+", "ADD");
    operators.insert("-", "SUB");
    operators.insert("*", "MUL");
    operators.insert("/", "DIV");
    operators.insert("=", "ASSIGN");

    operators.insert(">", "GT");
    operators.insert(">=", "GEQ");
    operators.insert(">", "LT");
    operators.insert(">=", "LEQ");
    operators.insert("==", "EQ");
    operators.insert("!=", "NEQ");
}

void CEXMLFile::reverseOperators()
{
    QMap<QString, QString> op(operators);
    operators.clear();
    QList<QString> keys = op.keys();
    //qDebug()<<keys.count();
    for(int i = 0;i<keys.count();i++)
        operators.insert(op[keys[i]], keys[i]);
    op.clear();
    //keys = operators.keys();
    //for(int i = 0;i<keys.count();i++)
    //    qDebug()<<keys[i]<<operators[keys[i]];
}

bool CEXMLFile::openToWrite()
{
    setOperators();
    //file = new QFile(_fileName);
    //file->open(QIODevice::WriteOnly | QIODevice::Text);
    //qDebug()<<"iiiiiii";
    docToWrite = new QXmlStreamWriter(_stream);
    //docToWrite->setDevice(file);
    docToWrite->setAutoFormatting(true);

    docToWrite->writeStartDocument();
    docToWrite->writeStartElement("xmodel");
    docToWrite->writeStartElement("code");

    docToWrite->writeTextElement(CEXMLConstants::SFUNCTIONNAME, sFunctionName);

    return true;

}

void CEXMLFile::writeDeclarations(QList<CEVariableDeclared> *list)
{
    if(list->count() > 0)
    {
        docToWrite->writeStartElement(CEXMLConstants::SDECLARATIONS);
        for(int i = 0;i < list->count();i++)
            writeVariable(list->at(i));
        docToWrite->writeEndElement();
    }
}

void CEXMLFile::writeVariable(CEVariableDeclared v)
{
    docToWrite->writeStartElement(CEXMLConstants::SVARIABLE);
    docToWrite->writeTextElement(CEXMLConstants::STYPE, v.getType());
    docToWrite->writeTextElement(CEXMLConstants::SNAME, v.getName());
    if(v.getExpression() != "")
    {
        CECodeParser cc(this);
        docToWrite->writeStartElement(CEXMLConstants::STATEMENT);
        cc.execute(v.getExpression());
        docToWrite->writeEndElement();
    }
    docToWrite->writeEndElement();
}

void CEXMLFile::writeState(CEGraphicsItem *g)
{
    docToWrite->writeStartElement(CEXMLConstants::SSTATE);
    docToWrite->writeTextElement(CEXMLConstants::SNAME, g->getName());
    docToWrite->writeEndElement();
}

void CEXMLFile::writeTransition(CEGraphicsItem *g)
{
    docToWrite->writeStartElement(CEXMLConstants::STRANSITION);
    docToWrite->writeTextElement(CEXMLConstants::SNAME, g->getName());
    docToWrite->writeStartElement(CEXMLConstants::STATEMENTS);
    CECodeParser cc(this);
    QStringList *list = g->getAssignmentList();
    for(int i = 0;i < list->length();i++)
    {
        docToWrite->writeStartElement(CEXMLConstants::STATEMENT);
        cc.execute(list->at(i));
        docToWrite->writeEndElement();
    }
    docToWrite->writeEndElement();
    docToWrite->writeEndElement();
}

void CEXMLFile::writeStartIF(CEGraphicsItem *g)
{
    docToWrite->writeStartElement(CEXMLConstants::SBLOCKIF);
    docToWrite->writeStartElement(CEXMLConstants::SCONDITION);
    docToWrite->writeTextElement(CEXMLConstants::SNAME, g->getName());
    docToWrite->writeStartElement(CEXMLConstants::SEXPRESSION);
    QStringList *list = g->getAssignmentList();
    qDebug()<<list->size();
    if(list->size() == 1 && list->at(0) != "")
    {
        CECodeParser cc(this);
        cc.execute(list->at(0));
    }
    docToWrite->writeEndElement();
    docToWrite->writeEndElement();
}

void CEXMLFile::writeStopIF()
{
    docToWrite->writeEndElement();
}

void CEXMLFile::writeStartTrue()
{
    docToWrite->writeStartElement(CEXMLConstants::SBLOCKTRUE);
}

void CEXMLFile::writeStopTrue()
{
    docToWrite->writeEndElement();
}

void CEXMLFile::writeStartFalse()
{
    docToWrite->writeStartElement(CEXMLConstants::SBLOCKFALSE);
}

void CEXMLFile::writeStopFalse()
{
    docToWrite->writeEndElement();
}

void CEXMLFile::writeStartWHILE(CEGraphicsItem *g)
{
    docToWrite->writeStartElement(CEXMLConstants::SBLOCKWHILE);
    docToWrite->writeStartElement(CEXMLConstants::SCONDITION);
    docToWrite->writeTextElement(CEXMLConstants::SNAME, g->getName());
    docToWrite->writeStartElement(CEXMLConstants::SEXPRESSION);
    QStringList *list = g->getAssignmentList();
    qDebug()<<list->size();
    if(list->size() == 1 && list->at(0) != "")
    {
        CECodeParser cc(this);
        cc.execute(list->at(0));
    }
    docToWrite->writeEndElement();
    docToWrite->writeEndElement();
    docToWrite->writeStartElement(CEXMLConstants::SBLOCK);
}

void CEXMLFile::writeEndWHILE(CEGraphicsItem *g)
{
    docToWrite->writeEndElement();
    writeState(g);
    docToWrite->writeEndElement();
}

void CEXMLFile::writeStartFOR(CEGraphicsItem *g)
{
    docToWrite->writeStartElement(CEXMLConstants::SBLOCKFOR);
    docToWrite->writeStartElement(CEXMLConstants::STATEMENTS);
    docToWrite->writeTextElement(CEXMLConstants::SNAME, g->getName());
    QStringList *list = g->getAssignmentList();
    qDebug()<<list->size();
    for(int i = 0;i<3;i++)
    {
        docToWrite->writeStartElement(CEXMLConstants::STATEMENT);
        if(i < list->size() && list->at(i) != "")
        {
            CECodeParser cc(this);
            cc.execute(list->at(i));
        }
        docToWrite->writeEndElement();
    }
    docToWrite->writeEndElement();
    docToWrite->writeStartElement(CEXMLConstants::SBLOCK);
}

void CEXMLFile::writeEndFOR(CEGraphicsItem *g)
{
    docToWrite->writeEndElement();
    writeState(g);
    docToWrite->writeEndElement();
}

void CEXMLFile::writeStartElement(QString name)
{
    docToWrite->writeStartElement(name);
}

void CEXMLFile::writeStopElement()
{
    docToWrite->writeEndElement();
}

void CEXMLFile::writeTextElement(QString name, QString v)
{
    docToWrite->writeTextElement(name, v);
}

bool CEXMLFile::openToRead()
{
    setOperators();
    reverseOperators();
    file = new QFile(_fileName);
    if(!file->open(QIODevice::ReadOnly))
        return false;
    docToRead = new QDomDocument();
    if(!docToRead->setContent(file))
    {
        file->close();
        return false;
    }
    file->close();
    return true;
}

void CEXMLFile::read(QList<CEGraphicsItem *> &itemsList)
{
    QDomElement root = docToRead->documentElement();
    if(!root.isNull())
    {
        QDomNode r = root.firstChild();
        sFunctionName = readTextElement(r, CEXMLConstants::SFUNCTIONNAME);
        variables = new QList<CEVariableDeclared>();
        QDomNode dnDeclarations = r.namedItem(CEXMLConstants::SDECLARATIONS);
        if(!dnDeclarations.isNull())
            readDeclarations(dnDeclarations);
        qDebug()<<r.nodeName();
        QList<CEGraphicsItem*> items;
        read(itemsList, items, r, 0);
    }
}

void CEXMLFile::read(QList<CEGraphicsItem *> &itemsList, QList<CEGraphicsItem *> &items, QDomNode node, int level)
{
    QDomNode lastChild = node.lastChild();
    QDomNode n = node.firstChild();
    QString name;
    CEGraphicsItem *p = 0;
    CEGraphicsItem *c = 0;
    CEGraphicsItem *gif = 0;
    CEGraphicsItem *w = 0;
    if(items.count() > 0)
    {
        p = items.last();
        items.clear();
    }
    while(!n.isNull())
    {
        name = n.nodeName();
        qDebug()<<n.nodeName()<<(n == lastChild ?"ok":"no");
        if(name == CEXMLConstants::SSTATE)
            c = readState(n, n == lastChild?level == 0?FinalState:SolidState:State);
        else if(name == CEXMLConstants::STRANSITION)
            c = readTransition(n);
        else if(name == CEXMLConstants::SBLOCKIF)
        {
            gif = readIF(itemsList, items, n, level + 1);
            CEArrow *a = new CEArrow(p, gif);
            if(w != 0) a->setName("exit");
            p->addGraphicsItem(a);
            gif->addGraphicsItemParents(p);
            qDebug()<<items.count();
        }
        else if(name == CEXMLConstants::SBLOCKWHILE)
        {
            QList<CEGraphicsItem *> itemsBlock;
            itemsBlock.append(p);
            c = readWhile(itemsList, itemsBlock, n, level + 1);
            p = c;
            c = 0;
        }
        else if(name == CEXMLConstants::SBLOCKFOR)
        {
            QList<CEGraphicsItem *> itemsBlock;
            itemsBlock.append(p);
            c = readFor(itemsList, itemsBlock, n, level + 1);
            p = c;
            c = 0;
        }

        if(gif != 0 || w != 0)
        {
            if(gif != 0 && w != 0)
            {
                CEArrow *a2 = new CEArrow(p, w);
                p->addGraphicsItem(a2);
                w->addGraphicsItemParents(p);
                w = 0;
            }
            if(c != 0)
            {
                if(gif != 0)
                {
                    while(items.count() > 0)
                    {
                        CEGraphicsItem *itm = items.first();
                        items.removeAt(0);
                        CEArrow *a = new CEArrow(itm, c);
                        itm->addGraphicsItem(a);
                        c->addGraphicsItemParents(itm);
                    }
                    p = c;
                    c = 0;
                    gif = 0;
                    itemsList.append(p);
                }
                else if(w != 0)
                {
                    CEArrow *a1 = new CEArrow(p, c);
                    a1->setName("exit");
                    p->addGraphicsItem(a1);
                    c->addGraphicsItemParents(p);

                    CEArrow *a2 = new CEArrow(p, w);
                    p->addGraphicsItem(a2);
                    w->addGraphicsItemParents(p);
                    p = c;
                    c = 0;
                    w = 0;
                    itemsList.append(p);
                }
            }
        }
        else if(p == 0 && c != 0)
        {
            p = c;
            c = 0;
            itemsList.append(p);
        }
        else if(p != 0 && c != 0)
        {
            CEArrow *a = new CEArrow(p, c);
            p->addGraphicsItem(a);
            c->addGraphicsItemParents(p);
            p = c;
            c = 0;
            itemsList.append(p);
        }
        n = n.nextSibling();
    }
    if(p != 0 && items.count() == 0)
        items.append(p);
}

CEGraphicsItem* CEXMLFile::readIF(QList<CEGraphicsItem *> &itemsList, QList<CEGraphicsItem *> &items, QDomNode node, int level)
{
    items.clear();
    QDomNode dnConditionIf = node.namedItem(CEXMLConstants::SCONDITION);
    CEGraphicsItem *gConditionIf = 0;
    if(!dnConditionIf.isNull())
    {
        gConditionIf = readCondition(dnConditionIf, ConditionIf);
        QDomNode dnExpression = dnConditionIf.namedItem(CEXMLConstants::SEXPRESSION);
        if(!dnExpression.isNull())
        {
            QString s = "";
            readStatement(dnExpression, s);
            QStringList *list = gConditionIf->getAssignmentList();
            list->append(s);
        }
    }
    itemsList.append(gConditionIf);
    CEGraphicsItem *stateFalse = new CEGraphicsItem(EmptyState);
    CEArrow *af = new CEArrow(gConditionIf, stateFalse);
    af->setName("false");
    gConditionIf->addGraphicsItem(af);
    stateFalse->addGraphicsItemParents(gConditionIf);
    itemsList.append(stateFalse);
    QDomNode dnFalse = node.namedItem(CEXMLConstants::SBLOCKFALSE);
    if(!dnFalse.isNull())
    {
        QList<CEGraphicsItem *> itemsFalse;
        itemsFalse.append(stateFalse);
        read(itemsList, itemsFalse, dnFalse, level + 1);
        itemsFalse.removeOne(stateFalse);
        while(itemsFalse.count() > 0)
        {
            items.append(itemsFalse.first());
            itemsFalse.removeAt(0);
        }
    }
    CEGraphicsItem *stateTrue = new CEGraphicsItem(EmptyState);
    CEArrow *at = new CEArrow(gConditionIf, stateTrue);
    at->setName("true");
    gConditionIf->addGraphicsItem(at);
    stateTrue->addGraphicsItemParents(gConditionIf);
    itemsList.append(stateTrue);
    QDomNode dnTrue = node.namedItem(CEXMLConstants::SBLOCKTRUE);
    if(!dnTrue.isNull())
    {
        QList<CEGraphicsItem *> itemsTrue;
        itemsTrue.append(stateTrue);
        read(itemsList, itemsTrue, dnTrue, level + 1);
        itemsTrue.removeOne(stateTrue);
        while(itemsTrue.count() > 0)
        {
            items.append(itemsTrue.first());
            itemsTrue.removeAt(0);
        }
    }
    return gConditionIf;
}

CEGraphicsItem *CEXMLFile::readWhile(QList<CEGraphicsItem *> &itemsList, QList<CEGraphicsItem *> &items, QDomNode node, int level)
{
    QDomNode dnConditionWhile = node.namedItem(CEXMLConstants::SCONDITION);
    CEGraphicsItem *p = items.first();
    items.clear();
    CEGraphicsItem *gConditionWhile = 0;
    if(!dnConditionWhile.isNull())
    {
        gConditionWhile = readCondition(dnConditionWhile, ConditionWhile);
        CEArrow *a = new CEArrow(p, gConditionWhile);
        p->addGraphicsItem(a);
        gConditionWhile->addGraphicsItemParents(p);
        QDomNode dnExpression = dnConditionWhile.namedItem(CEXMLConstants::SEXPRESSION);
        if(!dnExpression.isNull())
        {
            QString s = "";
            readStatement(dnExpression, s);
            QStringList *list = gConditionWhile->getAssignmentList();
            list->append(s);
        }
    }
    itemsList.append(gConditionWhile);

    CEGraphicsItem *gOutsideEmptyState = new CEGraphicsItem(EmptyState);
    CEArrow *aOutsideEmptyState = new CEArrow(gConditionWhile, gOutsideEmptyState);
    aOutsideEmptyState->setName("exit");
    gConditionWhile->addGraphicsItem(aOutsideEmptyState);
    gOutsideEmptyState->addGraphicsItemParents(gConditionWhile);
    itemsList.append(gOutsideEmptyState);

    QDomNode dnBlock = node.namedItem(CEXMLConstants::SBLOCK);
    if(!dnBlock.isNull())
    {
        QList<CEGraphicsItem *> itemsBlock;
        CEGraphicsItem *gEmptyState = new CEGraphicsItem(EmptyState);
        CEArrow *aEmptyState = new CEArrow(gConditionWhile, gEmptyState);
        gConditionWhile->addGraphicsItem(aEmptyState);
        gEmptyState->addGraphicsItemParents(gConditionWhile);
        itemsList.append(gEmptyState);
        itemsBlock.append(gEmptyState);
        read(itemsList, itemsBlock, dnBlock, level + 1);
        CEGraphicsItem *gSolidState = new CEGraphicsItem(SolidState);
        while(itemsBlock.count() > 0)
        {
            CEGraphicsItem *g = itemsBlock.first();
            itemsBlock.removeAt(0);
            CEArrow *a = new CEArrow(g, gSolidState);
            g->addGraphicsItem(a);
            gSolidState->addGraphicsItemParents(g);
        }
        itemsList.append(gSolidState);
    }
    return gOutsideEmptyState;
}

CEGraphicsItem *CEXMLFile::readFor(QList<CEGraphicsItem *> &itemsList, QList<CEGraphicsItem *> &items, QDomNode node, int level)
{
    QDomNode dnConditionFor = node.namedItem(CEXMLConstants::STATEMENTS);
    CEGraphicsItem *p = items.first();
    items.clear();
    CEGraphicsItem *gConditionFor = 0;
    if(!dnConditionFor.isNull())
    {
        gConditionFor = readCondition(dnConditionFor, ConditionFor);
        CEArrow *a = new CEArrow(p, gConditionFor);
        p->addGraphicsItem(a);
        gConditionFor->addGraphicsItemParents(p);
        QDomNode dnExpression = dnConditionFor.firstChild();
        QStringList *list = gConditionFor->getAssignmentList();
        int i = 0;
        while(i<3 && !dnExpression.isNull())
        {
            if(dnExpression.nodeName() == CEXMLConstants::STATEMENT)
            {
                QString s = "";
                readStatement(dnExpression, s);
                list->append(s);
            }
            dnExpression = dnExpression.nextSibling();
        }
        for(;i<3;i++)
            list->append("");
    }
    itemsList.append(gConditionFor);

    CEGraphicsItem *gOutsideEmptyState = new CEGraphicsItem(EmptyState);
    CEArrow *aOutsideEmptyState = new CEArrow(gConditionFor, gOutsideEmptyState);
    aOutsideEmptyState->setName("exit");
    gConditionFor->addGraphicsItem(aOutsideEmptyState);
    gOutsideEmptyState->addGraphicsItemParents(gConditionFor);
    itemsList.append(gOutsideEmptyState);

    QDomNode dnBlock = node.namedItem(CEXMLConstants::SBLOCK);
    if(!dnBlock.isNull())
    {
        QList<CEGraphicsItem *> itemsBlock;
        CEGraphicsItem *gEmptyState = new CEGraphicsItem(EmptyState);
        CEArrow *aEmptyState = new CEArrow(gConditionFor, gEmptyState);
        gConditionFor->addGraphicsItem(aEmptyState);
        gEmptyState->addGraphicsItemParents(gConditionFor);
        itemsList.append(gEmptyState);
        itemsBlock.append(gEmptyState);
        read(itemsList, itemsBlock, dnBlock, level + 1);
        CEGraphicsItem *gSolidState = new CEGraphicsItem(SolidState);
        while(itemsBlock.count() > 0)
        {
            CEGraphicsItem *g = itemsBlock.first();
            itemsBlock.removeAt(0);
            CEArrow *a = new CEArrow(g, gSolidState);
            g->addGraphicsItem(a);
            gSolidState->addGraphicsItemParents(g);
        }
        itemsList.append(gSolidState);
    }
    return gOutsideEmptyState;
}

CEGraphicsItem* CEXMLFile::readState(QDomNode node, CEItemType type)
{
    CEGraphicsItem *g = new CEGraphicsItem(type);
    g->setName(readTextElement(node, CEXMLConstants::SNAME));
    return g;
}

CEGraphicsItem *CEXMLFile::readTransition(QDomNode node)
{
    CEGraphicsItem *g = new CEGraphicsItem(Transition);
    g->setName(readTextElement(node, CEXMLConstants::SNAME));
    QDomNode dnSS = node.namedItem(CEXMLConstants::STATEMENTS);
    if(!dnSS.isNull())
    {
        QStringList *list = g->getAssignmentList();
        QDomNode dnS = dnSS.firstChild();
        while(!dnS.isNull())
        {
            QString s = "";
            readStatement(dnS, s);
            list->append(s);
            dnS = dnS.nextSibling();
        }
    }
    return g;
}

CEGraphicsItem *CEXMLFile::readCondition(QDomNode node, CEItemType type)
{
    CEGraphicsItem *g = new CEGraphicsItem(type);
    g->setName(readTextElement(node, CEXMLConstants::SNAME));
    return g;
}

void CEXMLFile::readStatement(QDomNode node, QString &statement)
{
    QDomNode n = node.firstChild();
    QString name;
    while(!n.isNull())
    {
        name = n.nodeName();
        if(name == CEXMLConstants::SIDENTIFIER || name == CEXMLConstants::SNUMBER)
            statement.append(readTextElement(n));
        else if(name == CEXMLConstants::SOP)
        {
            QString s = readTextElement(n);
            if(operators.contains(s))
                statement.append(operators[s]);
        }
        else if(name == CEXMLConstants::SFUNCTION)
        {
            statement.append(readTextElement(n, CEXMLConstants::SNAME));
            QDomNode dnFPS = n.namedItem(CEXMLConstants::SPARAMETERS);
            statement.append("(");
            if(!dnFPS.isNull())
            {
                int i = 0;
                QDomNode dnFP = dnFPS.firstChild();
                while(!dnFP.isNull())
                {
                    if(i > 0) statement.append(",");
                    QString s = "";
                    readStatement(dnFP, s);
                    statement.append(s);
                    i++;
                    dnFP = dnFP.nextSibling();
                }
            }
            statement.append(")");
        }
        n = n.nextSibling();
    }
}

QString CEXMLFile::readTextElement(QDomNode node, QString name)
{
    QDomNode n = node.namedItem(name);
    if(!n.isNull() && n.isElement())
    {
        QString s = n.toElement().text();
        qDebug()<<s;
        return s;
    }
    return "";
}

QString CEXMLFile::readTextElement(QDomNode node)
{
    if(!node.isNull() && node.isElement())
    {
        QString s = node.toElement().text();
        qDebug()<<s;
        return s;
    }
    return "";
}

void CEXMLFile::readDeclarations(QDomNode node)
{
    QDomNode n = node.firstChild();
    QString name;
    while(!n.isNull())
    {
        name = n.nodeName();
        if(name == CEXMLConstants::SVARIABLE)
        {
            CEVariableDeclared v = readVariable(n);
            if(v.getType() != "" && v.getName() != "")
                variables->append(v);
        }
        n = n.nextSibling();
    }
}

CEVariableDeclared CEXMLFile::readVariable(QDomNode node)
{
    QString t = readTextElement(node, CEXMLConstants::STYPE);
    QString n = readTextElement(node, CEXMLConstants::SNAME);
    QString e = readTextElement(node, CEXMLConstants::STATEMENT);
    CEVariableDeclared v(t, n, e);
    return v;
}
