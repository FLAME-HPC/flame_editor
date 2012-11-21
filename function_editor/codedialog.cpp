#include "codedialog.h"
#include "./assignmentdialog.h"
#include "functioncodedialog.h"
#include <QDebug>
#include <QCompleter>
#include <QFile>
#include <QStringListModel>
#include <QApplication>
#include <QStandardItemModel>
#include <QStringList>
#include <QMessageBox>

#include "./linkedlist.h"
#include <stdlib.h>

CodeDialog::CodeDialog(MemoryModel *m, QWidget *parent) :
    QDialog(parent), agentMemory(m)
{
    setupUi(this);

    // Create the machine scene object
    machineScene = new FEMachineScene(this);
    // Set the graphics view to use the machine scene object
    graphicsView->setScene(machineScene);
    // If item edit selected then start function code dialog
    connect(machineScene, SIGNAL(functionCodeDialog(FEGraphicsItem*)),
            this, SLOT(functionCodeDialog(FEGraphicsItem*)));

    functionName->setEnabled(false);

    //this->setMinimumSize(500, 500);
    tableViewMemory->verticalHeader()->hide();
    tableViewMemory->setModel(agentMemory);
    tableViewMemory->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableViewMemory->resizeColumnsToContents();
    tableViewMemory->resizeRowsToContents();
    tableViewMemory->update();

    variablesDeclared = new VariableDeclaredModel;
    tableViewVariables->setItemDelegate(new VariableDeclaredDelegate);
    tableViewVariables->verticalHeader()->hide();
    tableViewVariables->setModel(variablesDeclared);
    selectionModel = tableViewVariables->selectionModel();

    pbRemove->setEnabled(false);

    connect(selectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(currentRowChanged(QItemSelection,QItemSelection)));
    connect(pbAdd, SIGNAL(clicked()), this, SLOT(add_click()));
    connect(pbRemove, SIGNAL(clicked()), this, SLOT(remove_click()));
    //connect(pbCheck, SIGNAL(clicked()), this, SLOT(check_click()));
    //connect(leName, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));

#ifdef debug_on
    connect(pbSaveXML, SIGNAL(clicked()), this, SLOT(saveXML_click()));
    connect(pbSaveC, SIGNAL(clicked()), this, SLOT(saveC_click()));
    connect(pbReadXML, SIGNAL(clicked()), this, SLOT(readXML_click()));
#endif
/*    connect(openAction, SIGNAL(triggered()), this, SLOT(open_click()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save_click()));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs_click()));
    connect(saveGenerateAction, SIGNAL(triggered()), this, SLOT(saveGenerate_click()));
    connect(generateAction, SIGNAL(triggered()), this, SLOT(generate_click()));

    connect(mainMenu, SIGNAL(aboutToShow()), this, SLOT(showMainMenu()));*/

//    machineScene->installEventFilter(this);
}

CodeDialog::~CodeDialog()
{
    //delete machineScene;
    commitAndCloseEditor();
}

void CodeDialog::hideEvent(QHideEvent */*e*/)
{
    //qDebug()<<"hide";
}

void CodeDialog::closeEvent(QCloseEvent *e)
{
    //qDebug()<<"pppppppppppppppppppppp";
/*
    autocompletionTextEdit->testValid();
    if(isShowList() && isCorrect() == false)
    {
        e->ignore();
        autocompletionTextEdit->showMessage();
        autocompletionTextEdit->setFocus();
    }
    else
        if(machineScene->isChanged())
        {
            QString sm = "Current project is not saved! You want to save?";
            QMessageBox::StandardButton sb = QMessageBox::warning(this, this->windowTitle(), sm,
                                                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if(sb == QMessageBox::Yes)
                if(saveFile() == false)
                    e->ignore();
                else
                    e->accept();
            else if(sb == QMessageBox::Cancel)
                e->ignore();
            else if(sb == QMessageBox::No)
                e->accept();
        }
        else*/
            e->accept();

}

bool CodeDialog::eventFilter(QObject *o, QEvent *e)
{
    //qDebug()<<"777777777777777777778978878";
    //qDebug()<<autocompletionTextEdit<<"  "<<o;
/*
    if(o == machineScene && e->type() == QEvent::FocusIn)
    {
        qDebug()<<"---- ====  "<<(autocompletionTextEdit->isCorrect() == false);
        if(autocompletionTextEdit->isEnabled() && autocompletionTextEdit->isCorrect() == false &&
                autocompletionTextEdit->isAfterShowMessageBox() == false)
        {
            autocompletionTextEdit->showMessage();
            return true;
        }
        else if(autocompletionTextEdit->isEnabled() && autocompletionTextEdit->isAfterShowMessageBox())
        {
            autocompletionTextEdit->setFocus();
            return true;
        }
        else if(autocompletionTextEdit->isCorrect() && getShowList())
        {
            if(getShowList())
                setShowList(false);
        }
        if(getShowSelectItem())
        {
            if(isAfterShowMessageBox)
            {
                isAfterShowMessageBox = false;
                leName->setFocus();
                return true;
            }
            QString s = leName->text();
            if(s == "")
            {
                isAfterShowMessageBox = true;
                QMessageBox::warning(this, tr(""), "Must enter the identifier name, this can not be empty!");
                return true;
            }
            else if(machineScene->isNameInList(s))
            {
                isAfterShowMessageBox = true;
                QMessageBox::warning(this, tr(""), "There is another item with the same identifier, please enter another name!");
                return true;
            }
            else
            {
                machineScene->setName(s);
                setShowSelectItem(false);
            }
        }
    }
    */
    return QDialog::eventFilter(o, e);
}

void CodeDialog::functionCodeDialog(FEGraphicsItem *item) {
    QStringList variableNames;

    // Get variable names
    variableNames.append(agentMemory->getNames());
    variableNames.append(variablesDeclared->getNames());

    FunctionCodeDialog *editor = new FunctionCodeDialog(item, variableNames);
    connect(editor, SIGNAL(accepted()), this, SLOT(commitAndCloseEditor()));
    editor->setModal(true);
    editor->show();
}

void CodeDialog::commitAndCloseEditor() {
    //FunctionCodeDialog *editor = qobject_cast<FunctionCodeDialog *>(sender());
    //emit commitData(editor);
    //emit closeEditor(editor);
    qDebug() << "Close FunctionCodeDialog";
}

void CodeDialog::setMpost(Mpost c) {
    mpost = c;
}

Mpost CodeDialog::getMpost() {
    return mpost;
}

void CodeDialog::setName(QString n) {
    functionName->setText(n);
}

/*
void CodeDialog::setUI()
{
    mainLayout = new QVBoxLayout();
    menuBar = new QMenuBar();
    mainMenu = new QMenu("File", menuBar);

    openAction = new QAction("Open...", mainMenu);
    saveAction = new QAction("Save", mainMenu);
    saveAsAction = new QAction("Save As...", mainMenu);
    saveGenerateAction = new QAction("Save And Generate", mainMenu);
    generateAction = new QAction("Generate", mainMenu);

    mainMenu->addAction(openAction);
    mainMenu->addAction(saveAction);
    mainMenu->addAction(saveAsAction);
    mainMenu->addAction(saveGenerateAction);
    mainMenu->addAction(generateAction);

    menuBar->addMenu(mainMenu);
    mainLayout->setMenuBar(menuBar);
//    mainContentLayout = new QVBoxLayout();

//    QHBoxLayout *gUpLayout = new QHBoxLayout();

//    lFunctionName = new QLabel("Function Name");
//    lFunctionName->setMaximumSize(80, 10);
//    lFunctionName->setMinimumSize(50, 10);
//    leFunctionName = new QLineEdit();
//    leFunctionName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    leFunctionName->setMinimumSize(100, 20);
//    leFunctionName->setMaximumSize(100, 20);
//    leFunctionName->setText("function1");

//    gUpLayout->addWidget(lFunctionName);
//    gUpLayout->addWidget(leFunctionName, 0, Qt::AlignLeft);

//    mainLayout->addLayout(gUpLayout);

 //   QGridLayout *gridLayout = new QGridLayout;

 //   QGridLayout *gridLayoutTableMemory = new QGridLayout;

//    QLabel *lMemory = new QLabel("Memory");
//    lMemory->setMaximumSize(80, 10);
//    lMemory->setMinimumSize(50, 10);

//    gridLayoutTableMemory->addWidget(lMemory, 0, 0);

//    tableViewMemory = new QTableView;
//    tableViewMemory->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    tableViewMemory->setMinimumSize(319, 110);
//    tableViewMemory->setMaximumSize(319, 110);

//    gridLayoutTableMemory->addWidget(tableViewMemory, 1, 0, 2, 2);

//    gridLayout->addLayout(gridLayoutTableMemory, 0, 0, 1, 1, Qt::AlignRight);

//    QGridLayout *gridLayoutTableVariables = new QGridLayout;

//    QLabel *lVariables = new QLabel("Local Variables");
//    lVariables->setMaximumSize(80, 10);
//    lVariables->setMinimumSize(50, 10);

//    gridLayoutTableVariables->addWidget(lVariables, 0, 0);

//    tableViewVariables = new QTableView;
//    tableViewVariables->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    tableViewVariables->setMinimumSize(319, 80);
//    tableViewVariables->setMaximumSize(319, 80);

//    gridLayoutTableVariables->addWidget(tableViewVariables, 1, 0, 1, 2);

//    pbAdd = new QPushButton("Add");
//    pbAdd->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    pbAdd->setMinimumSize(50, 25);

//    gridLayoutTableVariables->addWidget(pbAdd, 2, 0, Qt::AlignLeft);

//    pbRemove = new QPushButton("Remove");
//    pbRemove->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    pbRemove->setMinimumSize(50, 25);

//    gridLayoutTableVariables->addWidget(pbRemove, 2, 1, Qt::AlignRight);

//    gridLayout->addLayout(gridLayoutTableVariables, 0, 1, 1, 1, Qt::AlignLeft);

//    mainLayout->addLayout(gridLayout);

//    graphicsView = new QGraphicsView(this);
//    mainContentLayout->addWidget(graphicsView);
//    mainLayout->addLayout(mainContentLayout);
//    setLayout(mainLayout);
//    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    // Create the machine scene object
    machineScene = new FEMachineScene(this);
    // Set the graphics view to use the machine scene object
    graphicsView->setScene(machineScene);
    connect(machineScene, SIGNAL(myedit(FEGraphicsItem*)), this, SLOT(edit(FEGraphicsItem*)));

//    QHBoxLayout *gLayout = new QHBoxLayout();

//    lName = new QLabel("vedem");
//    lName->setMaximumSize(80, 10);
//    lName->setMinimumSize(50, 10);
//    leName = new QLineEdit();
//    leName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    leName->setMinimumSize(100, 20);
//    leName->setMaximumSize(100, 20);

//    mainLayout->addSpacing(10);

//    gLayout->addWidget(lName);
//    gLayout->addWidget(leName, 0, Qt::AlignLeft);

//    mainLayout->addLayout(gLayout);

//    listLayout = new QVBoxLayout();
//    label = new QLabel("Assignments");
//    listLayout->addWidget(label);

    //listWidget = new QListWidget();
    //listWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //listWidget->setMinimumSize(50, 10);
    //listLayout->addWidget(listWidget);

//    autocompletionTextEdit = new AutocompletionTextEdit;
//    autocompletionTextEdit->setWordWrapMode(QTextOption::NoWrap);
//    autocompletionTextEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
//    autocompletionTextEdit->setMinimumSize(50, 10);
    //autocompletionTextEdit->setMaximumSize(500, 150);
//    completer = new TreeModelCompleter(this);
//    completer->setSeparator(".");
    //completer->setModel(modelFromFileList(":/resources/wordlist.txt"));
//    QStringList list;
//    list<<"pooo"<<"poo";
//    completer->setModel(modelFromFileTree(list));
//    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
//    completer->setCaseSensitivity(Qt::CaseInsensitive);
//    completer->setWrapAround(false);
//    autocompletionTextEdit->setCompleter(completer);
//    listLayout->addWidget(autocompletionTextEdit);


//    QHBoxLayout *h = new QHBoxLayout();

//    pbCheck = new QPushButton("Check");
//    pbCheck->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    pbCheck->setMinimumSize(50, 25);
//    h->addWidget(pbCheck, 0, Qt::AlignLeft);

#ifdef debug_on
    pbSaveXML = new QPushButton("Save XML");
    pbSaveXML->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pbSaveXML->setMinimumSize(50, 25);
    h->addWidget(pbSaveXML, 0, Qt::AlignLeft);

    pbSaveC = new QPushButton("Save C");
    pbSaveC->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pbSaveC->setMinimumSize(50, 25);
    h->addWidget(pbSaveC, 0, Qt::AlignLeft);

    pbReadXML = new QPushButton("Read XML");
    pbReadXML->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pbReadXML->setMinimumSize(50, 25);
    h->addWidget(pbReadXML, 0, Qt::AlignLeft);
#endif

//    listLayout->addLayout(h);

//    mainLayout->addLayout(listLayout);

    //pbAdd->setVisible(false);
    //pbRemove->setVisible(false);
}


QAbstractItemModel *CodeDialog::modelFromFileTree(const QStringList &list)
{

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif

    QStandardItemModel *model = new QStandardItemModel(completer);
    QVector<QStandardItem *> parents(10);
    parents[0] = model->invisibleRootItem();

    for (int i = 0; i < list.count();i++) {
        QString line = list[i];
        QString trimmedLine = line.trimmed();
        if (line.isEmpty() || trimmedLine.isEmpty())
            continue;

        QRegExp re("^\\s+");
        int nonws = re.indexIn(line);
        int level = 0;
        if (nonws == -1) {
            level = 0;
        } else {
            if (line.startsWith("\t")) {
                level = re.cap(0).length();
            } else {
                level = re.cap(0).length()/4;
            }
        }

        if (level+1 >= parents.size())
            parents.resize(parents.size()*2);

        QStandardItem *item = new QStandardItem;
        item->setText(trimmedLine);
        parents[level]->appendRow(item);
        parents[level+1] = item;
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    return model;
}
*/
/*
bool CodeDialog::openFile()
{
    if(machineScene->isChanged())
    {
        QString sm = "Current project is not saved! You want to save?";
        QMessageBox::StandardButton sb = QMessageBox::warning(this, this->windowTitle(), sm,
                                                              QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(sb == QMessageBox::Yes)
            if(saveFile() == false)
                return false;
        else if(sb == QMessageBox::Cancel)
            return false;
    }
    QString s = QFileDialog::getOpenFileName(this, "Open", "", tr("XML (*.xml)"));
    if(s != "")
    {
        QFileInfo f(s);
        qDebug()<<s<<f.filePath();
        fileName = s;
        machineScene->readFile(fileName);
        return true;
    }
    return false;
    //machineScene->readFile("C:\\Documents and Settings\\Administrator\\Desktop\\Qt\\test.xml");
}

bool CodeDialog::saveFile()
{
    if(fileName != "")
    {
        machineScene->saveFile(fileName, leFunctionName->text(), 0);
        machineScene->arrangeGraphicsItem();
        return true;
    }
    else
        return saveAsFile();
}

bool CodeDialog::saveAsFile()
{
    QString s = QFileDialog::getSaveFileName(this, "Save", "", tr("XML (*.xml)"));
    if(s != "")
    {
        fileName = s;
        machineScene->saveFile(fileName, leFunctionName->text(), 0);
        machineScene->arrangeGraphicsItem();
        return true;
    }
    return false;
}

bool CodeDialog::saveGenerateFile()
{
    if(machineScene->isChanged())
    {
        QString sm = "Current project is not saved! You want to save?";
        QMessageBox::StandardButton sb = QMessageBox::warning(this, this->windowTitle(), sm,
                                                              QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(sb == QMessageBox::Yes)
            if(saveFile() == false)
                return false;
        else if(sb == QMessageBox::Cancel)
            return false;
    }
    if(fileName != "" && fileName.contains("."))
    {
        QString s = fileName.left(fileName.lastIndexOf(".") + 1) + "c";
        qDebug()<<s;
        machineScene->saveFile(s, leFunctionName->text(), 1);
    }
}

bool CodeDialog::generateFile()
{
    if(fileName == "")
    {
        QString sm = "Current project is not saved! You want to save?";
        QMessageBox::StandardButton sb = QMessageBox::warning(this, this->windowTitle(), sm,
                                                              QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(sb == QMessageBox::Yes)
            if(saveFile() == false)
                return false;
        else if(sb == QMessageBox::Cancel)
            return false;
    }
    if(fileName != "" && fileName.contains("."))
    {
        QString s = fileName.left(fileName.lastIndexOf(".") + 1) + "c";
        qDebug()<<s;
        machineScene->saveFile(s, leFunctionName->text(), 1);
    }
}
*/
void CodeDialog::done(int r)
{
    // All function code should be checked here
    // as local memory could have been changed
    QDialog::done(r);

//    qDebug()<<"oooooooooooooorrrrrrrrrrrrrrrrroooooooooooooooooooo";
//    autocompletionTextEdit->testValid();
//    if(isShowList() && isCorrect() == false)
//    {
//        autocompletionTextEdit->showMessage();
//        autocompletionTextEdit->setFocus();
//        return;
//    }
//    else
//    {
//        QDialog::done(r);
//        return;
//    }
}
/*
void CodeDialog::check_click()
{
    //QStringList identifierList;
    //char *s = "8ijik";
    //identifierList.append(QString(s));

    stringList *list = (stringList *) malloc(sizeof(stringList));
    char c[] = "ioi";
    char *cl;
    cl = c;
    addStringList(list, cl);
}

void CodeDialog::saveXML_click()
{
    //if(machineScene->check())
    machineScene->saveFile("C:\\Documents and Settings\\Administrator\\Desktop\\Qt\\test.xml", leFunctionName->text(), 0);
    machineScene->arrangeGraphicsItem();
}

void CodeDialog::saveC_click()
{
    machineScene->saveFile("C:\\Documents and Settings\\Administrator\\Desktop\\Qt\\test.txt", leFunctionName->text(), 1);
}

void CodeDialog::readXML_click()
{
    machineScene->readFile("C:\\Documents and Settings\\Administrator\\Desktop\\Qt\\test.xml");
}

void CodeDialog::open_click()
{
    openFile();
}

void CodeDialog::save_click()
{
    saveFile();
}

void CodeDialog::saveAs_click()
{
    saveAsFile();
}

void CodeDialog::saveGenerate_click()
{
    saveGenerateFile();
}

void CodeDialog::generate_click()
{
    generateFile();
}
*/
void CodeDialog::add_click()
{
    variablesDeclared->insertRow(1);
}

void CodeDialog::remove_click()
{
    QModelIndex index = selectionModel->currentIndex();
    if(index.isValid())
    {
        variablesDeclared->removeRow(index.row());
        selectionModel->clearSelection();
    }
    pbRemove->setEnabled(false);
}

void CodeDialog::currentRowChanged(const QItemSelection &selected, const QItemSelection /*&deselected*/)
{
    pbRemove->setEnabled(selected.indexes().count() > 0);
}
/*
void CodeDialog::showMainMenu()
{
    //if(!(isShowSelectItem() || isShowList()))
    //    saveAction->setEnabled(machineScene->isChanged());
}
*/
/*void CodeDialog::textChanged(const QString &s)
{
    machineScene->setSelectItemText(s);
}*/
/*
void CodeDialog::edit(FEGraphicsItem *item) {
    qDebug() << "my edit " << item->getName();
}

void CodeDialog::setShowList(bool b) {
    showList = b;
    //label->setVisible(b);
    //autocompletionTextEdit->setVisible(b);
    //pbCheck->setVisible(b);
    //pbRemove->setVisible(b);
    if(b){
        //autocompletionTextEdit->setFocus();
        //autocompletionTextEdit->setType(machineScene->itemType());
        QStringList *list = machineScene->getAssignmentList();
        if(list != 0){
            //autocompletionTextEdit->setAssignments(*list);
        }
    }
    else {
        QStringList *list = machineScene->getAssignmentList();
        if(list != 0)
            list->clear();
        //QStringList l = autocompletionTextEdit->getAssignments();
//            for(int i=0;i<l.count();i++)
//                list->append(QString(l[i]));
//            if(list !=0 && list->count() == 0 && (machineScene->itemType() == ConditionWhile || machineScene->itemType() == ConditionFor))
//                machineScene->changeCondition(ConditionWhileFor);
    }
}

bool CodeDialog::getShowSelectItem() const{
    return showSelectItem;
}

void CodeDialog::setShowSelectItem(bool b){
    showSelectItem = b;
    //lName->setVisible(b);
    //leName->setVisible(b);
//        if(b && autocompletionTextEdit->isVisible() == false)
//            leName->setFocus();

    openAction->setEnabled(!b);
    saveAction->setEnabled(!b);
    saveAsAction->setEnabled(!b);
    saveGenerateAction->setEnabled(!b);
    generateAction->setEnabled(!b);

#ifdef debug_on
    pbSaveXML->setVisible(!b);
    pbSaveC->setVisible(!b);
    pbReadXML->setVisible(!b);
#endif
}
*/

/*void CodeDialog::setSelectItem(QString s, QString t){
//        lName->setText(s);
//        leName->setText(t);
}*/

/*void CodeDialog::setFunctionName(QString s) {
    leFunctionName->setText(s);
}

bool CodeDialog::getShowList() const{
    return showList;
}*/
