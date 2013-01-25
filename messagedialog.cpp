#include "messagedialog.h"
#include "ui_messagedialog.h"

MessageDialog::MessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageDialog)
{
    ui->setupUi(this);
}

MessageDialog::~MessageDialog()
{
    delete ui;
}

void MessageDialog::setMessageName(QString n) {
    ui->lineEditMessageName->setText(n);
    ui->lineEditMessageName->selectAll();
}

QString MessageDialog::getMessageName() {
    return ui->lineEditMessageName->text();
}
