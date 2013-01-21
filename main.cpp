#include <QtGui/QApplication>
#include <QWidget>
#include "cecodedialog.h"
#include "ceassignmentdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CECodeDialog cd;
    cd.exec();

    //AssignmentDialog ad;
    //ad.exec();

    
    return 0;
}
