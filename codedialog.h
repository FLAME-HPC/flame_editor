#ifndef CODEDIALOG_H
#define CODEDIALOG_H

#include <QDialog>
#include "./machinescene.h"
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>

class CodeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CodeDialog(QWidget *parent = 0);
    ~CodeDialog();

    bool getShowList() const{
        return showList;
    }

    void setShowList(bool b){
        showList = b;
        label->setVisible(b);
        listWidget->setVisible(b);
        pbAdd->setVisible(b);
        pbRemove->setVisible(b);
    }
    
private:
    void setUI();


    QGraphicsView *graphicsView;
    MachineScene *machineScene;
    QVBoxLayout *mainLayout;
    QVBoxLayout *mainContentLayout;
    QVBoxLayout *listLayout;

    QListWidget *listWidget;

    QLabel *label;
    QPushButton *pbAdd;
    QPushButton *pbRemove;
    bool showList;

private slots:
    void selectListItem();
    void add_click();
    void removeItemList_click();
};

#endif // CODEDIALOG_H
