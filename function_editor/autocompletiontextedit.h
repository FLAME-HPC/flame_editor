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

 #ifndef AUTOCOMPLETIONTEXTEDIT_H
 #define AUTOCOMPLETIONTEXTEDIT_H

 #include <QTextEdit>
#include "./fegraphicsitem.h"

 class QCompleter;

 class AutocompletionTextEdit : public QTextEdit
 {
     Q_OBJECT

 public:
     AutocompletionTextEdit(QWidget *parent = 0);
     ~AutocompletionTextEdit();

     QStringList getAssignments() const;
     void setAssignments(QStringList list);
     void setType(ItemType item)
     {
         if(item == Transition || item == ConditionIf || item == ConditionWhile || item == ConditionFor)
             type = item;
         else
             type = Transition;
         multiAssignments = type == Transition;
     }

     bool isCorrect() const { return correct; }
     void testValid();
     void showMessage();

     bool isAfterShowMessageBox() const { return showMessageBox;}
     void setAfterShowMessageBox() { showMessageBox = false;}

     void setCompleter(QCompleter *c);
     QCompleter *completer() const;
     QString messageText;

 protected:
     void keyPressEvent(QKeyEvent *e);
     void focusInEvent(QFocusEvent *e);
     void focusOutEvent(QFocusEvent *);

 private slots:
     void insertCompletion(const QString &completion);
     void highlightCurrentLine();

 private:
     QString textUnderCursor() const;
     QString getCompletionPrefix(QString);
     bool valid(QString s);
     bool multiAssignments;
     bool showMessageBox;

 private:
     QCompleter *c;
     QTextCursor cursor;
     bool correct;

     ItemType type;
     QList<int> positions;

     bool testMoveCursor(int key, QTextCursor tc, QString str, int add = 0);
 };

 #endif // AUTOCOMPLETIONTEXTEDIT_H
