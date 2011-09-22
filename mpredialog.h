#ifndef MPREDIALOG_H
#define MPREDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include "mpre.h"
#include "memorymodel.h"

class QDialogButtonBox;

class MpreDialog : public QDialog
{
    Q_OBJECT

public:
    MpreDialog(MemoryModel * m, QWidget *parent = 0);
    void setMpre(Mpre m);
    Mpre getMpre();

signals:
    void setVariableComboBox(int i);
    void setOpComboBox(int i);

private:
    QDialogButtonBox *buttonBox;
    QComboBox *variable;
    QComboBox * op;
    QSpinBox * value;
    QCheckBox * myEnabled;
    QGroupBox *lhsGroup;
    QGroupBox *opGroup;
    QGroupBox *rhsGroup;
    Mpre mpre;
    MemoryModel * memory;
    QStringList operators;
};

#endif // MPREDIALOG_H
