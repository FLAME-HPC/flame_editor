#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>
#include <QCompleter>

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    LineEdit(QWidget *parent = 0);
    ~LineEdit();

    void setCompleter(QCompleter *completer);
    QCompleter *completer() const;

protected:
    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e);

private slots:
    void insertCompletion(const QString &completion);

private:
    QCompleter *c;
    QString getCompletionPrefix(QString input);
};

#endif // LINEEDIT_H
