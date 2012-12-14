#include "xmlfile.h"
#include <QtGui>
#include "./codeparser.h"
#include "./arrow.h"

XMLFile::XMLFile(QString fileName) : FileType(fileName, "")
{
    docToWrite = 0;
    docToRead = 0;
}

XMLFile::XMLFile(QString fileName, QString FunctionName) : FileType(fileName, FunctionName)
{
    docToWrite = 0;
    docToRead = 0;
    variables = 0;
}

XMLFile::~XMLFile()
{
    if(docToWrite != 0)
    {
        docToWrite->writeEndElement();
        docToWrite->writeEndElement();
        docToWrite->writeEndDocument();

        delete docToWrite;
    }
    file->close();
    delete file;

    if(docToRead != 0)
        delete docToRead;
    if(variables != 0)
    {
        variables->clear();
        delete variables;
    }
}

bool XMLFile::containsOperator(QString o)
{
    return operators.contains(o);
}

QString XMLFile::getOperators(QString o)
{
    if(operators.contains(o))
        return operators[o];
    else
        return "";
}

bool XMLFile::open(OpenMode om)
{
    if(om == Write)
        return openToWrite();
    else if(om == Read)
        return openToRead();
    else return false;
}

void XMLFile::setOperators()
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

void XMLFile::reverseOperators()
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

bool XMLFile::openToWrite()
{
    setOperators();
    file = new QFile(_fileName);
    file->open(QIODevice::WriteOnly | QIODevice::Text);
    //qDebug()<<"iiiiiii";
    docToWrite = new QXmlStreamWriter();
    docToWrite->setDevice(file);
    docToWrite->setAutoFormatting(true);

    docToWrite->writeStartDocument();
    docToWrite->writeStartElement("xmodel");
    docToWrite->writeStartElement("code");

    docToWrite->writeTextElement(XMLConstants::SFUNCTIONNAME, sFunctionName);

    return true;

}

void XMLFile::writeDeclarations(QList<VariableDeclared> *list)
{
    if(list->count() > 0)
    {
        docToWrite->writeStartElement(XMLConstants::SDECLARATIONS);
        for(int i = 0;i < list->count();i++)
            writeVariable(list->at(i));
        docToWrite->writeEndElement();
    }
}

void XMLFile::writeVariable(VariableDeclared v)
{
    docToWrite->writeStartElement(XMLConstants::SVARIABLE);
    docToWrite->writeTextElement(XMLConstants::STYPE, v.getType());
    docToWrite->writeTextElement(XMLConstants::SNAME, v.getName());
    if(v.getExpression() != "")
    {
        CodeParser cc(this);
        docToWrite->writeStartElement(XMLConstants::STATEMENT);
        cc.execute(v.getExpression());
        docToWrite->writeEndElement();
    }
    docToWrite->writeEndElement();
}

void XMLFile::writeState(GraphicsItem *g)
{
    docToWrite->writeStartElement(XMLConstants::SSTATE);
    docToWrite->writeTextElement(XMLConstants::SNAME, g->getName());
    docToWrite->writeEndElement();
}

void XMLFile::writeTransition(GraphicsItem *g)
{
    docToWrite->writeStartElement(XMLConstants::STRANSITION);
    docToWrite->writeTextElement(XMLConstants::SNAME, g->getName());
    docToWrite->writeStartElement(XMLConstants::STATEMENTS);
    CodeParser cc(this);
    QStringList *list = g->getAssignmentList();
    for(int i = 0;i < list->length();i++)
    {
        docToWrite->writeStartElement(XMLConstants::STATEMENT);
        cc.execute(list->at(i));
        docToWrite->writeEndElement();
    }
    docToWrite->writeEndElement();
    docToWrite->writeEndElement();
}

void XMLFile::writeStartIF(GraphicsItem *g)
{
    docToWrite->writeStartElement(XMLConstants::SBLOCKIF);
    docToWrite->writeStartElement(XMLConstants::SCONDITION);
    docToWrite->writeTextElement(XMLConstants::SNAME, g->getName());
    docToWrite->writeStartElement(XMLConstants::SEXPRESSION);
    QStringList *list = g->getAssignmentList();
    qDebug()<<list->size();
    if(list->size() == 1 && list->at(0) != "")
    {
        CodeParser cc(this);
        cc.execute(list->at(0));
    }
    docToWrite->writeEndElement();
    docToWrite->writeEndElement();
}

void XMLFile::writeStopIF()
{
    docToWrite->writeEndElement();
}

void XMLFile::writeStartTrue()
{
    docToWrite->writeStartElement(XMLConstants::SBLOCKTRUE);
}

void XMLFile::writeStopTrue()
{
    docToWrite->writeEndElement();
}

void XMLFile::writeStartFalse()
{
    docToWrite->writeStartElement(XMLConstants::SBLOCKFALSE);
}

void XMLFile::writeStopFalse()
{
    docToWrite->writeEndElement();
}

void XMLFile::writeStartWHILE(GraphicsItem *g)
{
    docToWrite->writeStartElement(XMLConstants::SBLOCKWHILE);
    docToWrite->writeStartElement(XMLConstants::SCONDITION);
    docToWrite->writeTextElement(XMLConstants::SNAME, g->getName());
    docToWrite->writeStartElement(XMLConstants::SEXPRESSION);
    QStringList *list = g->getAssignmentList();
    qDebug()<<list->size();
    if(list->size() == 1 && list->at(0) != "")
    {
        CodeParser cc(this);
        cc.execute(list->at(0));
    }
    docToWrite->writeEndElement();
    docToWrite->writeEndElement();
    docToWrite->writeStartElement(XMLConstants::SBLOCK);
}

void XMLFile::writeEndWHILE(GraphicsItem *g)
{
    docToWrite->writeEndElement();
    writeState(g);
    docToWrite->writeEndElement();
}

void XMLFile::writeStartFOR(GraphicsItem *g)
{
    docToWrite->writeStartElement(XMLConstants::SBLOCKFOR);
    docToWrite->writeStartElement(XMLConstants::STATEMENTS);
    docToWrite->writeTextElement(XMLConstants::SNAME, g->getName());
    QStringList *list = g->getAssignmentList();
    qDebug()<<list->size();
    for(int i = 0;i<3;i++)
    {
        docToWrite->writeStartElement(XMLConstants::STATEMENT);
        if(i < list->size() && list->at(i) != "")
        {
            CodeParser cc(this);
            cc.execute(list->at(i));
        }
        docToWrite->writeEndElement();
    }
    docToWrite->writeEndElement();
    docToWrite->writeStartElement(XMLConstants::SBLOCK);
}

void XMLFile::writeEndFOR(GraphicsItem *g)
{
    docToWrite->writeEndElement();
    writeState(g);
    docToWrite->writeEndElement();
}

void XMLFile::writeStartElement(QString name)
{
    docToWrite->writeStartElement(name);
}

void XMLFile::writeStopElement()
{
    docToWrite->writeEndElement();
}

void XMLFile::writeTextElement(QString name, QString v)
{
    docToWrite->writeTextElement(name, v);
}

bool XMLFile::openToRead()
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

void XMLFile::read(QList<GraphicsItem *> &itemsList)
{
    QDomElement root = docToRead->documentElement();
    if(!root.isNull())
    {
        QDomNode r = root.firstChild();
        sFunctionName = readTextElement(r, XMLConstants::SFUNCTIONNAME);
        variables = new QList<VariableDeclared>();
        QDomNode dnDeclarations = r.namedItem(XMLConstants::SDECLARATIONS);
        if(!dnDeclarations.isNull())
            readDeclarations(dnDeclarations);
        qDebug()<<r.nodeName();
        QList<GraphicsItem*> items;
        read(itemsList, items, r, 0);
    }
}

void XMLFile::read(QList<GraphicsItem *> &itemsList, QList<GraphicsItem *> &items, QDomNode node, int level)
{
    QDomNode lastChild = node.lastChild();
    QDomNode n = node.firstChild();
    QString name;
    GraphicsItem *p = 0;
    GraphicsItem *c = 0;
    GraphicsItem *gif = 0;
    GraphicsItem *w = 0;
    if(items.count() > 0)
    {
        p = items.last();
        items.clear();
    }
    while(!n.isNull())
    {
        name = n.nodeName();
        qDebug()<<n.nodeName()<<(n == lastChild ?"ok":"no");
        if(name == XMLConstants::SSTATE)
            c = readState(n, n == lastChild?level == 0?FinalState:SolidState:State);
        else if(name == XMLConstants::STRANSITION)
            c = readTransition(n);
        else if(name == XMLConstants::SBLOCKIF)
        {
            gif = readIF(itemsList, items, n, level + 1);
            Arrow *a = new Arrow(p, gif);
            if(w != 0) a->setName("exit");
            p->addGraphicsItem(a);
            gif->addGraphicsItemParents(p);
            qDebug()<<items.count();
        }
        else if(name == XMLConstants::SBLOCKWHILE)
        {
            QList<GraphicsItem *> itemsBlock;
            itemsBlock.append(p);
            c = readWhile(itemsList, itemsBlock, n, level + 1);
            p = c;
            c = 0;
        }
        else if(name == XMLConstants::SBLOCKFOR)
        {
            QList<GraphicsItem *> itemsBlock;
            itemsBlock.append(p);
            c = readFor(itemsList, itemsBlock, n, level + 1);
            p = c;
            c = 0;
        }

        if(gif != 0 || w != 0)
        {
            if(gif != 0 && w != 0)
            {
                Arrow *a2 = new Arrow(p, w);
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
                        GraphicsItem *itm = items.first();
                        items.removeAt(0);
                        Arrow *a = new Arrow(itm, c);
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
                    Arrow *a1 = new Arrow(p, c);
                    a1->setName("exit");
                    p->addGraphicsItem(a1);
                    c->addGraphicsItemParents(p);

                    Arrow *a2 = new Arrow(p, w);
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
            Arrow *a = new Arrow(p, c);
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

GraphicsItem* XMLFile::readIF(QList<GraphicsItem *> &itemsList, QList<GraphicsItem *> &items, QDomNode node, int level)
{
    items.clear();
    QDomNode dnConditionIf = node.namedItem(XMLConstants::SCONDITION);
    GraphicsItem *gConditionIf = 0;
    if(!dnConditionIf.isNull())
    {
        gConditionIf = readCondition(dnConditionIf, ConditionIf);
        QDomNode dnExpression = dnConditionIf.namedItem(XMLConstants::SEXPRESSION);
        if(!dnExpression.isNull())
        {
            QString s = "";
            readStatement(dnExpression, s);
            QStringList *list = gConditionIf->getAssignmentList();
            list->append(s);
        }
    }
    itemsList.append(gConditionIf);
    GraphicsItem *stateFalse = new GraphicsItem(EmptyState);
    Arrow *af = new Arrow(gConditionIf, stateFalse);
    af->setName("false");
    gConditionIf->addGraphicsItem(af);
    stateFalse->addGraphicsItemParents(gConditionIf);
    itemsList.append(stateFalse);
    QDomNode dnFalse = node.namedItem(XMLConstants::SBLOCKFALSE);
    if(!dnFalse.isNull())
    {
        QList<GraphicsItem *> itemsFalse;
        itemsFalse.append(stateFalse);
        read(itemsList, itemsFalse, dnFalse, level + 1);
        itemsFalse.removeOne(stateFalse);
        while(itemsFalse.count() > 0)
        {
            items.append(itemsFalse.first());
            itemsFalse.removeAt(0);
        }
    }
    GraphicsItem *stateTrue = new GraphicsItem(EmptyState);
    Arrow *at = new Arrow(gConditionIf, stateTrue);
    at->setName("true");
    gConditionIf->addGraphicsItem(at);
    stateTrue->addGraphicsItemParents(gConditionIf);
    itemsList.append(stateTrue);
    QDomNode dnTrue = node.namedItem(XMLConstants::SBLOCKTRUE);
    if(!dnTrue.isNull())
    {
        QList<GraphicsItem *> itemsTrue;
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

GraphicsItem *XMLFile::readWhile(QList<GraphicsItem *> &itemsList, QList<GraphicsItem *> &items, QDomNode node, int level)
{
    QDomNode dnConditionWhile = node.namedItem(XMLConstants::SCONDITION);
    GraphicsItem *p = items.first();
    items.clear();
    GraphicsItem *gConditionWhile = 0;
    if(!dnConditionWhile.isNull())
    {
        gConditionWhile = readCondition(dnConditionWhile, ConditionWhile);
        Arrow *a = new Arrow(p, gConditionWhile);
        p->addGraphicsItem(a);
        gConditionWhile->addGraphicsItemParents(p);
        QDomNode dnExpression = dnConditionWhile.namedItem(XMLConstants::SEXPRESSION);
        if(!dnExpression.isNull())
        {
            QString s = "";
            readStatement(dnExpression, s);
            QStringList *list = gConditionWhile->getAssignmentList();
            list->append(s);
        }
    }
    itemsList.append(gConditionWhile);

    GraphicsItem *gOutsideEmptyState = new GraphicsItem(EmptyState);
    Arrow *aOutsideEmptyState = new Arrow(gConditionWhile, gOutsideEmptyState);
    aOutsideEmptyState->setName("exit");
    gConditionWhile->addGraphicsItem(aOutsideEmptyState);
    gOutsideEmptyState->addGraphicsItemParents(gConditionWhile);
    itemsList.append(gOutsideEmptyState);

    QDomNode dnBlock = node.namedItem(XMLConstants::SBLOCK);
    if(!dnBlock.isNull())
    {
        QList<GraphicsItem *> itemsBlock;
        GraphicsItem *gEmptyState = new GraphicsItem(EmptyState);
        Arrow *aEmptyState = new Arrow(gConditionWhile, gEmptyState);
        gConditionWhile->addGraphicsItem(aEmptyState);
        gEmptyState->addGraphicsItemParents(gConditionWhile);
        itemsList.append(gEmptyState);
        itemsBlock.append(gEmptyState);
        read(itemsList, itemsBlock, dnBlock, level + 1);
        GraphicsItem *gSolidState = new GraphicsItem(SolidState);
        while(itemsBlock.count() > 0)
        {
            GraphicsItem *g = itemsBlock.first();
            itemsBlock.removeAt(0);
            Arrow *a = new Arrow(g, gSolidState);
            g->addGraphicsItem(a);
            gSolidState->addGraphicsItemParents(g);
        }
        itemsList.append(gSolidState);
    }
    return gOutsideEmptyState;
}

GraphicsItem *XMLFile::readFor(QList<GraphicsItem *> &itemsList, QList<GraphicsItem *> &items, QDomNode node, int level)
{
    QDomNode dnConditionFor = node.namedItem(XMLConstants::STATEMENTS);
    GraphicsItem *p = items.first();
    items.clear();
    GraphicsItem *gConditionFor = 0;
    if(!dnConditionFor.isNull())
    {
        gConditionFor = readCondition(dnConditionFor, ConditionFor);
        Arrow *a = new Arrow(p, gConditionFor);
        p->addGraphicsItem(a);
        gConditionFor->addGraphicsItemParents(p);
        QDomNode dnExpression = dnConditionFor.firstChild();
        QStringList *list = gConditionFor->getAssignmentList();
        int i = 0;
        while(i<3 && !dnExpression.isNull())
        {
            if(dnExpression.nodeName() == XMLConstants::STATEMENT)
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

    GraphicsItem *gOutsideEmptyState = new GraphicsItem(EmptyState);
    Arrow *aOutsideEmptyState = new Arrow(gConditionFor, gOutsideEmptyState);
    aOutsideEmptyState->setName("exit");
    gConditionFor->addGraphicsItem(aOutsideEmptyState);
    gOutsideEmptyState->addGraphicsItemParents(gConditionFor);
    itemsList.append(gOutsideEmptyState);

    QDomNode dnBlock = node.namedItem(XMLConstants::SBLOCK);
    if(!dnBlock.isNull())
    {
        QList<GraphicsItem *> itemsBlock;
        GraphicsItem *gEmptyState = new GraphicsItem(EmptyState);
        Arrow *aEmptyState = new Arrow(gConditionFor, gEmptyState);
        gConditionFor->addGraphicsItem(aEmptyState);
        gEmptyState->addGraphicsItemParents(gConditionFor);
        itemsList.append(gEmptyState);
        itemsBlock.append(gEmptyState);
        read(itemsList, itemsBlock, dnBlock, level + 1);
        GraphicsItem *gSolidState = new GraphicsItem(SolidState);
        while(itemsBlock.count() > 0)
        {
            GraphicsItem *g = itemsBlock.first();
            itemsBlock.removeAt(0);
            Arrow *a = new Arrow(g, gSolidState);
            g->addGraphicsItem(a);
            gSolidState->addGraphicsItemParents(g);
        }
        itemsList.append(gSolidState);
    }
    return gOutsideEmptyState;
}

GraphicsItem* XMLFile::readState(QDomNode node, ItemType type)
{
    GraphicsItem *g = new GraphicsItem(type);
    g->setName(readTextElement(node, XMLConstants::SNAME));
    return g;
}

GraphicsItem *XMLFile::readTransition(QDomNode node)
{
    GraphicsItem *g = new GraphicsItem(Transition);
    g->setName(readTextElement(node, XMLConstants::SNAME));
    QDomNode dnSS = node.namedItem(XMLConstants::STATEMENTS);
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

GraphicsItem *XMLFile::readCondition(QDomNode node, ItemType type)
{
    GraphicsItem *g = new GraphicsItem(type);
    g->setName(readTextElement(node, XMLConstants::SNAME));
    return g;
}

void XMLFile::readStatement(QDomNode node, QString &statement)
{
    QDomNode n = node.firstChild();
    QString name;
    while(!n.isNull())
    {
        name = n.nodeName();
        if(name == XMLConstants::SIDENTIFIER || name == XMLConstants::SNUMBER)
            statement.append(readTextElement(n));
        else if(name == XMLConstants::SOP)
        {
            QString s = readTextElement(n);
            if(operators.contains(s))
                statement.append(operators[s]);
        }
        else if(name == XMLConstants::SFUNCTION)
        {
            statement.append(readTextElement(n, XMLConstants::SNAME));
            QDomNode dnFPS = n.namedItem(XMLConstants::SPARAMETERS);
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

QString XMLFile::readTextElement(QDomNode node, QString name)
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

QString XMLFile::readTextElement(QDomNode node)
{
    if(!node.isNull() && node.isElement())
    {
        QString s = node.toElement().text();
        qDebug()<<s;
        return s;
    }
    return "";
}

void XMLFile::readDeclarations(QDomNode node)
{
    QDomNode n = node.firstChild();
    QString name;
    while(!n.isNull())
    {
        name = n.nodeName();
        if(name == XMLConstants::SVARIABLE)
        {
            VariableDeclared v = readVariable(n);
            if(v.getType() != "" && v.getName() != "")
                variables->append(v);
        }
        n = n.nextSibling();
    }
}

VariableDeclared XMLFile::readVariable(QDomNode node)
{
    QString t = readTextElement(node, XMLConstants::STYPE);
    QString n = readTextElement(node, XMLConstants::SNAME);
    QString e = readTextElement(node, XMLConstants::STATEMENT);
    VariableDeclared v(t, n, e);
    return v;
}
