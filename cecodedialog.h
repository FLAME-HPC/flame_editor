#ifndef CECODEDIALOG_H
#define CECODEDIALOG_H

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
#include "./cememorymodel.h"
#include "./cevariabledeclaredmodel.h"
#include "./cevariabledeclareddelegate.h"

#define debug_onn

class CECodeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CECodeDialog(QWidget *parent = 0);
    ~CECodeDialog();

    bool getShowList() const{
        return showList;
    }

    bool isShowList() const { return autocompletionTextEdit->isVisible(); }
    bool isCorrect() const { return autocompletionTextEdit->isCorrect(); }
    bool isShowSelectItem() const { return leName->isVisible(); }

    void setShowList(bool b){
        showList = b;
        label->setVisible(b);
        autocompletionTextEdit->setVisible(b);
        pbCheck->setVisible(b);
        //pbRemove->setVisible(b);
        if(b){
            autocompletionTextEdit->setFocus();
            autocompletionTextEdit->setType(machineScene->itemType());
            QStringList *list = machineScene->getAssignmentList();
            if(list != 0){
                autocompletionTextEdit->setAssignments(*list);
            }
        }
        else {
            QStringList *list = machineScene->getAssignmentList();
            if(list != 0)
                list->clear();
            QStringList l = autocompletionTextEdit->getAssignments();
            for(int i=0;i<l.count();i++)
                list->append(QString(l[i]));
            if(list !=0 && list->count() == 0 && (machineScene->itemType() == ConditionWhile || machineScene->itemType() == ConditionFor))
                machineScene->changeCondition(ConditionWhileFor);
        }
    }

    bool getShowSelectItem() const{
        return showSelectItem;
    }

    void setShowSelectItem(bool b){
        showSelectItem = b;
        lName->setVisible(b);
        leName->setVisible(b);
        if(b && autocompletionTextEdit->isVisible() == false)
            leName->setFocus();

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

    void setSelectItem(QString s, QString t){
        lName->setText(s);
        leName->setText(t);
    }

    void setFunctionName(QString s) { leFunctionName->setText(s);}

    static QAbstractItemModel *modelFromFileTree(const QStringList& fileName, QCompleter& completer);

    CEVariableDeclaredModel* getModel() const { return variablesDeclared;}

protected:
    void hideEvent(QHideEvent *e);
    void closeEvent(QCloseEvent *e);
    bool eventFilter(QObject *o, QEvent *e);

private:
    void setUI();

    QGraphicsView *graphicsView;
    CEMachineScene *machineScene;
    QVBoxLayout *mainLayout;
    QVBoxLayout *mainContentLayout;
    QVBoxLayout *listLayout;

    CEAutocompletionTextEdit *autocompletionTextEdit;
    CETreeModelCompleter *completer;

    QTableView *tableViewMemory;
    CEMemoryModel *memory;
    QTableView *tableViewVariables;
    CEVariableDeclaredDelegate *variableDeclaredDelegate;
    QItemSelectionModel *selectionModel;
    CEVariableDeclaredModel *variablesDeclared;
    QPushButton *pbAdd;
    QPushButton *pbRemove;

    QLabel *lFunctionName;
    QLineEdit *leFunctionName;
    QLabel *lName;
    QLineEdit *leName;
    QLabel *label;
    QPushButton *pbCheck;
#ifdef debug_on
    QPushButton *pbSaveXML;
    QPushButton *pbSaveC;
    QPushButton *pbReadXML;
#endif
    bool showList;
    bool showSelectItem;

    bool isAfterShowMessageBox;

    bool changed;

    QMenuBar *menuBar;

    QMenu *mainMenu;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *saveGenerateAction;
    QAction *generateAction;

    QString fileName;

    bool openFile();
    bool saveFile();
    bool saveAsFile();
    bool saveGenerateFile();
    bool generateFile();

    bool saveToFile(QString s,QByteArray *stream);

public slots:
    void done(int r);


private slots:
    void check_click();
    void saveXML_click();
    void saveC_click();
    void readXML_click();

    void open_click();
    void save_click();
    void saveAs_click();
    void saveGenerate_click();
    void generate_click();

    void add_click();
    void remove_click();
    void currentRowChanged ( const QItemSelection & selected, const QItemSelection & deselected );
    void closeEditor ( QWidget * editor, QAbstractItemDelegate::EndEditHint hint );

    void showMainMenu();

    void textChanged(const QString &s);
};

#endif // CODEDIALOG_H
