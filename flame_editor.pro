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
    function_editor/assignmentdialog.cpp \
    function_editor/femachinescene.cpp \
    function_editor/fegraphicsitem.cpp \
    function_editor/fearrow.cpp \
    function_editor/codedialog.cpp \
    function_editor/autocompletiontextedit.cpp \
    function_editor/parser/y.tab.c \
    function_editor/parser/lex.yy.c \
    function_editor/treemodelcompleter.cpp \
    function_editor/linkedlist.cpp \
    function_editor/codeparser.cpp \
    function_editor/graphicsarcitem.cpp \
    function_editor/graphicsgrup.cpp \
    function_editor/xmlfile.cpp \
    function_editor/filetype.cpp \
    function_editor/cfile.cpp \
    function_editor/fememorymodel.cpp \
    function_editor/variabledeclared.cpp \
    function_editor/variabledeclaredmodel.cpp \
    function_editor/variabledeclareddelegate.cpp
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
    function_editor/femachinescene.h \
    function_editor/fegraphicsitem.h \
    function_editor/fearrow.h \
    function_editor/codedialog.h \
    function_editor/assignmentdialog.h \
    function_editor/autocompletiontextedit.h \
    function_editor/parser/y.tab.h \
    function_editor/treemodelcompleter.h \
    function_editor/linkedlist.h \
    function_editor/codeparser.h \
    function_editor/graphicsarcitem.h \
    function_editor/graphicsgrup.h \
    function_editor/xmlfile.h \
    function_editor/filetype.h \
    function_editor/cfile.h \
    function_editor/fememorymodel.h \
    function_editor/variabledeclared.h \
    function_editor/variabledeclaredmodel.h \
    function_editor/variabledeclareddelegate.h
FORMS += mainwindow.ui \
    conditiondialog.ui \
    commdialog.ui \
    sortdialog.ui \
    timeunitdialog.ui








































































