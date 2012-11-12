#include "assignmentdialog.h"
#include <QRegExp>

#include <QRegExpValidator>
#include <QFile>
#include <QStringListModel>
#include <QApplication>
#include <QStandardItemModel>

extern "C" {

   int textToParse(char *p);

}

AssignmentDialog::AssignmentDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    setUI();

    setWindowTitle("Statement Edit");

    connect(pbOk, SIGNAL(clicked()), this, SLOT(ok_click()));

    connect(pbCancel, SIGNAL(clicked()), this, SLOT(cancel_click()));

/*
    QRegExp re("\\d{1,2}(\\,\\d{1,2})?");
    QRegExpValidator *v = new QRegExpValidator(re, comboBox1);

    comboBox1->addItem("uiu");
    comboBox1->addItem("uiu");
    comboBox1->setCurrentIndex(-1);
    comboBox1->setEditable(true);
    comboBox1->setValidator(v);
*/
}

void AssignmentDialog::setUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    QVBoxLayout *assignmentLayout = new QVBoxLayout();
    lAssignment = new QLabel("Statement");
    lAssignment->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    lAssignment->setMinimumSize(550, 10);
    assignmentLayout->addWidget(lAssignment, 0, Qt::AlignTop);


    autocompletionTextEdit = new AutocompletionTextEdit;
    autocompletionTextEdit->setWordWrapMode(QTextOption::NoWrap);
    autocompletionTextEdit->setMaximumSize(500, 150);
    completer = new TreeModelCompleter(this);
    completer->setSeparator(".");
    //completer->setModel(modelFromFileList(":/resources/wordlist.txt"));
    completer->setModel(modelFromFileTree(":/resources/treemodel.txt"));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    autocompletionTextEdit->setCompleter(completer);
    assignmentLayout->addWidget(autocompletionTextEdit);

    QVBoxLayout *buttonsLayou = new QVBoxLayout();

    pbOk = new QPushButton("Ok");
    pbOk->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pbOk->setMinimumSize(80, 25);
    pbOk->setMaximumSize(80, 25);
    buttonsLayou->addWidget(pbOk);

    pbCancel = new QPushButton("Cancel");
    pbCancel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pbCancel->setMinimumSize(80, 25);
    pbCancel->setMaximumSize(80, 25);
    buttonsLayou->addWidget(pbCancel, 0, Qt::AlignTop);

    QHBoxLayout *c = new QHBoxLayout();

    c->addLayout(assignmentLayout);
    c->addLayout(buttonsLayou);

    mainLayout->addLayout(c);

}

QAbstractItemModel *AssignmentDialog::modelFromFileList(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(completer);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    QStringList words;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << line.trimmed();
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return new QStringListModel(words, completer);
}

QAbstractItemModel *AssignmentDialog::modelFromFileTree(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(completer);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    QStringList words;

    QStandardItemModel *model = new QStandardItemModel(completer);
    QVector<QStandardItem *> parents(10);
    parents[0] = model->invisibleRootItem();

    while (!file.atEnd()) {
        QString line = file.readLine();
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

void AssignmentDialog::ok_click()
{
    QString s = autocompletionTextEdit->toPlainText();
    int i = textToParse(s.toLocal8Bit().data());
    if(i == 0){
        assignmentText = s;
        accept();
    }
    setWindowTitle(QString("%1 -> %2").arg(s).arg(i));
}

void AssignmentDialog::cancel_click()
{
    reject();
}

AssignmentDialog::~AssignmentDialog()
{
    delete lAssignment;
    delete pbOk;
    delete pbCancel;
    delete autocompletionTextEdit;
}
