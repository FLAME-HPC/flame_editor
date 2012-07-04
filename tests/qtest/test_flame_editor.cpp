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
    void open_test_model();
    void change_in_machine_tree_1();
    void change_in_memory_table_1();
    void change_in_graphics_view_1();
    void close_test_model();
    void change_in_machine_tree_2();
    void change_in_memory_table_2();
    void change_in_graphics_view_2();

  private:
    MainWindow w;
    bool saveImages;
    QPixmap blank_machine_tree;
    QPixmap blank_memory_table;
    QPixmap blank_graphics_view;
    QPixmap getMachineTreeImage();
    QPixmap getMemoryTableImage();
    QPixmap getGraphicsViewImage();
};

void TestEditor::initTestCase() {
    saveImages = true;

    /* Show main window */
    w.show();
    /* Save screen shots of blank widgets */
    blank_machine_tree = getMachineTreeImage();
    if (saveImages) blank_machine_tree.save("machine_tree_0.png", "png");
    blank_memory_table = getMemoryTableImage();
    if (saveImages) blank_memory_table.save("memory_table_0.png", "png");
    blank_graphics_view = getGraphicsViewImage();
    if (saveImages) blank_graphics_view.save("graphics_view_0.png", "png");
}

void TestEditor::cleanupTestCase() {
    /* Close main window */
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

void TestEditor::open_test_model() {
    int rc = w.openModel_internal("../models/conditions.xml", true);
    QVERIFY(rc == 0);
}

void TestEditor::change_in_machine_tree_1() {
    QPixmap updated_tree = getMachineTreeImage();
    /* Save screen shot of widget */
    if (saveImages) updated_tree.save("machine_tree_1.png", "png");
    /* Verify widget has changed from inital image */
    QVERIFY(blank_machine_tree.toImage() != updated_tree.toImage());
}

void TestEditor::change_in_machine_tree_2() {
    QPixmap updated_tree = getMachineTreeImage();
    /* Save screen shot of widget */
    if (saveImages) updated_tree.save("machine_tree_2.png", "png");
    /* Verify widget has changed from inital image */
    QVERIFY(blank_machine_tree.toImage() == updated_tree.toImage());
}

void TestEditor::change_in_graphics_view_1() {
    QPixmap updated_graphics = getGraphicsViewImage();
    /* Save screen shot of widget */
    if (saveImages) updated_graphics.save("graphics_view_1.png", "png");
    /* Verify widget has changed from inital image */
    QVERIFY(blank_graphics_view.toImage() != updated_graphics.toImage());
}

void TestEditor::change_in_graphics_view_2() {
    QPixmap updated_graphics = getGraphicsViewImage();
    /* Save screen shot of widget */
    if (saveImages) updated_graphics.save("graphics_view_2.png", "png");
    /* Verify widget has changed from inital image */
    QVERIFY(blank_graphics_view.toImage() == updated_graphics.toImage());
}

void TestEditor::change_in_memory_table_1() {
    QPixmap updated_table = getMemoryTableImage();
    /* Save screen shot of widget */
    if (saveImages) updated_table.save("memory_table_1.png", "png");
    /* Verify widget has changed from inital image */
    QVERIFY(blank_memory_table.toImage() != updated_table.toImage());
}

void TestEditor::change_in_memory_table_2() {
    QPixmap updated_table = getMemoryTableImage();
    /* Save screen shot of widget */
    if (saveImages) updated_table.save("memory_table_2.png", "png");
    /* Verify widget has changed from inital image */
    QVERIFY(blank_memory_table.toImage() == updated_table.toImage());
}

void TestEditor::close_test_model() {
    /* Opened model should still be selected */
    int rc = w.closeModel_internal();
    QVERIFY(rc == 0);
}

QPixmap TestEditor::getMachineTreeImage() {
    /* Grab image of machine tree widget minus top 20px and
       bottom 20px so that header and slider positions don't
       matter. */
    return QPixmap::grabWidget(w.getMachineTreeWidget(),
        0, 20, -1, w.getMachineTreeWidget()->height()-40);
}

QPixmap TestEditor::getMemoryTableImage() {
    return QPixmap::grabWidget(w.getMemoryTableWidget(),
        0, 22, -1, w.getMemoryTableWidget()->height()-42);
}

QPixmap TestEditor::getGraphicsViewImage() {
    return QPixmap::grabWidget(w.getGraphicsViewWidget(),
        0, 1, -1, w.getGraphicsViewWidget()->height()-1);
}

QTEST_MAIN(TestEditor)
#include "test_flame_editor.moc"
