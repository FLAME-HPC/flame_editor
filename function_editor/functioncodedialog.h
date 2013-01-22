#ifndef FUNCTIONCODEDIALOG_H
#define FUNCTIONCODEDIALOG_H

#include <QDialog>
#include "cetreemodelcompleter.h"
#include "ui_functioncodedialog.h"

class FunctionCodeDialog : public QDialog, public Ui::FunctionCodeDialog
{
    Q_OBJECT

public:
    FunctionCodeDialog(CEGraphicsItem *i, QStringList variableNames, QWidget *parent = 0);

private:
    QAbstractItemModel *modelFromFileTree(const QStringList& fileName);
    CEGraphicsItem *item_;
    CETreeModelCompleter *completer;

private slots:
    void nameChanged(const QString &s);
    void on_pushButton_clicked();
};

#endif // FUNCTIONCODEDIALOG_H
