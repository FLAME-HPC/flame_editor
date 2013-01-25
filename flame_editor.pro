# -------------------------------------------------
# Project created by QtCreator 2009-05-22T10:29:59
# -------------------------------------------------

TEMPLATE = app

QT       += xml

macx {
    TARGET = "FLAME Editor"
} else {
    TARGET = flame_editor
}
macx:ICON = flame_icon_e.icns
win32:RC_FILE = flame-e.rc

SOURCES += main.cpp \
    mainwindow.cpp \
    machinescene.cpp \
    arrow.cpp \
    machinemodel.cpp \
    memorydelegate.cpp \
    memorymodel.cpp \
    mpre.cpp \
    simulationthread.cpp \
    mpredelegate.cpp \
    mpredialog.cpp \
    mpost.cpp \
    mpostdelegate.cpp \
    mpostdialog.cpp \
    adt.cpp \
    memoryvariable.cpp \
    modelxmlreader.cpp \
    machine.cpp \
    transition.cpp \
    state.cpp \
    variable.cpp \
    statedelegate.cpp \
    machinetree.cpp \
    condition.cpp \
    timeunit.cpp \
    communication.cpp \
    commdelegate.cpp \
    commdialog.cpp \
    messagecomm.cpp \
    messagemodel.cpp \
    messagesort.cpp \
    messagedelegate.cpp \
    sortdelegate.cpp \
    sortdialog.cpp \
    datatypedelegate.cpp \
    lineeditdelegate.cpp \
    machinetreedelegate.cpp \
    timeunitdialog.cpp \
    texteditdelegate.cpp \
    graphicsitem.cpp \
    modelxmlwriter.cpp \
    calculatelayers.cpp \
    function_editor/ceassignmentdialog.cpp \
    function_editor/cemachinescene.cpp \
    function_editor/cegraphicsitem.cpp \
    function_editor/cearrow.cpp \
    function_editor/codedialog.cpp \
    function_editor/ceautocompletiontextedit.cpp \
    function_editor/parser/y.tab.c \
    function_editor/parser/lex.yy.c \
    function_editor/cetreemodelcompleter.cpp \
    function_editor/celinkedlist.cpp \
    function_editor/cecodeparser.cpp \
    function_editor/cegraphicsarcitem.cpp \
    function_editor/cegraphicsgrup.cpp \
    function_editor/cexmlfile.cpp \
    function_editor/cefiletype.cpp \
    function_editor/cecfile.cpp \
    function_editor/cevariabledeclared.cpp \
    function_editor/cevariabledeclaredmodel.cpp \
    function_editor/cevariabledeclareddelegate.cpp \
    function_editor/functioncodedialog.cpp \
    function_editor/celineedit.cpp \
    function_editor/cevariable.cpp \
    statedialog.cpp \
    messagedialog.cpp
HEADERS += mainwindow.h \
    machinescene.h \
    arrow.h \
    machinemodel.h \
    memorydelegate.h \
    memorymodel.h \
    mpre.h \
    simulationthread.h \
    mpredelegate.h \
    mpredialog.h \
    mpost.h \
    mpostdelegate.h \
    mpostdialog.h \
    adt.h \
    memoryvariable.h \
    modelxmlreader.h \
    machine.h \
    transition.h \
    state.h \
    variable.h \
    statedelegate.h \
    machinetree.h \
    condition.h \
    timeunit.h \
    communication.h \
    commdelegate.h \
    commdialog.h \
    messagecomm.h \
    messagemodel.h \
    messagesort.h \
    messagedelegate.h \
    sortdelegate.h \
    sortdialog.h \
    datatypedelegate.h \
    lineeditdelegate.h \
    machinetreedelegate.h \
    timeunitdialog.h \
    texteditdelegate.h \
    graphicsitem.h \
    function_editor/cemachinescene.h \
    function_editor/cearrow.h \
    function_editor/codedialog.h \
    function_editor/ceassignmentdialog.h \
    function_editor/ceautocompletiontextedit.h \
    function_editor/parser/y.tab.h \
    function_editor/cetreemodelcompleter.h \
    function_editor/celinkedlist.h \
    function_editor/cecodeparser.h \
    function_editor/cegraphicsarcitem.h \
    function_editor/cegraphicsgrup.h \
    function_editor/cegraphicsitem.h \
    function_editor/celineedit.h \
    function_editor/cexmlfile.h \
    function_editor/cefiletype.h \
    function_editor/cecfile.h \
    function_editor/cevariabledeclared.h \
    function_editor/cevariabledeclaredmodel.h \
    function_editor/cevariabledeclareddelegate.h \
    function_editor/functioncodedialog.h \
    function_editor/cevariable.h \
    statedialog.h \
    messagedialog.h
FORMS += mainwindow.ui \
    conditiondialog.ui \
    commdialog.ui \
    sortdialog.ui \
    timeunitdialog.ui \
    function_editor/codedialog.ui \
    function_editor/functioncodedialog.ui \
    statedialog.ui \
    messagedialog.ui








































































