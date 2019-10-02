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
#include "singlecubewidget.h"
#include "doublecubewidget.h"
#include "dialoggroups.h"
#include <QPrinter>
#include <QPrintDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CfgPlot *cfgPlot;
    PlotData *plotData;
    ModelDry *modelDry;
    ModelFilterDry *modelFilterDry;
    QDataWidgetMapper *mapper;
    ModelOven *modelOven;
    void loadSettings();
    void saveSettings();

private slots:
    void setCurrentMapperIndex(QModelIndex src_index);
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
