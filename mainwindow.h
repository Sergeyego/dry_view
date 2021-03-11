#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cfgplot.h"
#include "cfgplot.h"
#include "models.h"
#include <QDataWidgetMapper>
#include "plotdata.h"
#include <qwt_plot_renderer.h>
#include "cfgowendialog.h"
#include "olap/singlecubewidget.h"
#include "olap/doublecubewidget.h"
#include "dialoggroups.h"
#include <QPrinter>
#include <QPrintDialog>
#include "db/dbmapper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(bool ro, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CfgPlot *cfgPlot;
    PlotData *plotData;
    ModelDry *modelDry;
    DbMapper *mapper;
    ModelOven *modelOven;
    void loadSettings();
    void saveSettings();
    QString secToStr(qint64 sec);

private slots:
    void setCurrentProc(int index);
    void setCurrentOven(QModelIndex index);
    void refreshOvenData();
    void refreshDry();
    void print();
    void edtOven();
    void anDryKvo();
    void anDryEnerg();
    void anDryMas();
    void anDryEnergKg();
    void anSushKvo();
    void anSushEnerg();
    void setOst(bool b);
    void cfgGroups();
};

#endif // MAINWINDOW_H
