#-------------------------------------------------
#
# Project created by QtCreator 2019-12-24T22:45:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = 3D_UI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
PROJECT_SRC_DIR = $$PWD # for ui**.h 子文件夹的界面文件
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    glwidget.cpp \
    login_dialog.cpp

HEADERS += \
        mainwindow.h \
    glwidget.h \
    login_dialog.h

FORMS += \
        mainwindow.ui \
    logindialog.ui

RESOURCES += \
    textures.qrc
include(dock_widget/dock_widget.pri)

VPATH += $$INCLUDEPATH#只有加了这一句，在子文件夹pri文件里不用把每个文件的路径写全就能找到对应的头文件和源文件

