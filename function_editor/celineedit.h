#ifndef CELINEEDIT_H
#define CELINEEDIT_H

#include <QLineEdit>
#include <QCompleter>

class CELineEdit : public QLineEdit
{
    Q_OBJECT
public:
    CELineEdit(QWidget *parent = 0);
    ~CELineEdit();

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
