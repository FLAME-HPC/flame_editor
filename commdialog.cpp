/*!
 * \file commdialog.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of message communication dialog
 */
#include "./commdialog.h"
#include "./messagemodel.h"
#include "./mpredelegate.h"
#include "./messagedelegate.h"
#include "./sortdelegate.h"

CommDialog::CommDialog(Machine * m, QWidget *parent)
    : QDialog(parent) {
    setupUi(this);

    machine = m;

    QHeaderView *headerView = tableView_messages->horizontalHeader();
    headerView->setResizeMode(QHeaderView::Stretch);
    headerView->setResizeMode(1, QHeaderView::Interactive);
    tableView_messages->verticalHeader()->hide();
    tableView_messages->setItemDelegateForColumn(0,
            new MessageDelegate(m->getMessageNames()));
}

void CommDialog::setCommunication(Communication c) {
    comm = c;

    tableView_messages->setModel(comm.messageModel);
    tableView_messages->setItemDelegateForColumn(1,
            new MpreDelegate(machine, &comm));
    tableView_messages->setItemDelegateForColumn(2,
            new SortDelegate(machine, &comm));
}

Communication CommDialog::getCommunication() {
    return comm;
}

void CommDialog::on_pushButton_plus_clicked() {
    QStringList messages = machine->getMessageNames();
    if (messages.count() > 0) {
        comm.messageModel->addMessage(messages.first());
    } else {
        QMessageBox::warning(this, tr("FLAME Editor"),
          tr("There are no message types associated with this model to add."));
    }
}

void CommDialog::on_pushButton_minus_clicked() {
    QModelIndexList indexList =
            tableView_messages->selectionModel()->selectedRows();

    while (indexList.count() > 0) {
        comm.messageModel->removeRow(indexList.at(0).row());
        indexList = tableView_messages->selectionModel()->selectedRows();
    }
}
