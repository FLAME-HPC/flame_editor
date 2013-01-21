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
    parser/y.tab.c \
    parser/lex.yy.c \
    cearrow.cpp \
    cecfile.cpp \
    cecodedialog.cpp \
    ceautocompletiontextedit.cpp \
    ceassignmentdialog.cpp \
    cecodeparser.cpp \
    cefiletype.cpp \
    cegraphicsarcitem.cpp \
    cegraphicsgrup.cpp \
    cegraphicsitem.cpp \
    celineedit.cpp \
    celinkedlist.cpp \
    cemachinescene.cpp \
    cememorymodel.cpp \
    cetreemodelcompleter.cpp \
    cevariable.cpp \
    cevariabledeclared.cpp \
    cevariabledeclareddelegate.cpp \
    cevariabledeclaredmodel.cpp \
    cexmlfile.cpp

HEADERS  += \
    parser/y.tab.h \
    cearrow.h \
    cecfile.h \
    cecodedialog.h \
    ceautocompletiontextedit.h \
    ceassignmentdialog.h \
    cecodeparser.h \
    cefiletype.h \
    cegraphicsarcitem.h \
    cegraphicsgrup.h \
    cegraphicsitem.h \
    celineedit.h \
    celinkedlist.h \
    cemachinescene.h \
    cememorymodel.h \
    cetreemodelcompleter.h \
    cevariable.h \
    cevariabledeclared.h \
    cevariabledeclareddelegate.h \
    cevariabledeclaredmodel.h \
    cexmlfile.h

FORMS    +=

RESOURCES +=
