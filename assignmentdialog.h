#ifndef ASSIGNMENTDIALOG_H
#define ASSIGNMENTDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QAbstractItemModel>
#include <QCompleter>
#include "autocompletiontextedit.h"
#include "treemodelcompleter.h"

class QAbstractItemModel;

static const QString qsIdentifier = "Identifier";

static const QString qsOperation = "Operation";

static const QString qsOperator = "Operator";

class AssignmentDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AssignmentDialog(QWidget *parent = 0);
    ~AssignmentDialog();

    QString getAssignmentText(){
        return assignmentText;
    }
    
private:

    void setUI();
    QAbstractItemModel *modelFromFileList(const QString& fileName);
    QAbstractItemModel *modelFromFileTree(const QString& fileName);

    TreeModelCompleter *completer;


    QLabel *lAssignment;
    AutocompletionTextEdit *autocompletionTextEdit;
    QPushButton *pbOk;
    QPushButton *pbCancel;

    QString assignmentText;

private slots:
    void ok_click();
    void cancel_click();

};

#endif // ASSIGNMENTDIALOG_H
