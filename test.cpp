#include "test.h"

void Test::myFirstTest() {
    QVERIFY(1 == 1);
}

void Test::mySecondTest() {
    QVERIFY(1 != 2);
}
