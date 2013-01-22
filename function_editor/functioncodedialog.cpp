#include <QtGui>
#include "functioncodedialog.h"

FunctionCodeDialog::FunctionCodeDialog(CEGraphicsItem *i, QStringList variableNames, QWidget *parent) : QDialog(parent), item_(i)
{
    setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowTitleHint);

    lineEdit->setText(item_->getName());
    if(item_->mytype == State || item_->mytype == FinalState)
        label->setText("State name");
    if(item_->mytype == Transition) {
        label->setText("Transition name");
        //cd->setShowList(true);
    }
    if(item_->mytype == ConditionIf || item_->mytype == ConditionWhile || item_->mytype == ConditionFor) {
        label->setText("Condition name");
        //cd->setShowList(true);
    }

    connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(nameChanged(QString)));

    autocompletionTextEdit->setWordWrapMode(QTextOption::NoWrap);
    completer = new CETreeModelCompleter(this);
    completer->setSeparator(".");
//    completer->setModel(modelFromFileList(":/resources/wordlist.txt"));
    completer->setModel(modelFromFileTree(variableNames));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    autocompletionTextEdit->setCompleter(completer);
}

void FunctionCodeDialog::nameChanged(const QString &s)
{
    item_->setName(s);
}

QAbstractItemModel* FunctionCodeDialog::modelFromFileTree(const QStringList &list)
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

void FunctionCodeDialog::on_pushButton_clicked()
{
    qDebug() << autocompletionTextEdit->isCorrect();
}
