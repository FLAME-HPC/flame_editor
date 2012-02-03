/*!
 * \file mpredialog.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for mpre dialog
*/
#ifndef MPREDIALOG_H_
#define MPREDIALOG_H_

#include <QDialog>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include "./mpre.h"
#include "./memorymodel.h"
#include "./condition.h"
#include "./ui_conditiondialog.h"
#include "./machine.h"

class QDialogButtonBox;

class MpreDialog : public QDialog, public Ui::ConditionDialog {
    Q_OBJECT

  public:
    MpreDialog(Machine * agent, QString * messageType = 0, QWidget *parent = 0);
    void setCondition(Condition c);
    Condition getCondition();

  signals:
    void setVariableComboBox(int i);
    void setOpComboBox(int i);

  private slots:
    void enableCondition(bool);
    void checkedLhsAgentVariable(bool);
    void checkedLhsMessageVariable(bool);
    void checkedLhsValue(bool);
    void checkedRhsAgentVariable(bool);
    void checkedRhsMessageVariable(bool);
    void checkedRhsValue(bool);
    void valueClicked();
    void timeClicked();
    void nestedClicked();
    void editLhsClicked();
    void editRhsClicked();
    void disableTimePhaseVariable(bool);
    void disableTimePhaseValue(bool);
    void levelUpClicked();

  private:
    void setCurrentCondition(Condition * c);
    void getCurrentCondition();
    QDialogButtonBox *buttonBox;
    QComboBox *variable;
    QComboBox * op;
    QSpinBox * value;
    QCheckBox * myEnabled;
    QGroupBox *lhsGroup;
    QGroupBox *opGroup;
    QGroupBox *rhsGroup;
    Condition condition;
    Condition * c;
    Machine * agent;
    QStringList operators;
};

#endif  // MPREDIALOG_H_
