#ifndef STATEDIALOG_H
#define STATEDIALOG_H

#include <QDialog>
#include "state.h"

namespace Ui {
class StateDialog;
}

class StateDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit StateDialog(QWidget *parent = 0);
    ~StateDialog();
    void setStateName(QString n);
    QString getStateName();

private:
    Ui::StateDialog *ui;
    State * state_;
};

#endif // STATEDIALOG_H
