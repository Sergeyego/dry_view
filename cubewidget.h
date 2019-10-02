#ifndef CUBEWIDGET_H
#define CUBEWIDGET_H

#include <QWidget>
#include "olapmodel.h"
#include <QCalendarWidget>
#include "axiswidget.h"
#include <QTableView>
#include "tableview.h"

namespace Ui {
class CubeWidget;
}

class CubeWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CubeWidget(QString head, QStringList axes, QWidget *parent = 0);
    ~CubeWidget();
protected:
    TableView *getView();
    QDate getBegDate();
    QDate getEndDate();

private:
    Ui::CubeWidget *ui;
    AxisWidget *axisX;
    AxisWidget *axisY;
    void closeEvent(QCloseEvent *e);

protected slots:
    virtual void updQuery();
    void saveXls();

signals:
    void sigClose();
    void sigUpdX(QStringList list);
    void sigUpdY(QStringList list);
    void sigSum(bool b);
    void sigAvg(bool b);
    void sigMin(bool b);
    void sigMax(bool b);

};

#endif // CUBEWIDGET_H
