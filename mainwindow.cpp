/*!
 * \file mainwindow.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of the main window
 */
#include "./mainwindow.h"
#include "./ui_mainwindow.h"
#include "./graphicsitem.h"
#include "./memorydelegate.h"
#include "./mpredelegate.h"
#include "./mpostdelegate.h"
#include "./statedelegate.h"
#include "./commdelegate.h"
#include "./datatypedelegate.h"
#include "./lineeditdelegate.h"
#include "./texteditdelegate.h"
#include "./machinetreedelegate.h"
#include "./modelxmlreader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass) {
    ui->setupUi(this);

    /* Set the application icon (for linux) as mac and win
     * have platform-dependent techniques (see .pro file).
     */
    #ifdef Q_WS_X11
    setWindowIcon(QIcon("flame-e.png"));
    #endif

    currentMachine = 0;

    QAction * newAction = new QAction(tr("&New"), this);
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new machine"));

    /* Machine tree */
    machineTree = new MachineTree();
    ui->treeView_machines->setModel(machineTree);
    ui->treeView_machines->header()->setStretchLastSection(true);
    ui->treeView_machines->setItemDelegate(new MachineTreeDelegate());
    ui->treeView_machines->setSelectionBehavior(QAbstractItemView::SelectRows);

    /* Transition table */
    ui->tableViewMachine->setModel(0);
    ui->tableViewMachine->horizontalHeader()->setResizeMode(
            QHeaderView::Interactive);
    ui->tableViewMachine->setHorizontalScrollMode(
            QAbstractItemView::ScrollPerPixel);
    ui->tableViewMachine->setVerticalScrollMode(
            QAbstractItemView::ScrollPerPixel);
    ui->tableViewMachine->verticalHeader()->hide();

    /* Memory table */
    ui->tableViewMemory->setModel(0);
    ui->tableViewMemory->horizontalHeader()->setResizeMode(
            QHeaderView::Interactive);
    ui->tableViewMemory->setHorizontalScrollMode(
            QAbstractItemView::ScrollPerPixel);
    ui->tableViewMemory->setVerticalScrollMode(
            QAbstractItemView::ScrollPerPixel);
    ui->tableViewMemory->verticalHeader()->hide();
    ui->tableViewMemory->setSelectionBehavior(QAbstractItemView::SelectRows);

    /* Simulation */
    simulationThread = new SimulationThread();

    defaultGuiSettings();

    connect(ui->pushButtonNew, SIGNAL(clicked()), this, SLOT(newModel()));
    connect(ui->pushButtonOpen, SIGNAL(clicked()), this, SLOT(openModel()));
    connect(ui->pushButtonSave, SIGNAL(clicked()), this, SLOT(saveModel()));
    connect(ui->pushButtonAddMemory, SIGNAL(clicked()),
            this, SLOT(insertMemory()));
    connect(ui->pushButtonDeleteMemory, SIGNAL(clicked()),
            this, SLOT(deleteMemory()));
    connect(ui->treeView_machines, SIGNAL(clicked(QModelIndex)),
            this, SLOT(machineTreeClicked(QModelIndex)));
    ui->treeView_machines->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView_machines, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(machineTreeContextMenu(QPoint)));
    connect(ui->pushButton_update, SIGNAL(clicked()),
            this, SLOT(reload_scene()));
}

MainWindow::~MainWindow() {
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

void MainWindow::startModel() {
    // Check for a valid start state
    int flag = 0;  // machine->checkValidStartState();
    if (flag == 0) {
        emit(clearSceneSelection());
        // if(machine->getStartState() != 0)
        //    machine->getStartState()->setSelected(true);

        // ui->pushButtonPause->setEnabled(true);
        // ui->pushButtonStop->setEnabled(true);
        /*ui->treeWidget->setEnabled(false);
        ui->graphicsView->setEnabled(false);
        ui->tableViewMachine->setEnabled(false);
        ui->tableViewMemory->setEnabled(false);*/

        simulationThread->startSim();
        // ui->label->setText("Model started");
    }
/*  else if(flag == 1)
    ui->label->setText("Error: More than one start state possible");
    else if(flag == 2)
    ui->label->setText("Error: No start states found");*/
}

void MainWindow::pauseModel() {
    // ui->label->setText("Model paused");
}

void MainWindow::stopModel() {
    // ui->label->setText("Model stopped");

    // emit( clearSceneSelection() );

    // ui->pushButtonPause->setEnabled(false);
    // ui->pushButtonStop->setEnabled(false);
    /*ui->treeWidget->setEnabled(true);
    ui->graphicsView->setEnabled(true);
    ui->tableViewMachine->setEnabled(true);
    ui->tableViewMemory->setEnabled(true);*/
}

void MainWindow::selectTransition(QString n) {
    for (int i = 0;
        i < currentMachine->machineModel->getTransitions().size(); i++) {
        if (currentMachine->machineModel->getTransitions().at(i)->name() == n)
            ui->tableViewMachine->selectRow(i);
    }
    currentMachine->machineScene->selectTransition(n);
}

void MainWindow::selectState(QString n) {
    currentMachine->machineScene->selectState(n);
}

void MainWindow::updateStatusLabel(QString /*s*/) {
    // ui->label->setText(s);
}

void MainWindow::insertMemory() {
    currentMachine->memoryModel->insertRow(
        currentMachine->memoryModel->rowCount());
}

void MainWindow::deleteMemory() {
    QModelIndexList indexList =
        ui->tableViewMemory->selectionModel()->selectedRows();

    while (indexList.count() > 0) {
        currentMachine->memoryModel->removeRow(indexList.at(0).row());
        indexList = ui->tableViewMemory->selectionModel()->selectedRows();
    }
}

void MainWindow::newModel() {
    // Add new machine to the machine tree
    Machine * m = machineTree->addMachine();
    // Select file for model
    // Provide dialog to select file
    QString filepath =
        QFileDialog::getSaveFileName(this,
            tr("Choose file to save model to..."), "");

    if (filepath.isEmpty()) {
        machineTree->removeMachine(m);
        return;
    }

    // Get file directory and file name from file info
    QFileInfo finfo(filepath);
    m->fileDirectory = finfo.absolutePath();  // File directory
    m->fileDirectory.append("/");  // Append directory slash
    m->fileName      = finfo.fileName();     // File name

    // Organise the tree view in the gui
    handleNewAndOpenedModel(m);
}

int MainWindow::openModel_internal(QString fileName, bool test) {
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QString error =
                tr("Cannot read file %1:\n%2.")
                .arg(fileName)
                .arg(file.errorString());
        if (!test) QMessageBox::warning(this, tr("x-editor"), error);
        // else qDebug() << error;
        return 1;
    }

    // Add a new machine to the machine tree
    Machine * m = machineTree->addMachine();
    // Read the model into the machine
    modelXMLReader reader(m);
    // If the read fails
    if (!reader.read(&file)) {
        QString error =
            tr("Cannot parse model in file %1 at line %2, column %3:\n%4").arg(
            fileName).arg(reader.lineNumber()).arg(
            reader.columnNumber()).arg(reader.errorString());
        if (!test) QMessageBox::warning(this, "FLAME Editor", error);
        // else qDebug() << error;
        // Delete model from tree
        machineTree->removeMachine(m);
        defaultGuiSettings();
        return 2;
    } else {  // If the read is successful
        // Organise the tree view in the gui
        handleNewAndOpenedModel(m);
        statusBar()->showMessage(tr("File loaded"), 2000);
        return 0;
    }
}

void MainWindow::openModel() {
    QString fileName =
             QFileDialog::getOpenFileName(this, tr("Open model"),
                                          "",
                                          tr("XML Files (*.xml)"));
     if (fileName.isEmpty())
         return;

     openModel_internal(fileName, false);
}

void MainWindow::handleNewAndOpenedModel(Machine *m) {
    // Organise model tree view
    ui->treeView_machines->expandToDepth(0);
    ui->treeView_machines->resizeColumnToContents(0);
    // Select opened model
    ui->treeView_machines->setCurrentIndex(machineTree->getIndex(m));
    // React as if it had been clicked by the user
    machineTreeClicked(machineTree->getIndex(m));
}

void MainWindow::saveModel() {
    QString fileName;
    fileName.append(currentMachine->fileDirectory);
    // Qt will translate these directory separators to the native OS
    fileName.append("/");
    fileName.append(currentMachine->fileName);

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("FLAME Editor"),
                          tr("Cannot write file %1:\n%2.")
                          .arg(fileName)
                          .arg(file.errorString()));
        return;
    }

    if (currentMachine->writeModelXML(&file))
        statusBar()->showMessage(tr("File saved"), 2000);
}

void addRestOfModel(Machine * m, Machine * agent, int foreign) {
    if (m->type == 0) {  // model
        for (int i = 0; i < m->childCount(); i++) {
            if (m->child(i)->type == 1) {  // if agent
                if ((agent->type == 1 && m->child(i)->name == agent->name &&
                        m->child(i) != agent) || agent->type == 0) {
                    QList<Transition*> transitions =
                            m->child(i)->machineModel->getTransitions();

                    for (int j = 0; j < transitions.count(); j++) {
                        Transition * t = transitions.at(j);
                        agent->machineScene->addTransitionTransition(
                                m->child(i)->name, t, foreign);
                    }
                }
            }
            if (foreign == 1) {
                if (m->child(i)->type == 0) {  // model
                    if (m->child(i) != agent)
                        addRestOfModel(m->child(i), agent, foreign);
                }
            }
        }
    }
}

void MainWindow::machineTreeClicked(QModelIndex index) {
    // Disconnect buttons from current machineScene
    if (currentMachine != 0)
        if (currentMachine->machineScene != 0)
            disconnect(ui->pushButtonDeleteMemory,
                    SIGNAL(clicked()),
                    currentMachine->machineScene,
                    SLOT(deleteSelectedFunction()));

    // Get the machine that was clicked on
    Machine * m = static_cast<Machine*>(index.internalPointer());
    // Make the currentMachine equal to the selected machine
    currentMachine = m;

    // Set memory, machine and scene models to be null and update
    ui->tableViewMemory->setModel(0);
    ui->tableViewMemory->update();
    ui->tableViewMachine->setModel(0);
    ui->tableViewMachine->update();
    ui->graphicsView->setScene(0);
    ui->graphicsView->update();

    // Set up buttons for models
    if (m->type == 0) {
        // If the model is a root model then it can be closed
        if (m->rootModel() == m) ui->pushButtonClose->setEnabled(true);
        else
            ui->pushButtonClose->setEnabled(false);
        // Models and sub models can be saved
        ui->pushButtonSave->setEnabled(true);
        // Put the file location as the window title
        setWindowTitle(QString("FLAME Editor - ").append(m->filePath()));
    } else {
        // Only models can be closed and saved
        ui->pushButtonClose->setEnabled(false);
        ui->pushButtonSave->setEnabled(false);
        // Reset the window title
        setWindowTitle("FLAME Editor");
    }

    // Set up memory table for
    // models, agents, messages, environment, datatypes
    if (m->type == 0 || m->type == 1 || m->type == 2
            || m->type == 3 || m->type == 5) {
        // Set up the table view for the current memory
        ui->tableViewMemory->setModel(m->memoryModel);
        if (m->type == 0) {
            ui->tableViewMemory->setItemDelegateForColumn(1,
                    new TextEditDelegate());
        } else {
            ui->tableViewMemory->setItemDelegateForColumn(0,
                    new DataTypeDelegate(m));
            ui->tableViewMemory->setItemDelegateForColumn(1,
                    new LineEditDelegate());
            ui->tableViewMemory->setItemDelegateForColumn(2,
                    new LineEditDelegate());
        }
        ui->tableViewMemory->resizeColumnsToContents();
        ui->tableViewMemory->resizeRowsToContents();
        ui->tableViewMemory->update();

        // Handle memory buttons
        if (m->type == 0) {  // Model memory cannot be added to or deleted
            // Disable memory buttons
            ui->pushButtonAddMemory->setEnabled(false);
            ui->pushButtonDeleteMemory->setEnabled(false);
        } else {
            // Enable memory buttons
            ui->pushButtonAddMemory->setEnabled(true);
            ui->pushButtonDeleteMemory->setEnabled(true);
        }
    } else {
        // Disable memory buttons
        ui->pushButtonAddMemory->setEnabled(false);
        ui->pushButtonDeleteMemory->setEnabled(false);
    }

    // Set up machine table for
    // agents
    if (m->type == 1) {
        // Set up the table view for the current machine model
        ui->tableViewMachine->setModel(m->machineModel);
        ui->tableViewMachine->setItemDelegateForColumn(0,
                new LineEditDelegate());
        ui->tableViewMachine->setItemDelegateForColumn(1,
                new CommDelegate(m));
        ui->tableViewMachine->setItemDelegateForColumn(2,
                new MpreDelegate(m));
        ui->tableViewMachine->setItemDelegateForColumn(3,
                new LineEditDelegate());
        ui->tableViewMachine->setItemDelegateForColumn(4,
                new MpostDelegate(m->memoryModel));
        ui->tableViewMachine->setItemDelegateForColumn(5,
                new CommDelegate(m));
        ui->tableViewMachine->setItemDelegateForColumn(6,
                new LineEditDelegate());
        ui->tableViewMachine->setItemDelegateForColumn(7,
                new LineEditDelegate());
        ui->tableViewMachine->resizeColumnsToContents();
        ui->tableViewMachine->resizeRowsToContents();
        ui->tableViewMachine->update();

        // When input/output messages have been edited then
        // resize the rows depending on the new contents
        connect(ui->tableViewMachine->itemDelegateForColumn(1),
                SIGNAL(closeEditor(QWidget*)),
                ui->tableViewMachine, SLOT(resizeRowsToContents()));
        connect(ui->tableViewMachine->itemDelegateForColumn(5),
                SIGNAL(closeEditor(QWidget*)),
                ui->tableViewMachine, SLOT(resizeRowsToContents()));
        connect(m->machineModel, SIGNAL(communicationChanged()),
                ui->tableViewMachine, SLOT(resizeRowsToContents()));
    }

    // Set up graphics scene for
    // models, agents
    if (m->type == 0 || m->type == 1) {
        // Load other states and transitions from the rest of the other model
        addRestOfModel(m, m, 0);  // Add local agent functions
        addRestOfModel(m->rootModel(), m, 1);  // Add global agent functions

        // Set up graphics view
        ui->graphicsView->setScene(m->machineScene);
        m->machineScene->setGraphicsView(ui->graphicsView);
        m->machineScene->calcLayers();
        ui->graphicsView->setAlignment(Qt::AlignTop);
        ui->graphicsView->verticalScrollBar()->setValue(0);
        ui->graphicsView->show();

        if (m->type == 1) {  // if agent
            connect(m->machineScene, SIGNAL(functionSelected(bool)),
                    ui->pushButtonSceneDelete, SLOT(setEnabled(bool)));
            connect(ui->pushButtonSceneDelete, SIGNAL(clicked()),
                    m->machineScene, SLOT(deleteSelectedFunction()));
        }

        // Enable/disable graph buttons
        ui->pushButtonSceneDelete->setEnabled(false);
        ui->pushButton_viewModel->setEnabled(true);
        ui->pushButton_update->setEnabled(true);
    } else {
        // Disable graph buttons
        ui->pushButton_viewModel->setEnabled(false);
        ui->pushButton_update->setEnabled(false);
    }
}

void MainWindow::machineTreeContextMenu(QPoint p) {
    QPoint globalPos = ui->treeView_machines->mapToGlobal(p);
    QModelIndex index = ui->treeView_machines->indexAt(p);
    Machine * m = static_cast<Machine*>(index.internalPointer());

    QMenu myMenu;
    myMenu.addAction("Add agent");
    myMenu.addAction("Add message");
    myMenu.addAction("Add function file");
    myMenu.addAction("Add sub model");
    myMenu.addAction("Add time unit");
    myMenu.addAction("Add data type");
    if (m->type == 0) {
        if (m->rootModel() != m) myMenu.addAction("Remove sub model"); }
    else if (m->type == 1) myMenu.addAction("Delete agent");
    else if (m->type == 2) myMenu.addAction("Delete message");
    else if (m->type == 4) myMenu.addAction("Delete time unit");
    else if (m->type == 5) myMenu.addAction("Delete data type");
    else if (m->type == 6) myMenu.addAction("Delete function file");

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem) {
        if (selectedItem->text().startsWith("Add")) {
            // Find current model/sub model
            Machine * modelm = m;  // The model machine to add to
            while (modelm->type != 0) modelm = modelm->parent();
            int newType = 0;

            if (selectedItem->text() == "Add agent") newType = 1;
            else if (selectedItem->text() == "Add message") newType = 2;
            else if (selectedItem->text() == "Add function file") newType = 6;
            else if (selectedItem->text() == "Add sub model") newType = 0;
            else if (selectedItem->text() == "Add time unit") newType = 4;
            else if (selectedItem->text() == "Add data type") newType = 5;
            Machine * m1 = machineTree->insertMachine(modelm, newType);
            if (newType == 0) {  // Sub model
                // Select file for sub model
                QString s = modelm->fileDirectory;
                /* Provide dialog to select folder */
                QString filepath =
                        QFileDialog::getSaveFileName(this,
                                tr("Sub model file"), s);
                /* If new file is selected, i.e. the user canceled */
                if (filepath.isEmpty()) {
                    machineTree->removeMachine(m1);
                    return;
                }
                /* Return relative path from currentPath to location */
                QDir dir(modelm->fileDirectory);
                QString s1 = dir.canonicalPath();
                QDir dir2(s1);
                s = dir2.relativeFilePath(filepath);
                QFileInfo finfo(filepath);
                QString s2 = finfo.path();  // canonicalPath();
                    // Cannot use canonical because file needs to exist
                m1->fileDirectory = s2;
                m1->fileName = s;
            } else if (newType == 1) {  // Agent
                int count = 0;
                // Count number of agents
                for (int i = 0; i < modelm->childCount(); i++)
                    if (modelm->child(i)->type == 1) count++;
                m1->name = QString("agent_%1").arg(QString::number(count-1));
                // Add a starting transition
                m1->addTransitionString("transition_0", "start", "end",
                        Condition(), Mpost(),
                        Communication(), Communication(), "");
            } else if (newType == 2) {  // Message
                int count = 0;
                // Count number of agents
                for (int i = 0; i < modelm->childCount(); i++)
                    if (modelm->child(i)->type == 2) count++;
                m1->name = QString("message_%1").arg(QString::number(count-1));
            } else if (newType == 4) {  // Time unit
                // Name the time unit as it can be used
                // straight away as a variable type
                // Count the number of time units and use this for the name
                int tuCount = 0;
                // Find environment machine
                Machine * env = 0;
                for (int i = 0; i < modelm->childCount(); i++)
                    if (modelm->child(i)->type == 3) env = modelm->child(i);
                // Count number of data types
                for (int i = 0; i < env->childCount(); i++)
                    if (env->child(i)->type == 4) tuCount++;
                m1->setTimeUnitName(QString("timeunit_%1").arg(
                        QString::number(tuCount-1)));
            } else if (newType == 5) {  // Data type
                // Name the data type as it can be used
                // straight away as a variable type
                // Count the number of data types and use this for the name
                int dtCount = 0;
                // Find environment machine
                Machine * env = 0;
                for (int i = 0; i < modelm->childCount(); i++)
                    if (modelm->child(i)->type == 3) env = modelm->child(i);
                // Count number of data types
                for (int i = 0; i < env->childCount(); i++)
                    if (env->child(i)->type == 5) dtCount++;
                m1->name =
                        QString("datatype_%1").arg(QString::number(dtCount-1));
            }
            // Select the machine in the tree
            QModelIndex index = machineTree->getIndex(m1);  // Get its index
            ui->treeView_machines->scrollTo(index);  // Scroll to it
            ui->treeView_machines->setCurrentIndex(index);  // Select it
            emit(machineTreeClicked(index));  // Act like it has been clicked
        } else if (selectedItem->text().startsWith("Delete") ||
                selectedItem->text().startsWith("Remove")) {
            QMessageBox msgBox;
            QString text;
            if (selectedItem->text().startsWith("Delete"))
                text.append("Do you want to delete the ");
            else if (selectedItem->text().startsWith("Remove"))
                text.append("Do you want to remove the ");
            if (m->type == 0) text.append("sub model: ");
            else if (m->type == 1) text.append("agent: ");
            else if (m->type == 2) text.append("message: ");
            else if (m->type == 4) text.append("time unit: ");
            else if (m->type == 5) text.append("data type: ");
            else if (m->type == 6) text.append("function file: ");
            if (m->type == 4) text.append(m->timeUnit().name);
            else
                text.append(m->name);
            text.append("?");
            msgBox.setText(text);
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Yes);
            if (msgBox.exec() == QMessageBox::Yes) {
                machineTree->removeMachine(m);
            }
        }
    } else {
        // nothing was chosen
        // qDebug() << "nothing chosen";
    }
}

void MainWindow::repositionView(float x, float y) {
    ui->graphicsView->centerOn(x, y);
}

void MainWindow::on_pushButton_viewModel_clicked() {
    QGraphicsView * modelView = new QGraphicsView(currentMachine->machineScene);
    currentMachine->machineScene->setGraphicsView(modelView);
    modelView->show();
    modelView->fitInView(currentMachine->machineScene->sceneRect(),
        Qt::KeepAspectRatioByExpanding);
}

void MainWindow::on_pushButtonClose_clicked() {
    machineTree->removeMachine(currentMachine);
    defaultGuiSettings();
}

void MainWindow::defaultGuiSettings() {
    /* Disable gui features */
    ui->tableViewMemory->setModel(0);
    ui->tableViewMemory->update();
    ui->tableViewMachine->setModel(0);
    ui->tableViewMachine->update();
    ui->graphicsView->setScene(0);
    ui->graphicsView->update();
    ui->pushButton_viewModel->setEnabled(false);
    ui->pushButtonAddMemory->setEnabled(false);
    ui->pushButtonClose->setEnabled(false);
    ui->pushButtonDeleteMemory->setEnabled(false);
    ui->pushButtonSave->setEnabled(false);
    ui->pushButtonSceneDelete->setEnabled(false);
    ui->pushButton_update->setEnabled(false);
}

void MainWindow::on_actionHelp_triggered() {
    QTextBrowser *help = new QTextBrowser(this);
    help->setWindowFlags(Qt::Dialog);
    help->setReadOnly(true);
    help->setAutoFormatting(QTextEdit::AutoNone);
    help->setOpenExternalLinks(true);
    QString helpText;
    helpText.append("<h1>FLAME Editor Help</h1>");
    helpText.append("<a href=\"http://www.flame.ac.uk/docs/flameeditor/v1\">");
    helpText.append("Follow this link to the online help page.</a>");

    help->setGeometry(50, 50, 600, 400);
    help->insertHtml(helpText);
    help->moveCursor(QTextCursor::Start);
    help->show();
}

void MainWindow::on_actionAbout_triggered() {
    QTextEdit *about = new QTextEdit(this);
    about->setWindowFlags(Qt::Dialog);
    about->setReadOnly(true);
    about->setAutoFormatting(QTextEdit::AutoNone);

    QString aboutText;
    aboutText.append("<h1>FLAME Editor</h1>");
    aboutText.append("<h3>Simon Coakley</h3>");
    aboutText.append("<h2>Version 1</h2>");
    aboutText.append("<h3>Changelog</h3>");
    /* Add new release notes here */
    aboutText.append("<h4>Version 1 (released 2012-01-13)</h4>");
    aboutText.append("<ul><li>Beta first release</li></ul>");

    about->setGeometry(50, 50, 600, 400);
    about->insertHtml(aboutText);
    about->moveCursor(QTextCursor::Start);
    about->show();
}

void MainWindow::reload_scene() {
    currentMachine->machineScene->calcLayers();
}
