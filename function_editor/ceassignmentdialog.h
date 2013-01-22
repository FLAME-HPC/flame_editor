#ifndef CEASSIGNMENTDIALOG_H
#define CEASSIGNMENTDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QAbstractItemModel>
#include <QCompleter>
#include "ceautocompletiontextedit.h"
#include "cetreemodelcompleter.h"

class QAbstractItemModel;

static const QString qsIdentifier = "Identifier";

static const QString qsOperation = "Operation";

static const QString qsOperator = "Operator";

class CEAssignmentDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CEAssignmentDialog(QWidget *parent = 0);
    ~CEAssignmentDialog();

    QString getAssignmentText(){
        return assignmentText;
    }
    
private:

    void setUI();
    QAbstractItemModel *modelFromFileList(const QString& fileName);
    QAbstractItemModel *modelFromFileTree(const QString& fileName);

    CETreeModelCompleter *completer;


    QLabel *lAssignment;
    CEAutocompletionTextEdit *autocompletionTextEdit;
    QPushButton *pbOk;
    QPushButton *pbCancel;

    QString assignmentText;

private slots:
    void ok_click();
    void cancel_click();

};

#endif // ASSIGNMENTDIALOG_H
