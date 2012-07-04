/*!
 * \file mpredialog.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of mpre dialog
 */
#include <QtGui>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "./mpredialog.h"

MpreDialog::MpreDialog(Machine * a, QString * mt, QWidget *parent)
     : QDialog(parent) {
    setupUi(this);

    agent = a;

    /* Initialise interfaces */
    QStringList operators;
    operators << "==" << "!=" << "<" << ">" << "<=" << ">=" << "IN";
    comboBox_valueOp->addItems(operators);
    operators.clear();
    operators << "AND" << "OR";
    comboBox_nestedOp->addItems(operators);
    QStringList agentMemoryNames = agent->getAgentMemoryNames(agent->name);
    comboBox_lhsAgentVariable->addItems(agentMemoryNames);
    comboBox_rhsAgentVariable->addItems(agentMemoryNames);
    comboBox_period->addItems(agent->getTimeUnits());
    comboBox_phaseVariable->addItems(agentMemoryNames);
    if (mt == 0) {  // if agent then disable message variables
        checkBox_lhsMessageVariable->setEnabled(false);
        comboBox_lhsMessageVariable->setEnabled(false);
        checkBox_rhsMessageVariable->setEnabled(false);
        comboBox_rhsMessageVariable->setEnabled(false);
    } else {  // if message then populate message variable combo boxes
        QStringList messageMemoryNames = agent->getMessageMemoryNames(*mt);
        comboBox_lhsMessageVariable->addItems(messageMemoryNames);
        comboBox_rhsMessageVariable->addItems(messageMemoryNames);
    }

    /* Check interfaces to functions */
    connect(checkBox_enabled, SIGNAL(clicked(bool)),
            this, SLOT(enableCondition(bool)));
    connect(checkBox_lhsAgentVariable, SIGNAL(clicked(bool)),
            this, SLOT(checkedLhsAgentVariable(bool)));
    connect(checkBox_lhsMessageVariable, SIGNAL(clicked(bool)),
            this, SLOT(checkedLhsMessageVariable(bool)));
    connect(checkBox_lhsValue, SIGNAL(clicked(bool)),
            this, SLOT(checkedLhsValue(bool)));
    connect(checkBox_rhsAgentVariable, SIGNAL(clicked(bool)),
            this, SLOT(checkedRhsAgentVariable(bool)));
    connect(checkBox_rhsMessageVariable, SIGNAL(clicked(bool)),
            this, SLOT(checkedRhsMessageVariable(bool)));
    connect(checkBox_rhsValue, SIGNAL(clicked(bool)),
            this, SLOT(checkedRhsValue(bool)));
    connect(radioButton_value, SIGNAL(clicked()), this, SLOT(valueClicked()));
    connect(radioButton_time, SIGNAL(clicked()), this, SLOT(timeClicked()));
    connect(radioButton_nested, SIGNAL(clicked()), this, SLOT(nestedClicked()));
    connect(pushButton_editLhs, SIGNAL(clicked()),
            this, SLOT(editLhsClicked()));
    connect(pushButton_editRhs, SIGNAL(clicked()),
            this, SLOT(editRhsClicked()));
    connect(checkBox_phaseVariable, SIGNAL(clicked(bool)),
            this, SLOT(disableTimePhaseValue(bool)));
    connect(checkBox_phaseValue, SIGNAL(clicked(bool)),
            this, SLOT(disableTimePhaseVariable(bool)));
    connect(pushButton_levelUp, SIGNAL(clicked()),
            this, SLOT(levelUpClicked()));
}

/*!
 * \brief Sets up dialog using given condition
 * \param[in] cond The condition
 *
 * Sets up dialog interface using given condition.
 */
void MpreDialog::setCurrentCondition(Condition *cond) {
    c = cond;

    if (c->enabled) lineEdit_condition->setText(c->toString());
    else
        lineEdit_condition->setText("");

    /* If not root condition */
    if (c->parentCondition) {
        checkBox_enabled->setChecked(true);
        checkBox_enabled->setEnabled(false);
        pushButton_levelUp->setEnabled(true);
    } else {
        checkBox_enabled->setEnabled(true);
        pushButton_levelUp->setEnabled(false);
    }

    checkBox_enabled->setChecked(c->enabled);
    enableCondition(c->enabled);

    /* If condition uses values */
    if (c->isValues) {
        if (c->enabled) {
            groupBox_value->setEnabled(true);
            groupBox_time->setEnabled(false);
            groupBox_nested->setEnabled(false);
        }

        radioButton_value->setChecked(true);
        checkBox_notValue->setChecked(c->isNot);
        checkBox_lhsAgentVariable->setChecked(c->lhsIsAgentVariable);
        checkBox_lhsMessageVariable->setChecked(c->lhsIsMessageVariable);
        checkBox_lhsValue->setChecked(c->lhsIsValue);
        comboBox_lhsAgentVariable->setEnabled(c->lhsIsAgentVariable);
        comboBox_lhsMessageVariable->setEnabled(c->lhsIsMessageVariable);
        doubleSpinBox_lhsValue->setEnabled(c->lhsIsValue);
        doubleSpinBox_lhsValue->setValue(c->lhsDouble);
        comboBox_lhsAgentVariable->setCurrentIndex(0);
        if (c->lhsIsAgentVariable) {
            for (int i = 0; i < comboBox_lhsAgentVariable->count(); i++) {
                if (QString::compare(comboBox_lhsAgentVariable->itemText(i),
                        c->lhs) == 0)
                    comboBox_lhsAgentVariable->setCurrentIndex(i);
            }
        }
        comboBox_lhsMessageVariable->setCurrentIndex(0);
        if (c->lhsIsMessageVariable) {
            for (int i = 0; i < comboBox_lhsMessageVariable->count(); i++) {
                if (QString::compare(comboBox_lhsMessageVariable->itemText(i),
                        c->lhs) == 0)
                    comboBox_lhsMessageVariable->setCurrentIndex(i);
            }
        }

        checkBox_rhsAgentVariable->setChecked(c->rhsIsAgentVariable);
        checkBox_rhsMessageVariable->setChecked(c->rhsIsMessageVariable);
        checkBox_rhsValue->setChecked(c->rhsIsValue);
        comboBox_rhsAgentVariable->setEnabled(c->rhsIsAgentVariable);
        comboBox_rhsMessageVariable->setEnabled(c->rhsIsMessageVariable);
        doubleSpinBox_rhsValue->setEnabled(c->rhsIsValue);
        doubleSpinBox_rhsValue->setValue(c->rhsDouble);
        comboBox_rhsAgentVariable->setCurrentIndex(0);
        if (c->rhsIsAgentVariable) {
            for (int i = 0; i < comboBox_rhsAgentVariable->count(); i++) {
                if (QString::compare(comboBox_rhsAgentVariable->itemText(i),
                        c->rhs) == 0)
                    comboBox_rhsAgentVariable->setCurrentIndex(i);
            }
        }
        comboBox_rhsMessageVariable->setCurrentIndex(0);
        if (c->rhsIsMessageVariable) {
            for (int i = 0; i < comboBox_rhsMessageVariable->count(); i++) {
                if (QString::compare(comboBox_rhsMessageVariable->itemText(i),
                        c->rhs) == 0)
                    comboBox_rhsMessageVariable->setCurrentIndex(i);
            }
        }

        for (int i = 0; i < comboBox_valueOp->count(); i++) {
            if (QString::compare(comboBox_valueOp->itemText(i), c->op) == 0)
                comboBox_valueOp->setCurrentIndex(i);
        }
    } else {
        checkBox_notValue->setChecked(false);
        checkBox_lhsAgentVariable->setChecked(false);
        checkBox_lhsValue->setChecked(true);
        comboBox_lhsAgentVariable->setEnabled(false);
        doubleSpinBox_lhsValue->setEnabled(true);
        doubleSpinBox_lhsValue->setValue(0.0);
        comboBox_lhsAgentVariable->setCurrentIndex(0);

        checkBox_rhsAgentVariable->setChecked(false);
        checkBox_rhsValue->setChecked(true);
        comboBox_rhsAgentVariable->setEnabled(false);
        doubleSpinBox_rhsValue->setEnabled(true);
        doubleSpinBox_rhsValue->setValue(0.0);
        comboBox_rhsAgentVariable->setCurrentIndex(0);

        comboBox_valueOp->setCurrentIndex(0);
    }

    /* If condition is time based */
    if (c->isTime) {
        if (c->enabled) {
            groupBox_value->setEnabled(false);
            groupBox_time->setEnabled(true);
            groupBox_nested->setEnabled(false);
        }

        radioButton_time->setChecked(true);
        checkBox_notTime->setChecked(c->isNot);
        if (c->timePeriod == "") {
            comboBox_period->setCurrentIndex(0);
        } else {
            for (int i = 0; i < comboBox_period->count(); i++) {
                if (QString::compare(comboBox_period->itemText(i),
                        c->timePeriod) == 0)
                    comboBox_period->setCurrentIndex(i);
            }
        }
        checkBox_phaseVariable->setChecked(c->timePhaseIsVariable);
        checkBox_phaseValue->setChecked(!c->timePhaseIsVariable);
        comboBox_phaseVariable->setEnabled(c->timePhaseIsVariable);
        spinBox_phaseValue->setEnabled(!c->timePhaseIsVariable);
        if (c->timePhaseIsVariable) {
            if (c->timePhaseVariable == "") {
                comboBox_phaseVariable->setCurrentIndex(0);
            } else {
                for (int i = 0; i < comboBox_phaseVariable->count(); i++) {
                    if (QString::compare(comboBox_phaseVariable->itemText(i),
                            c->timePhaseVariable) == 0)
                        comboBox_phaseVariable->setCurrentIndex(i);
                }
            }
        } else {
            spinBox_phaseValue->setValue(c->timePhaseValue);
        }
        spinBox_duration->setValue(c->timeDuration);
    } else {
        checkBox_notTime->setChecked(false);
        comboBox_period->setCurrentIndex(0);
        checkBox_phaseVariable->setChecked(false);
        checkBox_phaseValue->setChecked(true);
        comboBox_phaseVariable->setEnabled(false);
        spinBox_phaseValue->setEnabled(0);
        comboBox_phaseVariable->setCurrentIndex(0);
        spinBox_duration->setValue(0);
    }

    /* If condition uses nested conditions */
    if (c->isConditions) {
        if (c->enabled) {
            groupBox_value->setEnabled(false);
            groupBox_time->setEnabled(false);
            groupBox_nested->setEnabled(true);
        }

        radioButton_nested->setChecked(true);
        checkBox_notNested->setChecked(c->isNot);

        lineEdit_lhsCondition->setText(c->lhsCondition->toString());
        for (int i = 0; i < comboBox_nestedOp->count(); i++) {
            if (QString::compare(comboBox_nestedOp->itemText(i), c->op) == 0)
                comboBox_nestedOp->setCurrentIndex(i);
        }
        lineEdit_rhsCondition->setText(c->rhsCondition->toString());
    } else {
        checkBox_notNested->setChecked(false);
        lineEdit_lhsCondition->setText("");
        comboBox_nestedOp->setCurrentIndex(0);
        lineEdit_rhsCondition->setText("");
    }
}

/*!
 * \brief Sets local condition and calls interface to be updated
 * \param[in] c The condition
 *
 * Sets local condition and calls interface to be updated.
 */
void MpreDialog::setCondition(Condition c) {
    condition = c;

    setCurrentCondition(&condition);
}

/*!
 * \brief Captures interface settings and saves to local condition
 *
 * Captures interface settings and saves to local condition.
 */
void MpreDialog::getCurrentCondition() {
    c->enabled = checkBox_enabled->isChecked();

    c->isValues = radioButton_value->isChecked();
    c->isTime   = radioButton_time->isChecked();
    c->isConditions = radioButton_nested->isChecked();

    if (c->isValues) {
        c->isNot = checkBox_notValue->isChecked();
        c->op = comboBox_valueOp->currentText();
    }
    if (c->isTime) c->isNot = checkBox_notTime->isChecked();
    if (c->isConditions) {
        c->isNot = checkBox_notNested->isChecked();
        c->op = comboBox_nestedOp->currentText();
    }

    c->lhsIsAgentVariable = checkBox_lhsAgentVariable->isChecked();
    c->lhsIsMessageVariable =
            checkBox_lhsMessageVariable->isChecked();
    c->lhsIsValue = checkBox_lhsValue->isChecked();
    if (c->lhsIsAgentVariable) c->lhs =
            comboBox_lhsAgentVariable->currentText();
    if (c->lhsIsMessageVariable) c->lhs =
            comboBox_lhsMessageVariable->currentText();
    c->lhsDouble = doubleSpinBox_lhsValue->value();

    c->rhsIsAgentVariable = checkBox_rhsAgentVariable->isChecked();
    c->rhsIsMessageVariable = checkBox_rhsMessageVariable->isChecked();
    c->rhsIsValue = checkBox_rhsValue->isChecked();
    if (c->rhsIsAgentVariable) c->rhs =
            comboBox_rhsAgentVariable->currentText();
    if (c->rhsIsMessageVariable) c->rhs =
            comboBox_rhsMessageVariable->currentText();
    c->rhsDouble = doubleSpinBox_rhsValue->value();

    c->timePeriod = comboBox_period->currentText();
    c->timePhaseIsVariable = checkBox_phaseVariable->isChecked();
    c->timePhaseVariable = comboBox_phaseVariable->currentText();
    c->timePhaseValue = spinBox_phaseValue->value();
    c->timeDuration = spinBox_duration->value();
}

/*!
 * \brief Returns local condition
 * \return The condition
 *
 * Calls for capture of condition settings and returns the local condition.
 */
Condition MpreDialog::getCondition() {
    getCurrentCondition();

    return condition;
}

/*!
 * \brief Enables and disabled the condition interface
 * \param[in] e The boolean
 *
 * Enables and disabled the condition interface.
 */
void MpreDialog::enableCondition(bool e) {
    radioButton_value->setEnabled(e);
    radioButton_time->setEnabled(e);
    radioButton_nested->setEnabled(e);

    groupBox_value->setEnabled(e);
    groupBox_time->setEnabled(e);
    groupBox_nested->setEnabled(e);

    if (e) {
        if (c->isValues) {
            groupBox_time->setEnabled(false);
            groupBox_nested->setEnabled(false);
        }
        if (c->isTime) {
            groupBox_value->setEnabled(false);
            groupBox_nested->setEnabled(false);
        }
        if (c->isConditions) {
            groupBox_value->setEnabled(false);
            groupBox_time->setEnabled(false);
        }
    }
}

/*!
 * \brief Enables value group box and disables time and nested
 *
 * Enables value group box and disables time and nested.
 */
void MpreDialog::valueClicked() {
    groupBox_value->setEnabled(true);
    groupBox_time->setEnabled(false);
    groupBox_nested->setEnabled(false);
}

/*!
 * \brief Enables time group box and disables value and nested
 *
 * Enables time group box and disables value and nested.
 */
void MpreDialog::timeClicked() {
    groupBox_value->setEnabled(false);
    groupBox_time->setEnabled(true);
    groupBox_nested->setEnabled(false);
}

/*!
 * \brief Enables nested group box and disables time and value
 *
 * Enables nested group box and disables time and value.
 */
void MpreDialog::nestedClicked() {
    groupBox_value->setEnabled(false);
    groupBox_time->setEnabled(false);
    groupBox_nested->setEnabled(true);
}

void MpreDialog::checkedLhsAgentVariable(bool b) {
    if (checkBox_lhsMessageVariable->isEnabled())
        checkBox_lhsMessageVariable->setChecked(!b);
    checkBox_lhsValue->setChecked(!b);

    comboBox_lhsAgentVariable->setEnabled(b);
    comboBox_lhsMessageVariable->setEnabled(!b);
    doubleSpinBox_lhsValue->setEnabled(!b);
}

void MpreDialog::checkedLhsMessageVariable(bool b) {
    checkBox_lhsAgentVariable->setChecked(!b);
    checkBox_lhsValue->setChecked(!b);

    comboBox_lhsAgentVariable->setEnabled(!b);
    comboBox_lhsMessageVariable->setEnabled(b);
    doubleSpinBox_lhsValue->setEnabled(!b);
}

void MpreDialog::checkedLhsValue(bool b) {
    checkBox_lhsAgentVariable->setChecked(!b);
    if (checkBox_lhsMessageVariable->isEnabled())
        checkBox_lhsMessageVariable->setChecked(!b);

    comboBox_lhsAgentVariable->setEnabled(!b);
    comboBox_lhsMessageVariable->setEnabled(!b);
    doubleSpinBox_lhsValue->setEnabled(b);
}

void MpreDialog::checkedRhsAgentVariable(bool b) {
    if (checkBox_rhsMessageVariable->isEnabled())
        checkBox_rhsMessageVariable->setChecked(!b);
    checkBox_rhsValue->setChecked(!b);

    comboBox_rhsAgentVariable->setEnabled(b);
    comboBox_rhsMessageVariable->setEnabled(!b);
    doubleSpinBox_rhsValue->setEnabled(!b);
}

void MpreDialog::checkedRhsMessageVariable(bool b) {
    checkBox_rhsAgentVariable->setChecked(!b);
    checkBox_rhsValue->setChecked(!b);

    comboBox_rhsAgentVariable->setEnabled(!b);
    comboBox_rhsMessageVariable->setEnabled(b);
    doubleSpinBox_rhsValue->setEnabled(!b);
}

void MpreDialog::checkedRhsValue(bool b) {
    checkBox_rhsAgentVariable->setChecked(!b);
    if (checkBox_rhsMessageVariable->isEnabled())
        checkBox_rhsMessageVariable->setChecked(!b);

    comboBox_rhsAgentVariable->setEnabled(!b);
    comboBox_rhsMessageVariable->setEnabled(!b);
    doubleSpinBox_rhsValue->setEnabled(b);
}

void MpreDialog::disableTimePhaseVariable(bool b) {
    checkBox_phaseVariable->setChecked(!b);
    comboBox_phaseVariable->setEnabled(!b);
    spinBox_phaseValue->setEnabled(b);
}

void MpreDialog::disableTimePhaseValue(bool b) {
    checkBox_phaseValue->setEnabled(!b);
    comboBox_phaseVariable->setEnabled(b);
    spinBox_phaseValue->setEnabled(!b);
}

/*!
 * \brief Creates a lhs condition and sets it as the local condition
 *
 * Captures interface settings to local condition.
 * Creates a lhs condition (if it doesn't exist) and sets it as the local condition.
 */
void MpreDialog::editLhsClicked() {
    getCurrentCondition();
    if (c->lhsCondition == 0) {
        c->lhsCondition = new Condition();
        c->lhsCondition->enabled = true;
        c->lhsCondition->isValues = true;
    }
    c->lhsCondition->parentCondition = c;
    setCurrentCondition(c->lhsCondition);
}

/*!
 * \brief Creates a rhs condition and sets it as the local condition
 *
 * Captures interface settings to local condition.
 * Creates a rhs condition (if it doesn't exist) and sets it as the local condition.
 */
void MpreDialog::editRhsClicked() {
    getCurrentCondition();
    if (c->rhsCondition == 0) {
        c->rhsCondition = new Condition();
        c->lhsCondition->enabled = true;
        c->lhsCondition->isValues = true;
    }
    c->rhsCondition->parentCondition = c;
    setCurrentCondition(c->rhsCondition);
}

void MpreDialog::levelUpClicked() {
    getCurrentCondition();
    setCurrentCondition(c->parentCondition);
}
