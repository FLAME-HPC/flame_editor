QT_464    = /usr/bin/qmake-4.6
QT_474    = /Applications/QtSDK/Desktop/Qt/474/gcc/bin/qmake
QT_481    = /Applications/QtSDK/Desktop/Qt/4.8.1/gcc/bin/qmake
QMAKE_PAR = -spec macx-g++ test_flame_editor.pro
SOURCES   = qtest/test_flame_editor.cpp \
            ../mainwindow.cpp \
            ../machinescene.cpp \
            ../arrow.cpp \
            ../machinemodel.cpp \
            ../memorydelegate.cpp \
            ../memorymodel.cpp \
            ../mpre.cpp \
            ../simulationthread.cpp \
            ../mpredelegate.cpp \
            ../mpredialog.cpp \
            ../mpost.cpp \
            ../mpostdelegate.cpp \
            ../mpostdialog.cpp \
            ../adt.cpp \
            ../memoryvariable.cpp \
            ../modelxmlreader.cpp \
            ../machine.cpp \
            ../transition.cpp \
            ../state.cpp \
            ../variable.cpp \
            ../statedelegate.cpp \
            ../machinetree.cpp \
            ../condition.cpp \
            ../timeunit.cpp \
            ../communication.cpp \
            ../commdelegate.cpp \
            ../commdialog.cpp \
            ../messagecomm.cpp \
            ../messagemodel.cpp \
            ../messagesort.cpp \
            ../messagedelegate.cpp \
            ../sortdelegate.cpp \
            ../sortdialog.cpp \
            ../datatypedelegate.cpp \
            ../lineeditdelegate.cpp \
            ../machinetreedelegate.cpp \
            ../timeunitdialog.cpp \
            ../texteditdelegate.cpp \
            ../graphicsitem.cpp
HEADERS   = ../mainwindow.h \
            ../machinescene.h \
            ../arrow.h \
            ../machinemodel.h \
            ../memorydelegate.h \
            ../memorymodel.h \
            ../mpre.h \
            ../simulationthread.h \
            ../mpredelegate.h \
            ../mpredialog.h \
            ../mpost.h \
            ../mpostdelegate.h \
            ../mpostdialog.h \
            ../adt.h \
            ../memoryvariable.h \
            ../modelxmlreader.h \
            ../machine.h \
            ../transition.h \
            ../state.h \
            ../variable.h \
            ../statedelegate.h \
            ../machinetree.h \
            ../condition.h \
            ../timeunit.h \
            ../communication.h \
            ../commdelegate.h \
            ../commdialog.h \
            ../messagecomm.h \
            ../messagemodel.h \
            ../messagesort.h \
            ../messagedelegate.h \
            ../sortdelegate.h \
            ../sortdialog.h \
            ../datatypedelegate.h \
            ../lineeditdelegate.h \
            ../machinetreedelegate.h \
            ../timeunitdialog.h \
            ../texteditdelegate.h \
            ../graphicsitem.h
EXECUTABLE = test_flame_editor.app/Contents/MacOS/test_flame_editor
CPPLINT    = /Users/stc/workspace/flame/flame_visualiser/tags/3/tests/ccplint/cpplint.py
lint:
	$(CPPLINT) $(SOURCES) $(HEADERS)

4.8:
	cd qtest; \
	echo "qmake ..."; \
	$(QT_481) $(QMAKE_PAR); \
	echo "make ..."; \
	make > /dev/null; \
	./$(EXECUTABLE)

4.7:
	cd qtest; \
	echo "qmake ..."; \
	$(QT_474) $(QMAKE_PAR); \
	echo "make ..."; \
	make > /dev/null; \
	./$(EXECUTABLE)

clean:
	cd qtest; \
	echo "clean up ..."; \
	make distclean > /dev/null

# dsymutil - only relevent on Mac OS X
memtest: 4.8
	cd qtest; \
	valgrind --dsymutil=yes --leak-check=full ./$(EXECUTABLE); \
	rm -fr $(EXECUTABLE).dSYM
	