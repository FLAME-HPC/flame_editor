/*!
 * \file sortdialog.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of message sort dialog
 */
#include "./sortdialog.h"
#include "./messagesort.h"

SortDialog::SortDialog(Machine * m, QString * mt, QWidget *parent)
    : QDialog(parent) {
    setupUi(this);

    machine = m;

    QStringList messageMemoryNames = machine->getMessageMemoryNames(*mt);
    comboBox_key->addItems(messageMemoryNames);
    QStringList sorts;
    sorts << "ascend" << "descend";
    comboBox_order->addItems(sorts);

    connect(this->checkBox_notRandomOrSort, SIGNAL(clicked()),
            this, SLOT(clickedNotRandomOrSort()));
    connect(this->checkBox_random, SIGNAL(clicked()),
            this, SLOT(clickedRandom()));
    connect(this->checkBox_sort, SIGNAL(clicked()), this, SLOT(clickedSort()));
}

void SortDialog::setSort(MessageSort s) {
    sort = s;

    if (s.isRandom) {
        checkBox_notRandomOrSort->setChecked(false);
        checkBox_random->setChecked(true);
        checkBox_sort->setChecked(false);
        groupBox_sort->setEnabled(false);
    } else if (s.isSort) {
        checkBox_notRandomOrSort->setChecked(false);
        checkBox_random->setChecked(false);
        checkBox_sort->setChecked(true);
        groupBox_sort->setEnabled(true);
        comboBox_key->setCurrentIndex(0);
        for (int i = 0; i < comboBox_key->count(); i++) {
            if (QString::compare(comboBox_key->itemText(i), s.key) == 0)
                comboBox_key->setCurrentIndex(i);
        }
        comboBox_order->setCurrentIndex(0);
        for (int i = 0; i < comboBox_order->count(); i++) {
            if (QString::compare(comboBox_order->itemText(i), s.order) == 0)
                comboBox_order->setCurrentIndex(i);
        }
    } else {
        checkBox_notRandomOrSort->setChecked(true);
        checkBox_random->setChecked(false);
        checkBox_sort->setChecked(false);
        groupBox_sort->setEnabled(false);
    }
}

MessageSort SortDialog::getSort() {
    sort.isRandom = checkBox_random->isChecked();
    sort.isSort = checkBox_sort->isChecked();
    sort.key = comboBox_key->currentText();
    sort.order = comboBox_order->currentText();

    return sort;
}

void SortDialog::clickedNotRandomOrSort() {
    checkBox_notRandomOrSort->setChecked(true);
    checkBox_random->setChecked(false);
    checkBox_sort->setChecked(false);
    groupBox_sort->setEnabled(false);
}

void SortDialog::clickedRandom() {
    checkBox_notRandomOrSort->setChecked(false);
    checkBox_random->setChecked(true);
    checkBox_sort->setChecked(false);
    groupBox_sort->setEnabled(false);
}

void SortDialog::clickedSort() {
    checkBox_notRandomOrSort->setChecked(false);
    checkBox_random->setChecked(false);
    checkBox_sort->setChecked(true);
    groupBox_sort->setEnabled(true);
}
