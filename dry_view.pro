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
    widgets.cpp \
    delegate.cpp \
    mainwindow.cpp \
    dialoggroups.cpp \
    db/dbdelegate.cpp \
    db/dblogin.cpp \
    db/dbmapper.cpp \
    db/dbtablemodel.cpp \
    db/dbviewer.cpp \
    olap/axiswidget.cpp \
    olap/cubewidget.cpp \
    olap/cubic.cpp \
    olap/doublecubewidget.cpp \
    olap/doubleolapmodel.cpp \
    olap/olapmodel.cpp \
    olap/singlecubewidget.cpp \
    olap/tableview.cpp \
    rels.cpp

HEADERS  += \
    cfgplot.h \
    models.h \
    srcplot.h \
    plotdata.h \
    srcchannel.h \
    cfgowendialog.h \
    widgets.h \
    delegate.h \
    mainwindow.h \
    dialoggroups.h \
    db/dbdelegate.h \
    db/dblogin.h \
    db/dbmapper.h \
    db/dbtablemodel.h \
    db/dbviewer.h \
    olap/axiswidget.h \
    olap/cubewidget.h \
    olap/cubic.h \
    olap/doublecubewidget.h \
    olap/doubleolapmodel.h \
    olap/olapmodel.h \
    olap/singlecubewidget.h \
    olap/tableview.h \
    rels.h

FORMS    += \
    cfgplot.ui \
    cfgowendialog.ui \
    mainwindow.ui \
    dialoggroups.ui \
    db/dblogin.ui \
    olap/axiswidget.ui \
    olap/cubewidget.ui

LIBS     += -lqwt-qt5 \
            -lxls

INCLUDEPATH+=/usr/include/xlslib \
             /usr/include/qt5/qwt

RESOURCES += \
    res.qrc
