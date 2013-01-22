#ifndef CODEDIALOG_H
#define CODEDIALOG_H

#include <QDialog>
#include "./cemachinescene.h"
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCloseEvent>
#include <QTableView>
#include <QModelIndex>
#include <QItemSelectionModel>
#include "ceautocompletiontextedit.h"
#include "cetreemodelcompleter.h"
#include "./cevariabledeclaredmodel.h"
#include "./cevariabledeclareddelegate.h"
#include "ui_codedialog.h"
#include "./cegraphicsitem.h"
#include "../memorymodel.h"
#include "../mpost.h"

//#define debug_onn

class CodeDialog : public QDialog, public Ui::CodeDialog
{
    Q_OBJECT
    
public:
    explicit CodeDialog(MemoryModel * m, QWidget *parent = 0);
    ~CodeDialog();
    void setMpost(Mpost c);
    Mpost getMpost();
    void setName(QString n);
    QString getName();

protected:
    void hideEvent(QHideEvent *);
    void closeEvent(QCloseEvent *e);
    bool eventFilter(QObject *o, QEvent *e);

private:
    CEMachineScene *machineScene;
    MemoryModel *agentMemory;
    QItemSelectionModel *selectionModel;
    CEVariableDeclaredModel *variablesDeclared;
    Mpost mpost;

public slots:
    void done(int r);
    void functionCodeDialog(CEGraphicsItem* item);

private slots:
    void add_click();
    void remove_click();
    void currentRowChanged (const QItemSelection & selected, const QItemSelection );
    void commitAndCloseEditor();
    void on_checkBoxEnableEditor_clicked(bool checked);
    void initialise();
};

#endif // CODEDIALOG_H
