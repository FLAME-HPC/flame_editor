#ifndef VARIABLEDECLAREDDELEGATE_H
#define VARIABLEDECLAREDDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTableView>

class VariableDeclaredDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    VariableDeclaredDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool isBadIndex() const { return badIndex != 0;}
    QModelIndex getBadIndex() const { return *badIndex;}

    void setMemoryVariableNames(QStringList s)
    {
        if(memoryVariableNames != 0)
        {
            memoryVariableNames->clear();
            memoryVariableNames = 0;
        }
        memoryVariableNames = new QStringList(s);
    }
    void setVariableNames(QStringList s)
    {
        if(variableNames != 0)
        {
            variableNames->clear();
            variableNames = 0;
        }
        variableNames = new QStringList(s);
    }
    bool isEditMode() const { return editMode;}
private:
    mutable QModelIndex *badIndex;
    QStringList *memoryVariableNames;
    mutable QStringList *variableNames;
    mutable bool editMode;
};

#endif // VARIABLEDECLAREDDELEGATE_H
