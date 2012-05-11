#include "codedialog.h"
#include "./assignmentdialog.h"
#include <QDebug>

CodeDialog::CodeDialog(QWidget *parent) :
    QDialog(parent)
{

    Qt::WindowFlags f = 0;
    f |= Qt::WindowMaximizeButtonHint;

    setWindowFlags(f);

    setUI();


    setShowList(false);
/*
    listWidget->addItem("hh 1");
    listWidget->addItem("hh 2");
    pbRemove->setDisabled(true);
*/
    connect(listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(selectListItem()));

    connect(pbAdd, SIGNAL(clicked()), this, SLOT(add_click()));

    connect(pbRemove, SIGNAL(clicked()), this, SLOT(removeItemList_click()));

}

CodeDialog::~CodeDialog()
{
    delete machineScene;
}

void CodeDialog::setUI()
{
    mainLayout = new QVBoxLayout();
    mainContentLayout = new QVBoxLayout();

    graphicsView = new QGraphicsView(this);
    mainContentLayout->addWidget(graphicsView);
    mainLayout->addLayout(mainContentLayout);
    setLayout(mainLayout);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    /* Create the machine scene object */
    machineScene = new MachineScene(this);
    /* Set the graphics view to use the machine scene object */
    graphicsView->setScene(machineScene);

    listLayout = new QVBoxLayout();
    label = new QLabel("Assignments");
    listLayout->addWidget(label);

    listWidget = new QListWidget();
    listWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    listWidget->setMinimumSize(50, 10);
    listLayout->addWidget(listWidget);

    QHBoxLayout *h = new QHBoxLayout();

    pbAdd = new QPushButton("Add");
    pbAdd->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pbAdd->setMinimumSize(50, 25);
    h->addWidget(pbAdd);
    pbRemove = new QPushButton("Remove");
    pbRemove->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pbRemove->setMinimumSize(50, 25);
    h->addWidget(pbRemove, 0, Qt::AlignLeft);
    listLayout->addLayout(h);
    mainLayout->addLayout(listLayout);
}

void CodeDialog::selectListItem()
{
    qDebug()<<"ok";
    bool b = false;
    for(int i = 0;i < listWidget->count() && !b;i++){
        b = listWidget->item(i)->isSelected();
    }
    pbRemove->setDisabled(!b);
}

void CodeDialog::add_click()
{
    AssignmentDialog a(this);
    a.exec();
}

void CodeDialog::removeItemList_click()
{
    QListWidgetItem *it;
    bool b = false;
    for(int i = 0;i < listWidget->count() && !b;i++){
        b = listWidget->item(i)->isSelected();
        if(b)
            it = listWidget->item(i);
    }
    if(it != 0){
        delete it;
        listWidget->setCurrentRow(-1);
    }
}
