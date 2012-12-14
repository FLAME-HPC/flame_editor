#-------------------------------------------------
#
# Project created by QtCreator 2012-04-19T10:02:35
#
#-------------------------------------------------

QT       += core gui

QT       += xml

TARGET = code_editor
TEMPLATE = app


SOURCES += \
    main.cpp \
    machinescene.cpp \
    graphicsitem.cpp \
    arrow.cpp \
    codedialog.cpp \
    assignmentdialog.cpp \
    autocompletiontextedit.cpp \
    parser/y.tab.c \
    parser/lex.yy.c \
    treemodelcompleter.cpp \
    linkedlist.cpp \
    codeparser.cpp \
    graphicsarcitem.cpp \
    graphicsgrup.cpp \
    xmlfile.cpp \
    filetype.cpp \
    cfile.cpp \
    memorymodel.cpp \
    variable.cpp \
    variabledeclared.cpp \
    variabledeclaredmodel.cpp \
    variabledeclareddelegate.cpp \
    lineedit.cpp

HEADERS  += \
    machinescene.h \
    graphicsitem.h \
    arrow.h \
    codedialog.h \
    assignmentdialog.h \
    autocompletiontextedit.h \
    parser/y.tab.h \
    treemodelcompleter.h \
    linkedlist.h \
    codeparser.h \
    graphicsarcitem.h \
    graphicsgrup.h \
    xmlfile.h \
    filetype.h \
    cfile.h \
    memorymodel.h \
    variable.h \
    variabledeclared.h \
    variabledeclaredmodel.h \
    variabledeclareddelegate.h \
    lineedit.h

FORMS    +=

RESOURCES +=
