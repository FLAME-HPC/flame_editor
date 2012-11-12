#include "cfile.h"

CFile::CFile(QString fileName, QString FunctionName) : FileType(fileName, FunctionName)
{
    file = 0;
    text = 0;
    openBrackets = 0;
    indentationNumber = 0;
    indentationText = "";
}

CFile::~CFile()
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
        *text << CFileConstants::CloseBracket;
        text->flush();
        delete text;
    }
    if(file != 0)
    {
        file->close();
        delete file;
    }
    blocks.clear();
}

bool CFile::open(OpenMode om)
{
    file = new QFile(_fileName);
    if(file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        text = new QTextStream(file);
        *text << "void " << sFunctionName << "()" << CFileConstants::NewLine;
        *text << CFileConstants::OpenBracket << CFileConstants::NewLine;
        openBrackets++;
        indentationNumber++;
        creatIndentationText(indentationNumber);
        blocks.push(Block());
        return true;
    }
    else
        return false;
}

void CFile::writeState(GraphicsItem *g)
{
}

void CFile::writeTransition(GraphicsItem *g)
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

void CFile::writeStartIF(GraphicsItem *g)
{
    QStringList *list = g->getAssignmentList();
    QString s = list->count() == 0?"":QString(list->at(0));
    assignmentList.append(indentationText + "if(" + s + ")");
    add(1);
}

void CFile::writeStopIF()
{
}

void CFile::writeStartTrue()
{
    blocks.push(Block(NewBlock, assignmentList.count()));
    indentationNumber++;
    creatIndentationText(indentationNumber);
}

void CFile::writeStopTrue()
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
        assignmentList.insert(b.lineNumber, indentationText + CFileConstants::OpenBracket);
        assignmentList.append(indentationText + CFileConstants::OpenBracket);
    }
    //delete b;
}

void CFile::writeStartFalse()
{
    blocks.push(Block(NewBlock, assignmentList.count()));
    indentationNumber++;
    creatIndentationText(indentationNumber);
}

void CFile::writeStopFalse()
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
        assignmentList.insert(b.lineNumber, indentationText + CFileConstants::OpenBracket);
        assignmentList.insert(b.lineNumber, indentationText + "else");
        assignmentList.append(indentationText + CFileConstants::OpenBracket);
    }
    //delete b;
}

void CFile::writeStartWHILE(GraphicsItem *g)
{
    QStringList *list = g->getAssignmentList();
    QString s = list->count() == 0?"":QString(list->at(0));
    assignmentList.append(indentationText + "while(" + s + ")");
    add(1);

    blocks.push(Block(NewBlock, assignmentList.count()));
    indentationNumber++;
    creatIndentationText(indentationNumber);
}

void CFile::writeEndWHILE(GraphicsItem *g)
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
        assignmentList.insert(b.lineNumber, indentationText + CFileConstants::OpenBracket);
        assignmentList.append(indentationText + CFileConstants::OpenBracket);
    }
}

void CFile::writeStartFOR(GraphicsItem *g)
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

void CFile::writeEndFOR(GraphicsItem *g)
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
        assignmentList.insert(b.lineNumber, indentationText + CFileConstants::OpenBracket);
        assignmentList.append(indentationText + CFileConstants::OpenBracket);
    }
}

void CFile::creatIndentationText(int n)
{
    indentationText.clear();
    for(int i = 0;i < n;i++)
        indentationText.append("\t");
}

void CFile::add(int n)
{
    Block b = blocks.pop();
    b.add(n);
    blocks.push(b);
}

void CFile::append(QString s)
{
    *text << s << CFileConstants::NewLine;
}
