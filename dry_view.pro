#-------------------------------------------------
#
# Project created by QtCreator 2013-10-14T10:37:27
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dry_view
TEMPLATE = app
CONFIG += qwt

SOURCES += main.cpp\
    cfgplot.cpp \
    models.cpp \
    srcplot.cpp \
    plotdata.cpp \
    srcchannel.cpp \
    cfgowendialog.cpp \
    pushform.cpp \
    cbrelationdelegate.cpp \
    dbtablemodel.cpp \
    widgets.cpp \
    viewer.cpp \
    delegate.cpp \
    mainwindow.cpp \
    axiswidget.cpp \
    cubic.cpp \
    cubewidget.cpp \
    olapmodel.cpp \
    tableview.cpp \
    doubleolapmodel.cpp \
    doublecubewidget.cpp \
    singlecubewidget.cpp \
    dialoggroups.cpp

HEADERS  += \
    cfgplot.h \
    models.h \
    srcplot.h \
    plotdata.h \
    srcchannel.h \
    cfgowendialog.h \
    pushform.h \
    cbrelationdelegate.h \
    dbtablemodel.h \
    widgets.h \
    viewer.h \
    delegate.h \
    mainwindow.h \
    axiswidget.h \
    cubic.h \
    cubewidget.h \
    olapmodel.h \
    tableview.h \
    doubleolapmodel.h \
    doublecubewidget.h \
    singlecubewidget.h \
    dialoggroups.h

FORMS    += \
    cfgplot.ui \
    cfgowendialog.ui \
    pushform.ui \
    mainwindow.ui \
    axiswidget.ui \
    cubewidget.ui \
    dialoggroups.ui

LIBS     += -lqwt-qt5 \
            -lxls

INCLUDEPATH+=/usr/include/xlslib \
             /usr/include/qt5/qwt

RESOURCES += \
    res.qrc
