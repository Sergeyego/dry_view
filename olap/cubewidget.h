#ifndef CUBEWIDGET_H
#define CUBEWIDGET_H

#include <QWidget>
#include "olapmodel.h"
#include <QCalendarWidget>
#include "axiswidget.h"
#include <QTableView>

namespace Ui {
class CubeWidget;
}

class CubeWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CubeWidget(QString head, QStringList axes, QString qu, int dec, QWidget *parent = 0);
    explicit CubeWidget(int id_cube, QWidget *parent = 0);
    ~CubeWidget();
    QTableView *getViewer();
    QDate getBegDate();
    QDate getEndDate();
    
private:
    void inital(QString head, QStringList axes, QString qu, int dec);
    Ui::CubeWidget *ui;
    //OlapModel *olapmodel;
    QString query;
    AxisWidget *axisX;
    AxisWidget *axisY;
    QString he;
    QStringList ax;
    int d;

private slots:
    void updQuery();
    void saveXls();

signals:
    void sigUpdX(QStringList list);
    void sigUpdY(QStringList list);
    void sigSum(bool b);
    void sigAvg(bool b);
    void sigMin(bool b);
    void sigMax(bool b);

};

#endif // CUBEWIDGET_H
