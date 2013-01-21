#include "celineedit.h"
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QtDebug>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>

CELineEdit::CELineEdit(QWidget *parent) : QLineEdit(parent), c(0)
{
}

CELineEdit::~CELineEdit()
{
}

void CELineEdit::setCompleter(QCompleter *completer)
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

QCompleter *CELineEdit::completer() const
{
    return c;
}

void CELineEdit::keyPressEvent(QKeyEvent *e)
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
        insert("()");
        setCursorPosition(cursorPosition() - 1);
        e->ignore();
        return;
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!c || !isShortcut) // do not process the shortcut when we have a completer
        QLineEdit::keyPressEvent(e);

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

void CELineEdit::focusInEvent(QFocusEvent *e)
{
    if (c)
        c->setWidget(this);
    QLineEdit::focusInEvent(e);
}

void CELineEdit::insertCompletion(const QString &completion)
{
    if (c->widget() != this)
        return;
    int extra = completion.length() - c->completionPrefix().length();
    insert(completion.right(extra));
}

QString CELineEdit::getCompletionPrefix(QString input)
{
    int i=cursorPosition();
    QString s=text();
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
