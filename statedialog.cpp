#include "statedialog.h"
#include "ui_statedialog.h"

StateDialog::StateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StateDialog)
{
    ui->setupUi(this);
}

StateDialog::~StateDialog()
{
    delete ui;
}

void StateDialog::setStateName(QString n) {
    ui->lineEditStateName->setText(n);
    ui->lineEditStateName->selectAll();
}

QString StateDialog::getStateName() {
    return ui->lineEditStateName->text();
}
