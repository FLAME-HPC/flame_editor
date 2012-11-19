/****************************************************************************
 **
 ** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

 #include "autocompletiontextedit.h"
 #include <QCompleter>
 #include <QKeyEvent>
 #include <QAbstractItemView>
 #include <QtDebug>
 #include <QApplication>
 #include <QModelIndex>
 #include <QAbstractItemModel>
 #include <QScrollBar>
 #include <QTextBlock>
 #include <QMessageBox>

 //#include "./linkedlist.h"
 //#include "parser/y.tab.h"
 #include "./codeparser.h"

//extern "C" {

   //char *yytext;
   //int textToParse(char *p);
   //void textToScanner(char *p);
   //int yylex();

//}

 AutocompletionTextEdit::AutocompletionTextEdit(QWidget *parent)
 : QTextEdit(parent), c(0)
 {
     //setPlainText(tr("This TextEdit provides autocompletions for words that have more than"
     //                " 3 characters. You can trigger autocompletion using ") +
     //                QKeySequence("Ctrl+E").toString(QKeySequence::NativeText));

     //this->verticalScrollBar()->hide();
     //this->horizontalScrollBar()->hide();
     //this->horizontalScrollBar()->setVisible(false);

     //this->setFrameShape(QTextEdit::NoFrame);
     //this->setFrameShadow(QTextEdit::Plain);

     connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

     highlightCurrentLine();

     multiAssignments = true;

     showMessageBox = false;

     correct = false;
 }

 AutocompletionTextEdit::~AutocompletionTextEdit()
 {
 }

 QStringList AutocompletionTextEdit::getAssignments() const
 {
     QStringList list;
     if(type == Transition)
     {
     for(int i=0;i<document()->blockCount();i++)
         if(document()->findBlockByNumber(i).text() != "")
             list.append(document()->findBlockByNumber(i).text());
     }
     else
     {
         QTextCursor tc = textCursor();
         QString str = tc.block().text();
         if(str != "")
         {
             int min, max;
             for(int i = 0;i<positions.count() - 1;i++)
             {
                 min = positions[i];
                 max = positions[i+1];
                 if(max - min > 1)
                     list.append(str.mid(min + 1, max - min - 1));
                 else
                     list.append("");
             }
         }
     }
     return list;
 }

 void AutocompletionTextEdit::setAssignments(QStringList list)
 {
     QString s = "";
     positions.clear();
     if(type == Transition)
     {
         for(int i=0;i<list.count();i++){
             s.append(list[i]);
             if(i<list.count()-1)
                 s.append("\n");
         }
         setPlainText(s);
     }
     else if(type == ConditionIf)
     {
         s.append("if(");
         positions.append(s.length() - 1);
         if(list.count() > 0)
             s.append(list[0]);
         s.append(")");
         positions.append(s.length() - 1);
         setPlainText(s);
         QTextCursor tc = textCursor();
         tc.setPosition(positions[0] + 1);
         setTextCursor(tc);
     }
     else if(type == ConditionWhile)
     {
         s.append("while(");
         positions.append(s.length() - 1);
         if(list.count() > 0)
             s.append(list[0]);
         s.append(")");
         positions.append(s.length() - 1);
         setPlainText(s);
         QTextCursor tc = textCursor();
         tc.setPosition(positions[0] + 1);
         setTextCursor(tc);
     }
     else if(type == ConditionFor)
     {
         s.append("for(");
         positions.append(s.length() - 1);
         for(int i = 0;i<3;i++)
         {
             if(i < list.count())
                 s.append(list[i]);
             if(i < 2)
             {
                 s.append(";");
                 positions.append(s.length() - 1);
             }
         }
         s.append(")");
         positions.append(s.length() - 1);
         setPlainText(s);
         QTextCursor tc = textCursor();
         tc.setPosition(positions[0] + 1);
         setTextCursor(tc);
     }
     cursor = textCursor();
 }

 void AutocompletionTextEdit::testValid()
 {
     QTextCursor tc = textCursor();
     if(tc.block().text() != "")
         valid(tc.block().text());
 }

 void AutocompletionTextEdit::showMessage()
 {
     if(isCorrect() == false){
         showMessageBox = true;
         //correct = true;
         QMessageBox::warning(qobject_cast<QWidget*>( this->parent()), tr(""), messageText);
     }
 }

 void AutocompletionTextEdit::focusInEvent(QFocusEvent *e)
 {
     qDebug()<<"            innnnnnnnn";

     showMessageBox = false;

     if (c)
         c->setWidget(this);
     QTextEdit::focusInEvent(e);
 }

 void AutocompletionTextEdit::focusOutEvent(QFocusEvent */*e*/)
 {
     qDebug()<<"           paaaaaaaaa";
     if(showMessageBox == false){
         qDebug()<<"pa";
         QTextCursor tc = textCursor();
         qDebug()<<tc.blockNumber()<<"  "<<tc.block().text();
         qDebug()<<"ok";
         if(tc.block().text() != "")
             valid(tc.block().text());
         else
             correct = true;
     }
     else
         showMessageBox = false;
 }

 void AutocompletionTextEdit::setCompleter(QCompleter *completer)
 {
     if (c)
         QObject::disconnect(c, 0, this, 0);

     c = completer;

     if (!c)
         return;

     c->setWidget(this);
     c->setCompletionMode(QCompleter::PopupCompletion);
     c->setCaseSensitivity(Qt::CaseInsensitive);
     QObject::connect(c, SIGNAL(activated(QString)),
                      this, SLOT(insertCompletion(QString)));
 }

 QCompleter *AutocompletionTextEdit::completer() const
 {
     return c;
 }

 void AutocompletionTextEdit::insertCompletion(const QString& completion)
 {
     if (c->widget() != this)
         return;
     QTextCursor tc = textCursor();
     int extra = completion.length() - c->completionPrefix().length();
     //tc.movePosition(QTextCursor::Left);
     //tc.movePosition(QTextCursor::EndOfWord);
     if(multiAssignments == false)
         testMoveCursor(0, tc, "", extra);
     tc.insertText(completion.right(extra));
     setTextCursor(tc);
 }

 void AutocompletionTextEdit::highlightCurrentLine()
 {
     QList<QTextEdit::ExtraSelection> extraSelections;

     if (!isReadOnly()) {
         QTextEdit::ExtraSelection selection;

         QColor lineColor = QColor(Qt::yellow).lighter(160);

         selection.format.setBackground(lineColor);
         selection.format.setProperty(QTextFormat::FullWidthSelection, true);
         selection.cursor = textCursor();
         selection.cursor.clearSelection();
         extraSelections.append(selection);
     }

     setExtraSelections(extraSelections);

     QTextCursor tc = textCursor();
     if(multiAssignments == false)
         testMoveCursor(0, tc, "");
     //qDebug()<<tc.blockNumber()<<"  "<<tc.block().text();
     if(cursor.blockNumber() != tc.blockNumber()){
         qDebug()<<"ok";
         if(cursor.block().text() =="")
             setTextCursor(cursor);
         if(valid(cursor.block().text()) == false){
             QMessageBox::warning(qobject_cast<QWidget*>( this->parent()), tr(""), messageText);
             setTextCursor(cursor);
         }
     }
 }

 QString AutocompletionTextEdit::textUnderCursor() const
 {
     QTextCursor tc = textCursor();
     tc.select(QTextCursor::WordUnderCursor);
     return tc.selectedText();
 }

 QString AutocompletionTextEdit::getCompletionPrefix(QString /*input*/)
 {
     QTextCursor tc = textCursor();
     int i=tc.position();
     QString s=toPlainText();
     QString ss=s.left(i);

     QRegExp re("[A-Za-z0-9\\.\\_]");
     int d=-1;
     for(int ii=i-1;ii>=0 && re.exactMatch(ss.mid(ii,1));ii--)
         d=ii;
     if(d != -1){
         ss=ss.mid(d,i-d);
         QRegExp rg("[A-Za-z\\_][A-Za-z0-9\\_]*(\\.[A-Za-z\\_][A-Za-z0-9\\_]*)*[\\.]?");
         if(!rg.exactMatch(ss))
             ss="";
     }
     else
         ss="";
     //qDebug()<<ss<<"  "<<d;
     return ss;
 }

 bool AutocompletionTextEdit::valid(QString s)
 {
     correct = true;
         int i = CodeParser::setParseToTest(s);
         if(i!=0){
             QString es(CodeParser::getToken());
             QString sm(s);
             sm = sm.insert(i-1, "<b>");
             sm.append("</b>");
             messageText = tr("Incorrect token in text appears in at the beginning bold part ") + "\"" + sm + "\"";
             correct = false;
             qDebug()<<i<<" "<<es;
             return false;
         }

         QStringList list = CodeParser::getIdentifierList(s);
         for(int i=0; i < list.length();i++)
             qDebug()<<list[i];

         return true;
 }

 bool AutocompletionTextEdit::testMoveCursor(int key, QTextCursor tc, QString str, int add)
 {
     bool b = false;
     int min = -1, max = -1;
     int i = -1;
     for(int ii = 0;ii<positions.count() - 1;ii++)
         if(positions[ii] < tc.position() && tc.position() <= positions[ii+1])
         {
             min = positions[ii];
             max = positions[ii+1];
             i = ii + 1;
         }
     qDebug()<<min<<tc.position()<<max;
     if(positions.count() == 0)
         b = true;
     else if(min == -1)
     {
         if(positions.count() > 0)
         {
             if(tc.position() <= positions[0])
                 tc.setPosition(positions[0] + 1);
             else
                 tc.setPosition(positions[positions.count() - 1]);
             setTextCursor(tc);
         }
         b = true;
     }
     else if(add > 0)
     {
         for(;i<positions.count();i++)
             positions[i] += add;
     }
     else if(!(key == Qt::Key_Backspace || key == Qt::Key_Delete) && str.length() > 0)
     {
         for(;i<positions.count();i++)
             positions[i] += str.length();
     }
     else if(min == tc.position() - 1 && i == 1 && key == Qt::Key_Left)
         b = true;
     else if(key == Qt::Key_Backspace)
     {
         if(min == tc.position()- 1)
             b = true;
         else
             for(;i<positions.count();i++)
                 positions[i] -= 1;
     }
     else if(max == tc.position() && i == positions.count() - 1 && key == Qt::Key_Right)
         b = true;
     else if(key == Qt::Key_Delete)
     {
         if(max == tc.position())
             b = true;
         else
             for(;i<positions.count();i++)
                 positions[i] -= 1;
     }
     else if(key == Qt::Key_Home)
     {
         tc.setPosition(positions[i-1] + 1);
         setTextCursor(tc);
         b = true;
     }
     else if(key == Qt::Key_End)
     {
         tc.setPosition(positions[i]);
         setTextCursor(tc);
         b = true;
     }
     return b;
 }

 void AutocompletionTextEdit::keyPressEvent(QKeyEvent *e)
 {
     if (c && c->popup()->isVisible()) {
         // The following keys are forwarded by the completer to the widget
        switch (e->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
             e->ignore();
             return; // let the completer do default behavior
        default:
            break;
        }
     }

     if(e->text() == "("){
         QTextCursor tc = textCursor();
         if(multiAssignments == false)
             testMoveCursor(e->key(), tc, "()");
         tc.insertText("()");
         tc.movePosition(QTextCursor::Left);
         setTextCursor(tc);
         e->ignore();
         return;
     }

     QTextCursor tc = textCursor();
     qDebug()<<tc.position()<<e->text();

     if(multiAssignments == false)
     {
         if((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_D)
         {
             positions.clear();
             tc.select(QTextCursor::BlockUnderCursor);
             tc.deleteChar();
             setTextCursor(tc);
             e->ignore();
             return;
         }
         bool b = testMoveCursor(e->key(), tc, e->text());
         if(b)
         {
             e->ignore();
             return;
         }
     }

     //|| (e->key() == Qt::Key_Up && tc.blockNumber() > 0) || (e->key() == Qt::Key_Down && tc.blockNumber() < document()->blockCount())
     if(e->key() == Qt::Key_Return){
         if(tc.block().text() == ""){
             e->ignore();
             return;
         }
         if(valid(tc.block().text()) == false){
             QMessageBox::warning(qobject_cast<QWidget*>( this->parent()), tr(""), messageText);
             e->ignore();
             return;
         }
         if(multiAssignments == false)
         {
             e->ignore();
             return;
         }
         tc.movePosition(QTextCursor::EndOfLine);
         setTextCursor(tc);

     }

     cursor = textCursor();

     bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
     if (!c || !isShortcut) // do not process the shortcut when we have a completer
         QTextEdit::keyPressEvent(e);

     const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
     if (!c || (ctrlOrShift && e->text().isEmpty()))
         return;

     static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
     bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
     QString completionPrefix = getCompletionPrefix(e->text()); //textUnderCursor();


     if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 1
                       //|| eow.contains(e->text().right(1))
                         )
             ) {
         c->popup()->hide();
         return;
     }

     //qDebug()<<completionPrefix;

     if (completionPrefix != c->completionPrefix()) {
         c->setCompletionPrefix(completionPrefix);
         c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
     }
     QRect cr = cursorRect();
     cr.setWidth(c->popup()->sizeHintForColumn(0)
                 + c->popup()->verticalScrollBar()->sizeHint().width());
     c->complete(cr); // popup it up!
 }
