/*!
 * \file machinetreedelegate.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of the model tree delegate
 */
#include "./machinetreedelegate.h"
#include "./machine.h"
#include "./timeunitdialog.h"

MachineTreeDelegate::MachineTreeDelegate(QObject * parent)
    : QItemDelegate(parent) {
}

QWidget *MachineTreeDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/*option*/,
    const QModelIndex &index) const {
    Machine * machine = static_cast<Machine*>(index.internalPointer());

    if (machine->type == 0 || machine->type == 1 || machine->type == 2
            || machine->type == 5) {
        QLineEdit *editor = new QLineEdit(parent);
        return editor;
    } else if (machine->type == 6) {  // functionFile
        QFileDialog *editor = new QFileDialog(parent, Qt::Dialog);
        editor->setModal(true);
        return editor;
    } else if (machine->type == 4) {  // timeUnit
        TimeUnitDialog *editor = new TimeUnitDialog(machine);
        connect(editor, SIGNAL(accepted()), this, SLOT(commitAndCloseEditor()));
        connect(editor, SIGNAL(rejected()), this, SLOT(commitAndCloseEditor()));
        editor->setModal(true);
        return editor;
    }

    return 0;
}

void MachineTreeDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const {
    Machine * machine = static_cast<Machine*>(index.internalPointer());

    if (machine->type == 0 || machine->type == 1 || machine->type == 2
            || machine->type == 5) {
        QString value = index.data().toString();

        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        lineEdit->setText(value);
    } else if (machine->type == 6) {  // functionFile
        QFileDialog *fileDialog = static_cast<QFileDialog*>(editor);
        if (machine->fileDirectory == "" || machine->fileName == "") {
            fileDialog->setDirectory(machine->rootModel()->fileDirectory);
        } else {
            fileDialog->setDirectory(machine->fileDirectory);
            fileDialog->selectFile(machine->fileName);
        }
    } else if (machine->type == 4) {  // timeUnit
        TimeUnit tu = machine->timeUnit();
        TimeUnitDialog *dialog = static_cast<TimeUnitDialog*>(editor);
        dialog->setTimeUnit(tu);
    }
}

void MachineTreeDelegate::setModelData(QWidget *editor,
        QAbstractItemModel *model, const QModelIndex &index) const {
    Machine * machine = static_cast<Machine*>(index.internalPointer());

    if (machine->type == 0 || machine->type == 1 || machine->type == 2 ||
            machine->type == 5) {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString value = lineEdit->text();
        model->setData(index, value, Qt::EditRole);
    } else if (machine->type == 6) {  // functionFile
        QFileDialog *fileDialog = static_cast<QFileDialog*>(editor);
        QStringList valueList = fileDialog->selectedFiles();

        if (valueList.size() > 0) {
            QDir dir(machine->rootModel()->fileDirectory);
            QString s1 = dir.canonicalPath();
            QDir dir2(s1);
            QString s = dir2.relativeFilePath(valueList.at(0));
            QFileInfo finfo(valueList.at(0));
            machine->fileDirectory = finfo.absoluteFilePath();
            machine->fileName = finfo.fileName();
            model->setData(index, s, Qt::EditRole);
        }
    } else if (machine->type == 4) {  // timeUnit
        TimeUnitDialog *dialog = static_cast<TimeUnitDialog*>(editor);
        model->setData(index, qVariantFromValue(dialog->getTimeUnit()));
    }
}

void MachineTreeDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const {
    editor->setGeometry(option.rect);
}

void MachineTreeDelegate::commitAndCloseEditor() {
    TimeUnitDialog *editor = qobject_cast<TimeUnitDialog *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
