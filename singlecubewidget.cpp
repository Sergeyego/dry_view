#include "singlecubewidget.h"


SingleCubeWidget::SingleCubeWidget(QString head, QStringList axes, QString qu, int dec, QWidget *parent) :CubeWidget(head,axes,parent), query(qu)
{
    olapmodel = new OlapModel(axes,dec,this);
    this->getView()->setModel(olapmodel);
    updQuery();

    connect(this,SIGNAL(sigUpdX(QStringList)),olapmodel,SLOT(setX(QStringList)));
    connect(this,SIGNAL(sigUpdY(QStringList)),olapmodel,SLOT(setY(QStringList)));
    connect(this,SIGNAL(sigSum(bool)),olapmodel,SLOT(setTypeSum(bool)));
    connect(this,SIGNAL(sigAvg(bool)),olapmodel,SLOT(setTypeAvg(bool)));
    connect(this,SIGNAL(sigMin(bool)),olapmodel,SLOT(setTypeMin(bool)));
    connect(this,SIGNAL(sigMax(bool)),olapmodel,SLOT(setTypeMax(bool)));

    connect(olapmodel,SIGNAL(sigRefresh()),this->getView(),SLOT(resizeToContents()));
}

void SingleCubeWidget::updQuery()
{
    QString squery=query;
    squery.replace(":d1","'"+getBegDate().toString("yyyy-MM-dd")+"'");
    squery.replace(":d2","'"+getEndDate().toString("yyyy-MM-dd")+"'");
    olapmodel->setQuery(squery);
}
