#include <QtGui/QApplication>
#include <QWidget>
#include "codedialog.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CodeDialog cd;
    cd.exec();

    
    return 0;
}
