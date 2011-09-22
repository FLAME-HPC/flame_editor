#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stateitem.h"
#include "memorydelegate.h"
#include "mpredelegate.h"
#include "mpostdelegate.h"
#include "statedelegate.h"
#include "modelxmlreader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);

    currentMachine = 0;
    noMachines = 0;
    enableUI(false);
    modelPath = ".";
    modelName = "model";

    QAction * newAction = new QAction(tr("&New"), this);
    //newAction->setIcon(":/images/new.png");
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new machine"));

    memory = new MemoryModel();
    machine = new MachineModel(memory);
    //machine->memory() = new MemoryModel();
    //modelTree = new ModelTree();
    //ui->treeView->setModel(modelTree);

    /* Set tables to stretch columns to table size */
    QHeaderView *headerView = ui->tableViewMachine->horizontalHeader();
    headerView->setResizeMode(QHeaderView::Stretch);
    headerView->setResizeMode(1, QHeaderView::Interactive);
    ui->tableViewMachine->verticalHeader()->hide();
    headerView = ui->tableViewMemory->horizontalHeader();
    headerView->setResizeMode(QHeaderView::Stretch);
    headerView->setResizeMode(1, QHeaderView::Interactive);
    ui->tableViewMemory->verticalHeader()->hide();

    ui->treeWidget->header()->setResizeMode(QHeaderView::Stretch);
    ui->treeWidget->header()->setResizeMode(1, QHeaderView::Interactive);

    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonStop->setEnabled(false);

    scene = new MachineScene(machine);
    scene->setSceneRect(0, 0, 500, 500);

    simulationThread = new SimulationThread();

    connect(ui->pushButtonNew, SIGNAL(clicked()), this, SLOT(newModel()));
    connect(ui->pushButtonOpen, SIGNAL(clicked()), this, SLOT(openModel()));
    connect(ui->pushButtonSave, SIGNAL(clicked()), this, SLOT(saveModel()));

    //connect(this->machine, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(transitionChanged(QModelIndex,QModelIndex)));

    connect(ui->pushButtonStart, SIGNAL(clicked()), this, SLOT(startModel()));
    //connect(ui->pushButtonStart, SIGNAL(clicked()), simulationThread, SLOT(startSim()));

    connect(ui->pushButtonPause, SIGNAL(clicked()), this, SLOT(pauseModel()));
    connect(ui->pushButtonPause, SIGNAL(clicked()), simulationThread, SLOT(pauseSim()));

    connect(ui->pushButtonStop, SIGNAL(clicked()), this, SLOT(stopModel()));
    connect(ui->pushButtonStop, SIGNAL(clicked()), simulationThread, SLOT(stopSim()));

    connect(this, SIGNAL(clearSceneSelection()), scene, SLOT(clearSelection()));
    connect(this, SIGNAL(updateScene()), scene, SLOT(update()));
    connect(simulationThread, SIGNAL(updateScene()), scene, SLOT(update()));

    connect(simulationThread, SIGNAL(finished()), this, SLOT(stopModel()));

    //connect(simulationThread, SIGNAL(selectTransition(Arrow*)), this, SLOT(selectTransition(Arrow*)));
    connect(simulationThread, SIGNAL(selectTransition(QString)), this, SLOT(selectTransition(QString)));
    connect(simulationThread, SIGNAL(selectState(QString)), this, SLOT(selectState(QString)));

    connect(scene, SIGNAL(updateStatus(QString)), this, SLOT(updateStatusLabel(QString)));

    connect(ui->pushButtonAddMemory, SIGNAL(clicked()), this, SLOT(insertMemory()));
    connect(ui->pushButtonDeleteMemory, SIGNAL(clicked()), this, SLOT(deleteMemory()));

    connect(ui->action_New, SIGNAL(triggered()), this, SLOT(newModel()));
    connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(openModel()));
    connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(saveModel()));

    //connect(machine, SIGNAL(newState(StateItem*)), scene, SLOT(addState(StateItem*)));
    //connect(machine, SIGNAL(newTransition(Arrow*)), scene, SLOT(addTransition(Arrow*)));

    connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(changeModel()));

    /*connect(scene, SIGNAL(addedState(QString,bool)), machine, SLOT(addState(QString,bool)));
    connect(scene, SIGNAL(addedTransition(QString,QString,QString)), machine, SLOT(addTransition(QString,QString,QString)));
    connect(machine, SIGNAL(updateStateName(QString,QString)), scene, SLOT(updateStateName(QString,QString)));*/



    //ui->tableWidget_2->insertRow(0);
    //ui->tableWidget_2->setItem(0,0,new QTableWidgetItem("int"));

    //ui->treeWidget->addTopLevelItem(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Firm"))));

    ui->tableViewMachine->setModel(machine);
    ui->tableViewMemory->setModel(memory);

    //MemoryDelegate memoryDelegate;
    ui->tableViewMemory->setItemDelegate(new MemoryDelegate);
    ui->tableViewMachine->setItemDelegateForColumn(0, new StateDelegate(memory));
    ui->tableViewMachine->setItemDelegateForColumn(2, new MpreDelegate(memory));
    ui->tableViewMachine->setItemDelegateForColumn(3, new StateDelegate(memory));
    ui->tableViewMachine->setItemDelegateForColumn(4, new MpostDelegate(memory));
    ui->tableViewMachine->setItemDelegateForColumn(6, new StateDelegate(memory));

    //qDebug() << "main window: " << this;

    //machine->memory()->insertRows(0, 3);

    //QGraphicsScene *scene = new QGraphicsScene();

    //scene->addText("SIMON!");

    ui->label->setText("New model");

    //StateItem *state = new StateItem;
    //scene->addItem(state);



    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();


}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::transitionChanged(QModelIndex topLeft, QModelIndex)
{
    //ui->label->setText(QString("transitionChanged r: %1 c: %2").arg(topLeft.row(), topLeft.column()));

    emit( updateScene() );

    // If mpre was updated
    if(topLeft.column() == 2)
    {
        //qDebug() << "mpre updated";

        // If another branch from the same state exists
        Arrow * a = machine->getTransitions().at(topLeft.row());
        for(int i = 0; i < machine->getTransitions().size(); i++)
        {
            Arrow * ab = machine->getTransitions().at(i);
            if(a->startItem() == ab->startItem() && i != topLeft.row())
            {
                qDebug() << "Found other branch: " << ab;
                ab->getMprePointer()->setNot(true);
                ab->getMprePointer()->setEnabled(a->getMpre().enabled());
                ab->getMprePointer()->setName(a->getMpre().name());
                ab->getMprePointer()->setOp(a->getMpre().op());
                ab->getMprePointer()->setValue(a->getMpre().value());
            }
        }
    }
}*/

void MainWindow::startModel()
{
    // Check for a valid start state
    int flag = machine->checkValidStartState();
    if(flag == 0)
    {
        emit( clearSceneSelection() );
        //if(machine->getStartState() != 0) machine->getStartState()->setSelected(true);

        ui->pushButtonPause->setEnabled(true);
        ui->pushButtonStop->setEnabled(true);
        /*ui->treeWidget->setEnabled(false);
        ui->graphicsView->setEnabled(false);
        ui->tableViewMachine->setEnabled(false);
        ui->tableViewMemory->setEnabled(false);*/

        simulationThread->startSim();
        ui->label->setText("Model started");
    }
    else if(flag == 1) ui->label->setText("Error: More than one start state possible");
    else if(flag == 2) ui->label->setText("Error: No start states found");
}

void MainWindow::pauseModel()
{
    ui->label->setText("Model paused");
}

void MainWindow::stopModel()
{
    ui->label->setText("Model stopped");

    //emit( clearSceneSelection() );

    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonStop->setEnabled(false);
    /*ui->treeWidget->setEnabled(true);
    ui->graphicsView->setEnabled(true);
    ui->tableViewMachine->setEnabled(true);
    ui->tableViewMemory->setEnabled(true);*/
}

void MainWindow::selectTransition(QString n)
{
    //qDebug() << "Transition: " << a;
    for(int i = 0; i < machine->getTransitions().size(); i++)
    {
        if(machine->getTransitions().at(i)->name() == n) ui->tableViewMachine->selectRow(i);
    }
    scene->selectTransition(n);
}

void MainWindow::selectState(QString n)
{
    scene->selectState(n);
}

void MainWindow::updateStatusLabel(QString s)
{
    ui->label->setText(s);
}

void MainWindow::insertMemory()
{
    memory->insertRow(memory->rowCount());
}

void MainWindow::deleteMemory()
{
    QModelIndex index = ui->tableViewMemory->currentIndex();

    memory->removeRow(index.row());

    //qDebug() << "row: " << index.row();
    //ui->statusBar->showMessage("Hello Simon");
}

void MainWindow::newModel()
{
    //ui->statusBar->showMessage("new Model");
    QString name = "machine_";
    name.append(QString().number(machines.size()));

    //ui->treeWidget->addTopLevelItem(new QTreeWidgetItem((QTreeWidget*)0, QStringList(name)));
    //modelTree->insertRows(0, 1);

    Machine * m = new Machine();
    m->setText(0, name);
    machines.insert(0, m);
    ui->treeWidget->addTopLevelItem(m);
    ui->treeWidget->setCurrentItem(m);

    changeModel();

    enableUI(true);
}

void MainWindow::openModel()
{
    QString modelFile;
    modelFile.append(modelPath);
    modelFile.append(modelName);
    modelFile.append(".xml");

    QString fileName =
             QFileDialog::getOpenFileName(this, tr("Open model"),
                                          modelFile,
                                          tr("XML Files (*.xml)"));
     if (fileName.isEmpty())
         return;

     //treeWidget->clear();

     QFile file(fileName);
     if (!file.open(QFile::ReadOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("x-editor"),
                              tr("Cannot read file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return;
     }

     //ui->treeWidget->addTopLevelItem(new QTreeWidgetItem((QTreeWidget*)0, QStringList(name)));

     Machine * m = new Machine();
     modelXMLReader reader(m);
     if (!reader.read(&file))
     {
         QMessageBox::warning(this, tr("x-editor"),
                              tr("Parse error in file %1 at line %2, column %3:\n%4").arg(fileName).arg(reader.lineNumber()).arg(reader.columnNumber()).arg(reader.errorString()));
     }
     else
     {
         machines.insert(0, m);
        ui->treeWidget->addTopLevelItem(m);
        ui->treeWidget->setCurrentItem(m);

        changeModel();

         enableUI(true);
         statusBar()->showMessage(tr("File loaded"), 2000);
     }
}

void MainWindow::saveModel()
{
    QString modelFile;
    modelFile.append(modelPath);
    modelFile.append(modelName);
    modelFile.append(".xml");
    
    QString fileName =
             QFileDialog::getSaveFileName(this, tr("Save Model File"),
                                          //QDir::currentPath(),
                                          modelFile,
                                          tr("XML Files (*.xml)"));
     if (fileName.isEmpty())
         return;

     QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("AgentChum"),
                              tr("Cannot write file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return;
     }

     if (currentMachine->writeModelXML(&file))
     {
         statusBar()->showMessage(tr("File saved"), 2000);
     }
}

void MainWindow::enableUI(bool enable)
{
    ui->treeWidget->setEnabled(enable);
    ui->graphicsView->setEnabled(enable);
    ui->tableViewMemory->setEnabled(enable);
    ui->tableViewMachine->setEnabled(enable);

    ui->pushButtonSave->setEnabled(enable);
    ui->pushButtonAddMemory->setEnabled(enable);
    ui->pushButtonDeleteMemory->setEnabled(enable);
    ui->pushButtonSceneDelete->setEnabled(enable);
    ui->pushButtonStart->setEnabled(enable);
    ui->pushButtonPause->setEnabled(enable);
    ui->pushButtonStop->setEnabled(enable);
}

void MainWindow::changeModel()
{
    Machine * m = static_cast<Machine*>(ui->treeWidget->selectedItems().at(0));

    Q_ASSERT(m != 0);

    currentMachine = m;
    machine = m->machineModel;
    memory = m->memoryModel;
    scene = m->machineScene;
    ui->tableViewMachine->setItemDelegateForColumn(2, new MpreDelegate(m->memoryModel));
    ui->tableViewMachine->setItemDelegateForColumn(4, new MpostDelegate(m->memoryModel));
    ui->tableViewMachine->setModel(m->machineModel);
    ui->tableViewMachine->update();
    ui->tableViewMemory->setModel(m->memoryModel);
    ui->tableViewMemory->update();
    ui->graphicsView->setScene(m->machineScene);
    ui->graphicsView->show();
    simulationThread->setMachine(m);

    //qDebug() << "machine selected: " << m->text(0);
}
