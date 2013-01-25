#include "cecfile.h"

CECFile::CECFile(QByteArray *stream, QString FunctionName) : CEFileType(stream, FunctionName)
{
    text = 0;
    openBrackets = 0;
    indentationNumber = 0;
    indentationText = "";
}

CECFile::~CECFile()
{
    if(text != 0)
    {
        while(assignmentList.count() > 0)
        {
            append(assignmentList.at(0));
            assignmentList.removeAt(0);
        }
//        while(!blocks.isEmpty())
//        {
//            Block *b = blocks.pop();
//            //delete b;
//        }
        *text << CECFileConstants::CloseBracket;
        text->flush();
        delete text;
    }
    blocks.clear();
}

bool CECFile::open(CEOpenMode om)
{
    //file = new QFile(_fileName);
    //if(file->open(QIODevice::WriteOnly | QIODevice::Text))
    //{
    text = new QTextStream(_stream, QIODevice::WriteOnly);
    *text << "void " << sFunctionName << "()" << CECFileConstants::NewLine;
    *text << CECFileConstants::OpenBracket << CECFileConstants::NewLine;
    openBrackets++;
    indentationNumber++;
    creatIndentationText(indentationNumber);
    blocks.push(Block());
    return true;
}

void CECFile::writeDeclarations(QList<CEVariableDeclared> *list)
{
    if(list->count() > 0)
    {
        for(int i = 0;i < list->count();i++)
            writeVariable(list->at(i));
        add(list->count());
    }
}

void CECFile::writeVariable(CEVariableDeclared v)
{
    QString s(v.getType());
    s.append(" " + v.getName());
    if(v.getExpression() != "")
        s.append(" = " + v.getExpression() + ";");
    else
        s.append(";");
    assignmentList.append(indentationText + s);
}

void CECFile::writeState(CEGraphicsItem *g)
{
}

void CECFile::writeTransition(CEGraphicsItem *g)
{
    QStringList *list = g->getAssignmentList();
    int n = 0;
    for(int i = 0;i<list->count();i++)
        if(list->at(i) != "")
        {
            assignmentList.append(indentationText + QString(list->at(i)) + ";");
            n++;
        }
    add(n);
}

void CECFile::writeStartIF(CEGraphicsItem *g)
{
    QStringList *list = g->getAssignmentList();
    QString s = list->count() == 0?"":QString(list->at(0));
    assignmentList.append(indentationText + "if(" + s + ")");
    add(1);
}

void CECFile::writeStopIF()
{
}

void CECFile::writeStartTrue()
{
    blocks.push(Block(NewBlock, assignmentList.count()));
    indentationNumber++;
    creatIndentationText(indentationNumber);
}

void CECFile::writeStopTrue()
{
    Block b = blocks.pop();
    indentationNumber--;
    creatIndentationText(indentationNumber);
    if(b.assignmentNumbers == 0)
    {
        creatIndentationText(indentationNumber + 1);
        assignmentList.append(indentationText + ";");
        creatIndentationText(indentationNumber - 1);
    }
    else if(b.assignmentNumbers > 1)
    {
        assignmentList.insert(b.lineNumber, indentationText + CECFileConstants::OpenBracket);
        assignmentList.append(indentationText + CECFileConstants::OpenBracket);
    }
    //delete b;
}

void CECFile::writeStartFalse()
{
    blocks.push(Block(NewBlock, assignmentList.count()));
    indentationNumber++;
    creatIndentationText(indentationNumber);
}

void CECFile::writeStopFalse()
{
    Block b = blocks.pop();
    indentationNumber--;
    creatIndentationText(indentationNumber);
    if(b.assignmentNumbers == 1)
    {
        assignmentList.insert(b.lineNumber, indentationText + "else");
    }
    else if(b.assignmentNumbers > 1)
    {
        assignmentList.insert(b.lineNumber, indentationText + CECFileConstants::OpenBracket);
        assignmentList.insert(b.lineNumber, indentationText + "else");
        assignmentList.append(indentationText + CECFileConstants::OpenBracket);
    }
    //delete b;
}

void CECFile::writeStartWHILE(CEGraphicsItem *g)
{
    QStringList *list = g->getAssignmentList();
    QString s = list->count() == 0?"":QString(list->at(0));
    assignmentList.append(indentationText + "while(" + s + ")");
    add(1);

    blocks.push(Block(NewBlock, assignmentList.count()));
    indentationNumber++;
    creatIndentationText(indentationNumber);
}

void CECFile::writeEndWHILE(CEGraphicsItem *g)
{
    Block b = blocks.pop();
    indentationNumber--;
    creatIndentationText(indentationNumber);
    if(b.assignmentNumbers == 0)
    {
        creatIndentationText(indentationNumber + 1);
        assignmentList.append(indentationText + ";");
        creatIndentationText(indentationNumber);
    }
    else if(b.assignmentNumbers > 1)
    {
        assignmentList.insert(b.lineNumber, indentationText + CECFileConstants::OpenBracket);
        assignmentList.append(indentationText + CECFileConstants::OpenBracket);
    }
}

void CECFile::writeStartFOR(CEGraphicsItem *g)
{
    QStringList *list = g->getAssignmentList();
    QString s1 = QString(list->at(0));
    QString s2 = QString(list->at(1));
    QString s3 = QString(list->at(2));
    assignmentList.append(indentationText + "for(" + s1 + ";" + s2 + ";" + s3 + ")");
    add(1);

    blocks.push(Block(NewBlock, assignmentList.count()));
    indentationNumber++;
    creatIndentationText(indentationNumber);
}

void CECFile::writeEndFOR(CEGraphicsItem *g)
{
    Block b = blocks.pop();
    indentationNumber--;
    creatIndentationText(indentationNumber);
    if(b.assignmentNumbers == 0)
    {
        creatIndentationText(indentationNumber + 1);
        assignmentList.append(indentationText + ";");
        creatIndentationText(indentationNumber);
    }
    else if(b.assignmentNumbers > 1)
    {
        assignmentList.insert(b.lineNumber, indentationText + CECFileConstants::OpenBracket);
        assignmentList.append(indentationText + CECFileConstants::OpenBracket);
    }
}

void CECFile::creatIndentationText(int n)
{
    indentationText.clear();
    for(int i = 0;i < n;i++)
        indentationText.append("\t");
}

void CECFile::add(int n)
{
    Block b = blocks.pop();
    b.add(n);
    blocks.push(b);
}

void CECFile::append(QString s)
{
    *text << s << CECFileConstants::NewLine;
}
