#include "cecodedialog.h"
#include "./ceassignmentdialog.h"
#include <QDebug>
#include <QCompleter>
#include <QByteArray>
#include <QFile>
#include <QStringListModel>
#include <QApplication>
#include <QStandardItemModel>
#include <QStringList>
#include <QMessageBox>

#include "./celinkedlist.h"
#include <stdlib.h>

CECodeDialog::CECodeDialog(QWidget *parent) :
    QDialog(parent)
{

    Qt::WindowFlags f = 0;
    f |= Qt::WindowMaximizeButtonHint;

    setWindowFlags(f);
    setModal(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle("Code Editor");

    setUI();

    memory = new CEMemoryModel;
    tableViewMemory->verticalHeader()->hide();
    tableViewMemory->setModel(memory);

    memory->insertRow(1);
    memory->insertRow(1);
    memory->insertRow(1);
    memory->insertRow(1);

    variablesDeclared = new CEVariableDeclaredModel;
    variablesDeclared->setMemoryVariableNames(memory->getNames());
    variableDeclaredDelegate = new CEVariableDeclaredDelegate();
    variableDeclaredDelegate->setMemoryVariableNames(memory->getNames());
    tableViewVariables->setItemDelegate(variableDeclaredDelegate);
    tableViewVariables->verticalHeader()->hide();
    tableViewVariables->setModel(variablesDeclared);
    selectionModel = tableViewVariables->selectionModel();

    showList = false;
    showSelectItem = false;

    isAfterShowMessageBox = false;

    changed = false;

    //QStringList l;
    //l << "add" << "ok";
    //QCompleter *c = new QCompleter(l);
    //leName->setCompleter(c);

    this->setMinimumSize(500, 500);

    setShowSelectItem(false);
    setShowList(false);

    pbRemove->setEnabled(false);

    connect(selectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(currentRowChanged(QItemSelection,QItemSelection)));

    connect(variableDeclaredDelegate, SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)), this,
            SLOT(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)));

    connect(pbAdd, SIGNAL(clicked()), this, SLOT(add_click()));

    connect(pbRemove, SIGNAL(clicked()), this, SLOT(remove_click()));

    connect(pbCheck, SIGNAL(clicked()), this, SLOT(check_click()));

#ifdef debug_on
    connect(pbSaveXML, SIGNAL(clicked()), this, SLOT(saveXML_click()));

    connect(pbSaveC, SIGNAL(clicked()), this, SLOT(saveC_click()));

    connect(pbReadXML, SIGNAL(clicked()), this, SLOT(readXML_click()));
#endif

    connect(leName, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));


    connect(openAction, SIGNAL(triggered()), this, SLOT(open_click()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save_click()));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs_click()));
    connect(saveGenerateAction, SIGNAL(triggered()), this, SLOT(saveGenerate_click()));
    connect(generateAction, SIGNAL(triggered()), this, SLOT(generate_click()));

    connect(mainMenu, SIGNAL(aboutToShow()), this, SLOT(showMainMenu()));

    machineScene->installEventFilter(this);

}

CECodeDialog::~CECodeDialog()
{
    delete machineScene;
}

void CECodeDialog::hideEvent(QHideEvent *e)
{
    qDebug()<<"hide";
}

void CECodeDialog::closeEvent(QCloseEvent *e)
{
    qDebug()<<"pppppppppppppppppppppp";
    autocompletionTextEdit->testValid();
    if(variableDeclaredDelegate->isEditMode())
        e->ignore();
    else
    if(isShowList() && isCorrect() == false)
    {
        e->ignore();
        autocompletionTextEdit->showMessage();
        autocompletionTextEdit->setFocus();
    }
    else
        if(machineScene->isChanged() || changed)
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
        else
            e->accept();
}

void CECodeDialog::done(int r)
{
    qDebug()<<"oooooooooooooorrrrrrrrrrrrrrrrroooooooooooooooooooo";
    autocompletionTextEdit->testValid();
    if(isShowList() && isCorrect() == false)
    {
        autocompletionTextEdit->showMessage();
        autocompletionTextEdit->setFocus();
        return;
    }
    else
        if(machineScene->isChanged() || changed)
        {
            QString sm = "Current project is not saved! You want to save?";
            QMessageBox::StandardButton sb = QMessageBox::warning(this, this->windowTitle(), sm,
                                                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if(sb == QMessageBox::Yes)
                if(saveFile() == false)
                    return;
                else
                {
                    QDialog::done(r);
                    return;
                }
            else if(sb == QMessageBox::Cancel)
                return;
            else if(sb == QMessageBox::No)
            {
                QDialog::done(r);
                return;
            }
        }
        else
        {
            QDialog::done(r);
            return;
        }
}

bool CECodeDialog::eventFilter(QObject *o, QEvent *e)
{
    //qDebug()<<"777777777777777777778978878";
    //qDebug()<<autocompletionTextEdit<<"  "<<o;
    if(o == machineScene && e->type() == QEvent::FocusIn)
    {
        qDebug()<<"---- ====  "<<(autocompletionTextEdit->isCorrect() == false);
        if(autocompletionTextEdit->isVisible() && autocompletionTextEdit->isCorrect() == false &&
                autocompletionTextEdit->isAfterShowMessageBox() == false)
        {
            autocompletionTextEdit->showMessage();
            return true;
        }
        else if(autocompletionTextEdit->isVisible() && autocompletionTextEdit->isAfterShowMessageBox())
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
    return QDialog::eventFilter(o, e);
}

void CECodeDialog::setUI()
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
    mainContentLayout = new QVBoxLayout();

    QHBoxLayout *gUpLayout = new QHBoxLayout();

    lFunctionName = new QLabel("Function Name");
    lFunctionName->setMaximumSize(80, 10);
    lFunctionName->setMinimumSize(50, 10);
    leFunctionName = new QLineEdit();
    leFunctionName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    leFunctionName->setMinimumSize(100, 20);
    leFunctionName->setMaximumSize(100, 20);
    leFunctionName->setText("function1");

    gUpLayout->addWidget(lFunctionName);
    gUpLayout->addWidget(leFunctionName, 0, Qt::AlignLeft);

    mainLayout->addLayout(gUpLayout);

    QGridLayout *gridLayout = new QGridLayout;

    QGridLayout *gridLayoutTableMemory = new QGridLayout;

    QLabel *lMemory = new QLabel("Memory");
    lMemory->setMaximumSize(80, 10);
    lMemory->setMinimumSize(50, 10);

    gridLayoutTableMemory->addWidget(lMemory, 0, 0);

    tableViewMemory = new QTableView;
    tableViewMemory->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tableViewMemory->setMinimumSize(319, 110);
    tableViewMemory->setMaximumSize(319, 110);

    gridLayoutTableMemory->addWidget(tableViewMemory, 1, 0, 2, 2);

    gridLayout->addLayout(gridLayoutTableMemory, 0, 0, 1, 1, Qt::AlignLeft);

    QGridLayout *gridLayoutTableVariables = new QGridLayout;

    QLabel *lVariables = new QLabel("Local Variables");
    lVariables->setMaximumSize(80, 10);
    lVariables->setMinimumSize(50, 10);

    gridLayoutTableVariables->addWidget(lVariables, 0, 0);

    tableViewVariables = new QTableView;
    tableViewVariables->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tableViewVariables->setMinimumSize(319, 80);
    tableViewVariables->setMaximumSize(319, 80);

    gridLayoutTableVariables->addWidget(tableViewVariables, 1, 0, 1, 2);

    pbAdd = new QPushButton("Add");
    pbAdd->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pbAdd->setMinimumSize(50, 25);
    pbAdd->setDefault(false);
    pbAdd->setAutoDefault(false);

    gridLayoutTableVariables->addWidget(pbAdd, 2, 0, Qt::AlignLeft);

    pbRemove = new QPushButton("Remove");
    pbRemove->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pbRemove->setMinimumSize(50, 25);
    pbRemove->setAutoDefault(false);

    gridLayoutTableVariables->addWidget(pbRemove, 2, 1, Qt::AlignRight);

    gridLayout->addLayout(gridLayoutTableVariables, 0, 1, 1, 1, Qt::AlignLeft);

    //gridLayout->setColumnMinimumWidth(1, 10);
    //gridLayout->setColumnStretch(1, 0);
    //gridLayout->setColumnStretch(0, 0);
    //gridLayout->setColumnStretch(2, 0);
    //gridLayout->setSpacing(10);

    mainLayout->addLayout(gridLayout);

    graphicsView = new QGraphicsView(this);
    mainContentLayout->addWidget(graphicsView);
    mainLayout->addLayout(mainContentLayout);
    setLayout(mainLayout);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    /* Create the machine scene object */
    machineScene = new CEMachineScene(this);
    /* Set the graphics view to use the machine scene object */
    graphicsView->setScene(machineScene);

    QHBoxLayout *gLayout = new QHBoxLayout();

    lName = new QLabel("vedem");
    lName->setMaximumSize(80, 10);
    lName->setMinimumSize(50, 10);
    leName = new QLineEdit();
    leName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    leName->setMinimumSize(100, 20);
    leName->setMaximumSize(100, 20);

    mainLayout->addSpacing(10);

    gLayout->addWidget(lName);
    gLayout->addWidget(leName, 0, Qt::AlignLeft);

    mainLayout->addLayout(gLayout);

    listLayout = new QVBoxLayout();
    label = new QLabel("Assignments");
    listLayout->addWidget(label);

    //listWidget = new QListWidget();
    //listWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //listWidget->setMinimumSize(50, 10);
    //listLayout->addWidget(listWidget);

    autocompletionTextEdit = new CEAutocompletionTextEdit;
    autocompletionTextEdit->setWordWrapMode(QTextOption::NoWrap);
    autocompletionTextEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    autocompletionTextEdit->setMinimumSize(50, 10);
    //autocompletionTextEdit->setMaximumSize(500, 150);
    completer = new CETreeModelCompleter(this);
    completer->setSeparator(".");
    //completer->setModel(modelFromFileList(":/resources/wordlist.txt"));
    QStringList list;
    list<<"pooo"<<"poo";
    completer->setModel(modelFromFileTree(list, *completer));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    autocompletionTextEdit->setCompleter(completer);
    listLayout->addWidget(autocompletionTextEdit);

    //leFunctionName->setCompleter(completer);

    QHBoxLayout *h = new QHBoxLayout();

    pbCheck = new QPushButton("Check");
    pbCheck->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pbCheck->setMinimumSize(50, 25);
    h->addWidget(pbCheck, 0, Qt::AlignLeft);

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

    listLayout->addLayout(h);

    mainLayout->addLayout(listLayout);

    //pbAdd->setVisible(false);
    //pbRemove->setVisible(false);
}

QAbstractItemModel *CECodeDialog::modelFromFileTree(const QStringList &list, QCompleter &completer)
{

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif

    QStandardItemModel *model = new QStandardItemModel(&completer);
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

bool CECodeDialog::openFile()
{
    if(machineScene->isChanged() || changed)
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
        QList<CEVariableDeclared> variables;
        machineScene->readFile(fileName, variables);
        variablesDeclared->setVariables(&variables);
        return true;
    }
    return false;
    //machineScene->readFile("C:\\Documents and Settings\\Administrator\\Desktop\\Qt\\test.xml");
}

bool CECodeDialog::saveFile()
{
    if(fileName != "")
    {
        QByteArray *stream = new QByteArray();
        machineScene->saveToStream(stream, &variablesDeclared->getVariables(), leFunctionName->text(), 0);
        machineScene->arrangeGraphicsItem();
        changed = false;
        saveToFile(fileName, stream);
        return true;
    }
    else
        return saveAsFile();
}

bool CECodeDialog::saveAsFile()
{
    QString s = QFileDialog::getSaveFileName(this, "Save", "", tr("XML (*.xml)"));
    if(s != "")
    {
        fileName = s;
        QByteArray *stream = new QByteArray();
        machineScene->saveToStream(stream, &variablesDeclared->getVariables(), leFunctionName->text(), 0);
        machineScene->arrangeGraphicsItem();
        changed = false;
        saveToFile(fileName, stream);
        return true;
    }
    return false;
}

bool CECodeDialog::saveGenerateFile()
{
    if(machineScene->isChanged() || changed)
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
        QByteArray *stream = new QByteArray();
        machineScene->saveToStream(stream, &variablesDeclared->getVariables(), leFunctionName->text(), 1);
        saveToFile(s, stream);
    }
}

bool CECodeDialog::generateFile()
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
        QByteArray *stream = new QByteArray();
        machineScene->saveToStream(stream, &variablesDeclared->getVariables(), leFunctionName->text(), 1);
        saveToFile(s, stream);
    }
}

bool CECodeDialog::saveToFile(QString s, QByteArray *stream)
{
    QFile f(s);
    if(f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        f.write(stream->data());
        f.close();
    }
    return true;
}

void CECodeDialog::check_click()
{
    //QStringList identifierList;
    //char *s = "8ijik";
    //identifierList.append(QString(s));

    CEstringList *list = (CEstringList *) malloc(sizeof(CEstringList));
    char c[] = "ioi";
    char *cl;
    cl = c;
    addStringList(list, cl);
}

void CECodeDialog::saveXML_click()
{
    //if(machineScene->check())
    QByteArray *stream = new QByteArray();
    machineScene->saveToStream(stream, &variablesDeclared->getVariables(), leFunctionName->text(), 0);
    machineScene->arrangeGraphicsItem();
    saveToFile("C:\\Documents and Settings\\Administrator\\Desktop\\Qt\\test.xml", stream);
}

void CECodeDialog::saveC_click()
{
    QByteArray *stream = new QByteArray();
    machineScene->saveToStream(stream, &variablesDeclared->getVariables(), leFunctionName->text(), 1);
    saveToFile("C:\\Documents and Settings\\Administrator\\Desktop\\Qt\\test.txt", stream);
}

void CECodeDialog::readXML_click()
{
    QList<CEVariableDeclared> variables;
    machineScene->readFile("C:\\Documents and Settings\\Administrator\\Desktop\\Qt\\test.xml", variables);
    variablesDeclared->setVariables(&variables);
}

void CECodeDialog::open_click()
{
    openFile();
}

void CECodeDialog::save_click()
{
    saveFile();
}

void CECodeDialog::saveAs_click()
{
    saveAsFile();
}

void CECodeDialog::saveGenerate_click()
{
    saveGenerateFile();
}

void CECodeDialog::generate_click()
{
    generateFile();
}

void CECodeDialog::add_click()
{
    variablesDeclared->addVariable();
    changed = true;
}

void CECodeDialog::remove_click()
{
    QModelIndex index = selectionModel->currentIndex();
    if(index.isValid())
    {
        variablesDeclared->removeRow(index.row());
        selectionModel->clearSelection();
        changed = true;
    }
    pbRemove->setEnabled(false);
}

void CECodeDialog::currentRowChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    pbRemove->setEnabled(selected.indexes().count() > 0);
    //qDebug()<<"ikloe";
    variableDeclaredDelegate->setVariableNames(variablesDeclared->getNames());
}

void CECodeDialog::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    qDebug()<<"yuuuuuuuuuuuuuuuuuuuuuuuuu";
    changed = true;
    if(variableDeclaredDelegate->isBadIndex())
        tableViewVariables->edit(variableDeclaredDelegate->getBadIndex());
}

void CECodeDialog::showMainMenu()
{
    if(!(isShowSelectItem() || isShowList()))
        saveAction->setEnabled(machineScene->isChanged());
}

void CECodeDialog::textChanged(const QString &s)
{
    machineScene->setSelectItemText(s);
}
