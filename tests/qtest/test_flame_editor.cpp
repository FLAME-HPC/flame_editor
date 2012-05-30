#include <QtTest/QtTest>
#include <QtGui/QApplication>
#include "../../mainwindow.h"

class TestEditor: public QObject {
    Q_OBJECT

  private slots:
    void initTestCase();
    void cleanupTestCase();
    void toUpper();

  private:
    MainWindow w;
};

void TestEditor::initTestCase() {

    //QApplication a();
//    MainWindow w;
    w.show();
    //a.exec();
}

void TestEditor::cleanupTestCase() {
    w.close();
}

void TestEditor::toUpper() {
    w.openModel();
    //QVERIFY(str.toUpper() == "HELLO");
}

QTEST_MAIN(TestEditor)
#include "test_flame_editor.moc"
