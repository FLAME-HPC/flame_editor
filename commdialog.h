/*!
 * \file commdialog.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for message communication dialog
 */
#ifndef COMMDIALOG_H_
#define COMMDIALOG_H_

#include <QDialog>
#include "./machine.h"
#include "./communication.h"
#include "./ui_commdialog.h"

class CommDialog : public QDialog, public Ui::CommDialog {
    Q_OBJECT

  public:
    CommDialog(Machine * machine, QWidget *parent = 0);
    void setCommunication(Communication c);
    Communication getCommunication();

  private slots:
    void on_pushButton_plus_clicked();
    void on_pushButton_minus_clicked();

  private:
    Machine * machine;
    Communication comm;
};

#endif  // COMMDIALOG_H_
