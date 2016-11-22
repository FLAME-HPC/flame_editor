/*!
 * \file timeunitdialog.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for the time unit dialog
*/
#ifndef TIMEUNITDIALOG_H_
#define TIMEUNITDIALOG_H_

#include <QDialog>
#include "./machine.h"

namespace Ui {
    class TimeUnitDialog;
}

class TimeUnitDialog : public QDialog {
    Q_OBJECT

  public:
    explicit TimeUnitDialog(Machine * m, QWidget *parent = 0);
    ~TimeUnitDialog();
    void setTimeUnit(TimeUnit t);
    TimeUnit getTimeUnit();

  private:
    Ui::TimeUnitDialog *ui;
    Machine * machine;
    TimeUnit timeUnit;
};

#endif  // TIMEUNITDIALOG_H_
