/*!
 * \file test_flame_editor.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Test suite for flame editor
 */
#include <QtTest/QtTest>
#include <QtGui/QApplication>
#include "../../mainwindow.h"

class TestEditor: public QObject {
    Q_OBJECT

  private slots:
    void initTestCase();
    void cleanupTestCase();
    void open_missing_model();
    void open_malformed_model();
    void open_missing_submodel();
    void open_malformed_submodel();

  private:
    MainWindow w;
};

void TestEditor::initTestCase() {
    w.show();
}

void TestEditor::cleanupTestCase() {
    w.close();
}

void TestEditor::open_missing_model() {
    int rc = w.openModel_internal("../models/missing.xml", true);
    QVERIFY(rc == 1);
}

void TestEditor::open_malformed_model() {
    int rc = w.openModel_internal("../models/malformed_xml.xml", true);
    QVERIFY(rc == 2);
}

void TestEditor::open_missing_submodel() {
    int rc = w.openModel_internal("../models/missing_submodel.xml", true);
    QVERIFY(rc == 2);
}

void TestEditor::open_malformed_submodel() {
    int rc = w.openModel_internal("../models/malformed_submodel.xml", true);
    QVERIFY(rc == 2);
}

QTEST_MAIN(TestEditor)
#include "test_flame_editor.moc"
