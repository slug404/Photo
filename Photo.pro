#-------------------------------------------------
#
# Project created by QtCreator 2012-10-31T21:51:57
#
#-------------------------------------------------

QT       += core gui
#CONFIG += qtestlib
TARGET = Photo
TEMPLATE = app

SOURCES += main.cpp\
        MainWindow.cpp \
    ListWidgetItem_Form.cpp \
    ListWidgetItem.cpp \
    ListWidget.cpp \
    GraphicsView.cpp \
    GraphicsScene.cpp \
    GraphicsItem.cpp \
    WidgetShowScene.cpp \
    ListWidgetLayout.cpp \
    AnchorItem.cpp \
    MoveCommand.cpp \
    DeleteCommand.cpp \
    AddCommand.cpp

HEADERS  += MainWindow.h \
    ListWidgetItem_Form.h \
    ListWidgetItem.h \
    ListWidget.h \
    GraphicsView.h \
    GraphicsScene.h \
    GraphicsItem.h \
    WidgetShowScene.h \
    ListWidgetLayout.h \
    AnchorItem.h \
    MoveCommand.h \
    DeleteCommand.h \
    AddCommand.h

FORMS    += MainWindow.ui \
    ListWidgetItem_Form.ui

RESOURCES += \
    src.qrc
RC_FILE = AppIcon.rc
