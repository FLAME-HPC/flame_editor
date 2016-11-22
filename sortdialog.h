/*!
 * \file sortdialog.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for the message sort dialog
*/
#ifndef SORTDIALOG_H_
#define SORTDIALOG_H_

#include <QDialog>
#include "./messagesort.h"
#include "./ui_sortdialog.h"
#include "./machine.h"

class SortDialog : public QDialog, public Ui::SortDialog {
    Q_OBJECT

  public:
    SortDialog(Machine * m, QString * messageType = 0, QWidget *parent = 0);
    void setSort(MessageSort s);
    MessageSort getSort();

  private slots:
    void clickedRandom();
    void clickedSort();
    void clickedNotRandomOrSort();

  private:
    MessageSort sort;
    Machine * machine;
};

#endif  // SORTDIALOG_H_
