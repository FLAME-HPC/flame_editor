#include <QtGui>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "mpredialog.h"

MpreDialog::MpreDialog(MemoryModel * m, QWidget *parent)
     : QDialog(parent)
{
    //qDebug() << "MpreDialog";
    memory = m;

    lhsGroup = new QGroupBox(tr("LHS"));
    opGroup = new QGroupBox(tr("OP"));
    rhsGroup = new QGroupBox(tr("RHS"));

    myEnabled = new QCheckBox(tr("Enabled"));

    connect(myEnabled, SIGNAL(clicked(bool)), lhsGroup, SLOT(setEnabled(bool)));
    connect(myEnabled, SIGNAL(clicked(bool)), opGroup, SLOT(setEnabled(bool)));
    connect(myEnabled, SIGNAL(clicked(bool)), rhsGroup, SLOT(setEnabled(bool)));

    variable = new QComboBox();
    op = new QComboBox();
    value = new QSpinBox();

    operators << "==" << "!=" << ">" << "<" << ">=" << "<=";
    op->addItems(operators);
    variable->addItems(memory->getNames());

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);

    connect(this, SIGNAL(setVariableComboBox(int)), variable, SLOT(setCurrentIndex(int)));
    connect(this, SIGNAL(setOpComboBox(int)), op, SLOT(setCurrentIndex(int)));

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *lhsLayout = new QVBoxLayout;
    lhsLayout->addWidget(variable);
    lhsGroup->setLayout(lhsLayout);

    QVBoxLayout *opLayout = new QVBoxLayout;
    opLayout->addWidget(op);
    opGroup->setLayout(opLayout);

    QVBoxLayout *rhsLayout = new QVBoxLayout;
    rhsLayout->addWidget(value);
    rhsGroup->setLayout(rhsLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(myEnabled);
    mainLayout->addWidget(lhsGroup);
    mainLayout->addWidget(opGroup);
    mainLayout->addWidget(rhsGroup);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Mpre Editor"));
}

void MpreDialog::setMpre(Mpre m)
{
    mpre = m;
    myEnabled->setChecked(mpre.enabled());
    lhsGroup->setEnabled(mpre.enabled());
    opGroup->setEnabled(mpre.enabled());
    rhsGroup->setEnabled(mpre.enabled());
    value->setValue(mpre.value());
    int index = memory->getNames().indexOf(mpre.name());
    if(index == -1) index = 0;
    emit ( setVariableComboBox(index) );
    emit ( setOpComboBox( operators.indexOf(mpre.op()) ) );
}

Mpre MpreDialog::getMpre()
{
    mpre.setEnabled(myEnabled->isChecked());
    mpre.setValue(value->value());
    mpre.setOp(op->currentText());
    mpre.setName(variable->currentText());
    mpre.setNot(false);
    return mpre;
}
