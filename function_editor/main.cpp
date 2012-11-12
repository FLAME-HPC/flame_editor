#include <QtGui/QApplication>
#include <QWidget>
#include "codedialog.h"
#include "assignmentdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CodeDialog cd;
    cd.exec();

    //AssignmentDialog ad;
    //ad.exec();

    
    return 0;
}
