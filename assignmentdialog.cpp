#include "assignmentdialog.h"
#include <QRegExp>
#include <QRegExpValidator>

AssignmentDialog::AssignmentDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    setUI();

    setWindowTitle("Statement Edit");

    QRegExp re("\\d{1,2}(\\,\\d{1,2})?");
    QRegExpValidator *v = new QRegExpValidator(re, comboBox1);

    comboBox1->addItem("uiu");
    comboBox1->addItem("uiu");
    comboBox1->setCurrentIndex(-1);
    comboBox1->setEditable(true);
    comboBox1->setValidator(v);

}

void AssignmentDialog::setUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    QHBoxLayout *assignmentLayout = new QHBoxLayout();
    lAssignment = new QLabel("Statement");
    lAssignment->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    lAssignment->setMinimumSize(550, 10);
    assignmentLayout->addWidget(lAssignment, 0, Qt::AlignTop);

    QVBoxLayout *buttonsLayou = new QVBoxLayout();

    pbOk = new QPushButton("Ok");
    pbOk->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pbOk->setMinimumSize(50, 10);
    buttonsLayou->addWidget(pbOk);

    pbCancel = new QPushButton("Cancel");
    pbCancel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pbCancel->setMinimumSize(50, 10);
    buttonsLayou->addWidget(pbCancel);

    QHBoxLayout *c = new QHBoxLayout();

    c->addLayout(assignmentLayout);
    c->addLayout(buttonsLayou);

    mainLayout->addLayout(c);

    QHBoxLayout *blocksLayout = new QHBoxLayout();
    blocksLayout->setSpacing(1);
    blocksLayout->setMargin(1);

    QVBoxLayout *blockLayout1 = new QVBoxLayout();

    label1 = new QLabel(qsIdentifier);
    comboBox1 = new QComboBox();
    comboBox1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    comboBox1->setMinimumSize(50, 10);

    blockLayout1->addWidget(label1, 0, Qt::AlignLeft);
    blockLayout1->addWidget(comboBox1, 0, Qt::AlignLeft);

    QVBoxLayout *blockLayout2 = new QVBoxLayout();

    label2 = new QLabel(qsOperation);
    comboBox2 = new QComboBox();
    comboBox2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    comboBox2->setMinimumSize(50, 10);

    blockLayout2->addWidget(label2, 0, Qt::AlignLeft);
    blockLayout2->addWidget(comboBox2, 0, Qt::AlignLeft);

    QVBoxLayout *blockLayout3 = new QVBoxLayout();

    label3 = new QLabel(qsOperator);
    comboBox3 = new QComboBox();
    comboBox3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    comboBox3->setMinimumSize(50, 10);

    blockLayout3->addWidget(label3, 0, Qt::AlignLeft);
    blockLayout3->addWidget(comboBox3, 0, Qt::AlignLeft);

    blocksLayout->addLayout(blockLayout1);
    blocksLayout->addLayout(blockLayout2);
    blocksLayout->addLayout(blockLayout3);

    mainLayout->addLayout(blocksLayout);
    mainLayout->addSpacing(20);

    QHBoxLayout *h = new QHBoxLayout();

    pbAdd = new QPushButton("Add");
    pbAdd->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pbAdd->setMinimumSize(50, 10);
    h->addWidget(pbAdd, 0, Qt::AlignLeft);
    h->addSpacing(10);

    pbRemove = new QPushButton("Remove");
    pbRemove->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pbRemove->setMinimumSize(50, 10);
    h->addWidget(pbRemove, 0, Qt::AlignLeft);

    mainLayout->addLayout(h);

}

AssignmentDialog::~AssignmentDialog()
{
    delete lAssignment;
    delete pbOk;
    delete pbCancel;
    delete label1;
    delete comboBox1;
    delete label2;
    delete comboBox2;
    delete label3;
    delete comboBox3;
    delete pbAdd;
    delete pbRemove;
}
