#include <QtGui>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "mpostdialog.h"

MpostDialog::MpostDialog(MemoryModel * m, QWidget *parent)
     : QDialog(parent)
{
    //qDebug() << "MpreDialog";
    memory = m;

    lhsGroup = new QGroupBox(tr("LHS"));
    rhsGroup = new QGroupBox(tr("RHS"));
    opGroup = new QGroupBox(tr("OP"));

    myEnabled = new QCheckBox(tr("Enabled"));

    connect(myEnabled, SIGNAL(clicked(bool)), lhsGroup, SLOT(setEnabled(bool)));
    connect(myEnabled, SIGNAL(clicked(bool)), rhsGroup, SLOT(setEnabled(bool)));
    connect(myEnabled, SIGNAL(clicked(bool)), opGroup, SLOT(setEnabled(bool)));

    variable = new QComboBox();
    variable2 = new QComboBox();
    value = new QSpinBox();
    op = new QComboBox();
    value2 = new QSpinBox();

    operators << "=" << "+=" << "-=" << "*=" << "/=";
    op->addItems(operators);
    variable->addItems(memory->getNames());
    variable2->addItems(memory->getNames());

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);

    connect(this, SIGNAL(setVariableComboBox(int)), variable, SLOT(setCurrentIndex(int)));
    connect(this, SIGNAL(setVariable2ComboBox(int)), variable2, SLOT(setCurrentIndex(int)));
    connect(this, SIGNAL(setOpComboBox(int)), op, SLOT(setCurrentIndex(int)));

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *lhsLayout = new QVBoxLayout;
    lhsLayout->addWidget(variable);
    lhsGroup->setLayout(lhsLayout);

    QVBoxLayout *rhsLayout = new QVBoxLayout;
    //rhsLayout->addWidget(value);
    rhsLayout->addWidget(variable2);
    rhsGroup->setLayout(rhsLayout);

    QHBoxLayout *opLayout = new QHBoxLayout;
    opLayout->addWidget(op);
    opGroup->setLayout(opLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(myEnabled);
    mainLayout->addWidget(lhsGroup);
    mainLayout->addWidget(opGroup);
    mainLayout->addWidget(rhsGroup);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Mpost Editor"));
}

void MpostDialog::setMpost(Mpost m)
{
    mpost = m;
    myEnabled->setChecked(mpost.enabled());
    lhsGroup->setEnabled(mpost.enabled());
    opGroup->setEnabled(mpost.enabled());
    rhsGroup->setEnabled(mpost.enabled());
    value->setValue(mpost.value());
    //value2->setValue(mpost.value2());
    int index = memory->getNames().indexOf(mpost.name());
    if(index == -1) index = 0;
    emit ( setVariableComboBox(index) );

    index = operators.indexOf(mpost.op());
    if(index == -1) index = 0;
    emit ( setOpComboBox(index) );

    index = memory->getNames().indexOf(mpost.name2());
    if(index == -1) index = 0;
    emit ( setVariable2ComboBox(index) );
}

Mpost MpostDialog::getMpost()
{
    mpost.setEnabled(myEnabled->isChecked());
    mpost.setValue(value->value());
    mpost.setName(variable->currentText());
    mpost.setOp(op->currentText());
    //mpost.setValue2(value2->value());
    mpost.setName2(variable2->currentText());
    return mpost;
}
