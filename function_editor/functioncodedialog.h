#ifndef FUNCTIONCODEDIALOG_H
#define FUNCTIONCODEDIALOG_H

#include <QDialog>
#include "treemodelcompleter.h"
#include "ui_functioncodedialog.h"

class FunctionCodeDialog : public QDialog, public Ui::FunctionCodeDialog
{
    Q_OBJECT

public:
    FunctionCodeDialog(FEGraphicsItem *i, QWidget *parent = 0);

private:
    QAbstractItemModel *modelFromFileTree(const QStringList& fileName);
    FEGraphicsItem *item_;
    TreeModelCompleter *completer;

private slots:
    void nameChanged(const QString &s);
};

#endif // FUNCTIONCODEDIALOG_H
