/*!
 * \file timeunitdialog.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of time unit dialog
 */
#include "./timeunitdialog.h"
#include "./ui_timeunitdialog.h"
#include "./machine.h"

TimeUnitDialog::TimeUnitDialog(Machine * m, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::TimeUnitDialog) {
    ui->setupUi(this);

    machine = m;

    ui->comboBox_unit->addItems(machine->getTimeUnits());
}

TimeUnitDialog::~TimeUnitDialog() {
    delete ui;
}

void TimeUnitDialog::setTimeUnit(TimeUnit t) {
    timeUnit = t;

    ui->lineEdit_name->setText(t.name);
    ui->comboBox_unit->setCurrentIndex(0);
    for (int i = 0; i < ui->comboBox_unit->count(); i++) {
        if (ui->comboBox_unit->itemText(i) == t.unit)
            ui->comboBox_unit->setCurrentIndex(i);
    }
    ui->spinBox_period->setValue(t.period);
}

TimeUnit TimeUnitDialog::getTimeUnit() {
    timeUnit.name = ui->lineEdit_name->text();
    timeUnit.unit = ui->comboBox_unit->currentText();
    timeUnit.period = ui->spinBox_period->value();

    return timeUnit;
}
