#ifndef ASSIGNMENTDIALOG_H
#define ASSIGNMENTDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

static const QString qsIdentifier = "Identifier";

static const QString qsOperation = "Operation";

static const QString qsOperator = "Operator";

class AssignmentDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AssignmentDialog(QWidget *parent = 0);
    ~AssignmentDialog();
    
private:

    void setUI();


    QLabel *lAssignment;
    QPushButton *pbOk;
    QPushButton *pbCancel;

    QLabel *label1;
    QComboBox *comboBox1;

    QLabel *label2;
    QComboBox *comboBox2;

    QLabel *label3;
    QComboBox *comboBox3;

    QPushButton *pbAdd;
    QPushButton *pbRemove;
};

#endif // ASSIGNMENTDIALOG_H
