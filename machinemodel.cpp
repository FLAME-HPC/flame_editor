#include <QtGui>
#include <QXMLStreamWriter>

#include "machinemodel.h"

/*MachineModel::MachineModel(QTableWidget * tt)
{
    transitionTable = tt;
}*/

int MachineModel::checkValidStartState()
{
    //qDebug() << "MachineModel checkValidStartState()";

    startState = 0; // Define startState as null
    int flag;
    // Find a unique state that does not have any incoming states
    for(int i = 0; i < states.size(); i++)
    {
        // For each state
        State * s = states.at(i);
        //qDebug() << s->name();
        flag = 0;
        for(int j = 0; j < transitions.size(); j++)
        {
            Transition * t = transitions.at(j);
            //qDebug() << "\t" << t->name() << " - " << t->nextState()->name();
            if(t->nextState() == s) flag = 1;
        }
        if(flag == 0)
        {
            if(startState == 0) startState = s;
            else return 1; // Error: More than one possible start state
        }
    }

    //qDebug() << "startState " << startState->name();

    if(startState == 0) return 2; // Error: No start state possible
    else return 0;
}

void MachineModel::addState(QString name, bool start)//StateItem *s)
{
    /*states.append(s);
    if(s->getStartState()) startState = s;*/
    //qDebug() << "machine addState " << s->getName();

    State * s = new State(name, start);
    if(start) startState = s;
    states.append(s);
}

void MachineModel::addTransition(QString cs, QString name, QString ns)
{
    //qDebug() << cs << " " << name << " " << ns;

    State * pcs = 0;
    State * pns = 0;

    this->insertRows(0, 1);

    // Search for and add states
    for(int i = 0; i < states.size(); i ++)
    {
        State * s = states.at(i);
        if(s->name() == cs) pcs = s;
        if(s->name() == ns) pns = s;
    }

    if(pcs == 0)
    {
        State * s = new State(cs);
        states.append(s);
        pcs = s;
    }
    if(pns == 0)
    {
        State * s = new State(ns);
        states.append(s);
        pns = s;
    }

    Transition * t = new Transition(pcs, name, pns);
    transitions.append(t);

    //connect(a, SIGNAL(updateMpre(Arrow*)), this, SLOT(handleUpdatedMpre(Arrow*)));

    /*transitionTable->insertRow(0);
    transitionTable->setItem(0,0,new StateTableItem(a->startItem()));//new QTableWidgetItem(a->startItem()->getName()));
    transitionTable->setItem(0,3,new QTableWidgetItem(a->getName()));
    transitionTable->setItem(0,6,new StateTableItem(a->endItem()));//new QTableWidgetItem(a->endItem()->getName()));*/

    //qDebug() << "machine addTransition " << t->name() << " " << pcs->name() << " -> " << pns->name();
}

void MachineModel::addTransitionString(QString n, QString cs, QString ns, Mpre mpre, Mpost mpost)
{
    State * pcs = 0;
    State * pns = 0;

    this->insertRows(0, 1);

    // Search for and add states
    for(int i = 0; i < states.size(); i ++)
    {
        State * s = states.at(i);
        if(s->name() == cs) pcs = s;
        if(s->name() == ns) pns = s;
    }

    if(pcs == 0)
    {
        State * s = new State(cs);
        states.append(s);
        pcs = s;
    }
    if(pns == 0)
    {
        State * s = new State(ns);
        states.append(s);
        pns = s;
    }

    Transition * t = new Transition(pcs, n, pns);
    t->setMpre(mpre);
    t->setMpost(mpost);
    transitions.append(t);
}

int MachineModel::rowCount(const QModelIndex &/*parent*/) const
 {
     return transitions.count();
 }

int MachineModel::columnCount(const QModelIndex &/*parent*/) const
 {
     return 7;
 }

QVariant MachineModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (index.row() >= transitions.size())
         return QVariant();

     if (role == Qt::DisplayRole)
     {
        if(index.column() == 0) return transitions.at(index.row())->currentState()->name();
        if(index.column() == 1) return transitions.at(index.row())->input();
        if(index.column() == 2) return qVariantFromValue(transitions.at(index.row())->mpre());
        if(index.column() == 3) return transitions.at(index.row())->name();
        if(index.column() == 4) return qVariantFromValue(transitions.at(index.row())->mpost());
        if(index.column() == 5) return transitions.at(index.row())->output();
        if(index.column() == 6) return transitions.at(index.row())->nextState()->name();
        return QVariant();
     }
     return QVariant();
 }

QVariant MachineModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const
 {
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal)
     {
         if(section == 0) return QString("Current State");//.arg(section);
         else if(section == 1) return QString("Input");
         //else if(section == 2) return QString("");
         else if(section == 2) return QString("Mpre");
         //else if(section == 4) return QString("");
         else if(section == 3) return QString("Function Name");
         else if(section == 4) return QString("Mpost");
         else if(section == 5) return QString("Output");
         else //if(section == 6)
             return QString("Next State");
     }
     else
         return QString("Row %1").arg(section);
 }

Qt::ItemFlags MachineModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return Qt::ItemIsEnabled;

     return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
 }

bool MachineModel::setData(const QModelIndex &index,
                               const QVariant &value, int role)
 {
     if (index.isValid() && role == Qt::EditRole)
    {
        if(index.column() == 0)
        {
            emit( updateStateName(transitions.at(index.row())->currentState()->name(), value.toString()) );
            transitions.at(index.row())->currentState()->setName(value.toString());
        }
        if(index.column() == 1) transitions.at(index.row())->setInput(value.toString());
        if(index.column() == 2) transitions.at(index.row())->setMpre(qVariantValue<Mpre>(value));
        if(index.column() == 3) transitions.at(index.row())->setName(value.toString());
        if(index.column() == 4) transitions.at(index.row())->setMpost(qVariantValue<Mpost>(value));
        if(index.column() == 5) transitions.at(index.row())->setOutput(value.toString());
        if(index.column() == 6)
        {
            emit( updateStateName(transitions.at(index.row())->nextState()->name(), value.toString()) );
            transitions.at(index.row())->nextState()->setName(value.toString());
        }
         emit dataChanged(index, index);
         return true;
     }
     return false;
 }

bool MachineModel::insertRows(int position, int rows, const QModelIndex &/*parent*/)
 {
     beginInsertRows(QModelIndex(), position, position+rows-1);

     for (int row = 0; row < rows; ++row)
     {
         /*stringListCurrentState.insert(position, "");
         stringListInput.insert(position, "");
         stringListMpre.insert(position, "");
         stringListFunctionName.insert(position, "");
         stringListMpost.insert(position, "");
         stringListOutput.insert(position, "");
         stringListNextState.insert(position, "");*/
     }

     endInsertRows();
     return true;
 }

bool MachineModel::removeRows(int position, int rows, const QModelIndex &/*parent*/)
 {
     beginRemoveRows(QModelIndex(), position, position+rows-1);

     for (int row = 0; row < rows; ++row) {
         /*stringList.removeAt(position);*/
     }

     endRemoveRows();
     return true;
 }

void MachineModel::setStartStateString(QString n)
{
    // Check to see if state exists
    startState = 0;

    // Check if states exist
    for(int i = 0; i < states.size(); i++)
    {
        states.at(i)->setStartState(false);
        if(states.at(i)->name() == n)
        {
            startState = states.at(i);
            states.at(i)->setStartState(true);
        }
    }
    if(startState == 0)
    {
        this->addState(n, true);
    }
}

void MachineModel::transitionUpdated(QModelIndex topLeft, QModelIndex /*bottomRight*/)
{
    //ui->label->setText(QString("transitionChanged r: %1 c: %2").arg(topLeft.row(), topLeft.column()));

    //emit( updateScene() );

    // If mpre was updated
    if(topLeft.column() == 2)
    {
        //qDebug() << "mpre updated";

        // If another branch from the same state exists
        Transition * t = transitions.at(topLeft.row());
        for(int i = 0; i < transitions.size(); i++)
        {
            Transition * t2 = transitions.at(i);
            if(t->currentState() == t2->currentState() && i != topLeft.row())
            {
                //qDebug() << "Found other branch: " << t2->name();
                t2->getMprePointer()->setNot(false);
                t2->getMprePointer()->setEnabled(t->mpre().enabled());
                t2->getMprePointer()->setName(t->mpre().name());
                if(t->mpre().op() == "==") t2->getMprePointer()->setOp("!=");
                if(t->mpre().op() == "!=") t2->getMprePointer()->setOp("==");
                if(t->mpre().op() == ">")  t2->getMprePointer()->setOp("<=");
                if(t->mpre().op() == "<")  t2->getMprePointer()->setOp(">=");
                if(t->mpre().op() == ">=") t2->getMprePointer()->setOp("<");
                if(t->mpre().op() == "<=") t2->getMprePointer()->setOp(">");
                t2->getMprePointer()->setValue(t->mpre().value());
            }
        }
    }
}
