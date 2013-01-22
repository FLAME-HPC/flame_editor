#include "cevariabledeclareddelegate.h"
#include <QAbstractItemView>
#include <QMessageBox>
#include "./codedialog.h"
#include "./celineedit.h"
#include "./cetreemodelcompleter.h"
#include "./cecodeparser.h"
#include "./cevariabledeclaredmodel.h"

CEVariableDeclaredDelegate::CEVariableDeclaredDelegate(QObject *parent)
{
    memoryVariableNames = 0;
    variableNames = 0;
    badIndex = 0;
}

QWidget *CEVariableDeclaredDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
    badIndex = 0;
    editMode = true;
    if(index.column() == 0)
    {
        QComboBox *editor = new QComboBox(parent);
        editor->insertItem(0, "int");
        editor->insertItem(1, "double");

        return editor;
    }
    else if(index.column() == 1)
    {
        QLineEdit *editor = new QLineEdit(parent);
        return editor;
    }
    else if(index.column() == 2)
    {
        CELineEdit *editor = new CELineEdit(parent);
        CETreeModelCompleter *completer;
        completer = new CETreeModelCompleter(editor);
        completer->setSeparator(".");
        QStringList list(*memoryVariableNames);
        qDebug()<<index.row();
        int m = index.row() < variableNames->count()?index.row():variableNames->count();
        for(int i = 0;i < m;i++)
            list.append(variableNames->at(i));
        //completer->setModel(CodeDialog::modelFromFileTree(list, *completer));
        completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setWrapAround(false);
        editor->setCompleter(completer);
        return editor;
    }
    else return QItemDelegate::createEditor(parent, option, index);
}

void CEVariableDeclaredDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    if(index.column() == 0)
    {
        QString value = index.data().toString();

        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        for(int i = 0; i < comboBox->count(); i++)
        {
            if(comboBox->itemText(i) == value)
                comboBox->setCurrentIndex(i);
        }
    }
    else if(index.column() == 1)
    {
        QString value = index.data().toString();
        QLineEdit * lineEdit = static_cast<QLineEdit*>(editor);
        lineEdit->setText(value);
    }
    else if(index.column() == 2)
    {
        QString value = index.data().toString();

        QLineEdit * lineEdit = static_cast<QLineEdit*>(editor);
        lineEdit->setText(value);
    }
    else
    {
        QItemDelegate::setEditorData(editor, index);
    }
}

void CEVariableDeclaredDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    editMode = false;
    if(index.column() == 0)
    {
            QComboBox *comboBox = static_cast<QComboBox*>(editor);
            QString value = comboBox->currentText();

            model->setData(index, value, Qt::EditRole);
    }
    else if(index.column() == 1)
    {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString value = lineEdit->text();

        QStringList list(*memoryVariableNames);
        qDebug()<<index.row();
        for(int i = 0;i < variableNames->count();i++)
            if(i != index.row())
                list.append(variableNames->at(i));
        if(list.contains(value, Qt::CaseInsensitive))
        {
            QString messageText = tr("Variable '%1' is already declared!").arg(value);
            QMessageBox::warning(qobject_cast<QWidget*>( this->parent()), tr(""), messageText);
        }
        else
            model->setData(index, value, Qt::EditRole);
    }
    else if(index.column() == 2)
    {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString value = lineEdit->text();

        if(value == "")
            model->setData(index, value, Qt::EditRole);
        else
        {
            int i = CECodeParser::setParseToTest(value);
            if(i!=0){
                QString es(CECodeParser::getToken());
                QString sm(value);
                sm = sm.insert(i-1, "<b>");
                sm.append("</b>");
                QString messageText = tr("Incorrect token in text appears in at the beginning bold part ") + "\"" + sm + "\"";
                badIndex = new QModelIndex(index);
                model->setData(index, value, Qt::EditRole);
                QMessageBox::warning(qobject_cast<QWidget*>( this->parent()), tr(""), messageText);
            }
            else
                model->setData(index, value, Qt::EditRole);
        }
    }
    else
    {
        QItemDelegate::setModelData(editor, model, index);
    }
}

void CEVariableDeclaredDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}
