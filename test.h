#ifndef TEST_H
#define TEST_H
#include <QtTest>

class Test: public QObject
{
    Q_OBJECT
private slots:
    void myFirstTest();
    void mySecondTest();
};

#endif // TEST_H
