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
    WidgetShowScene.cpp \
    ListWidgetLayout.cpp \
    GraphicFramework/GraphicsView.cpp \
    GraphicFramework/GraphicsScene.cpp \
    GraphicFramework/GraphicsItem.cpp \
    GraphicFramework/AnchorItem.cpp \
    undoFramework/MoveCommand.cpp \
    undoFramework/DeleteCommand.cpp \
    undoFramework/AddCommand.cpp \
    WidgetMain.cpp

HEADERS  += MainWindow.h \
    ListWidgetItem_Form.h \
    ListWidgetItem.h \
    ListWidget.h \
    WidgetShowScene.h \
    ListWidgetLayout.h \
    GraphicFramework/GraphicsView.h \
    GraphicFramework/GraphicsScene.h \
    GraphicFramework/GraphicsItem.h \
    GraphicFramework/AnchorItem.h \
    undoFramework/MoveCommand.h \
    undoFramework/DeleteCommand.h \
    undoFramework/AddCommand.h \
    WidgetMain.h

FORMS    += MainWindow.ui \
    ListWidgetItem_Form.ui \
    WidgetMain.ui

RESOURCES += \
    src.qrc
RC_FILE = AppIcon.rc
