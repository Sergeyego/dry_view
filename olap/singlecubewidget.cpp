#include "singlecubewidget.h"


SingleCubeWidget::SingleCubeWidget(QString head, QStringList axes, QString qu, int dec, QWidget *parent) : CubeWidget(head, axes, qu, dec, parent)
{
    createModel();
}

SingleCubeWidget::SingleCubeWidget(int id_cube, QWidget *parent) : CubeWidget(id_cube, parent)
{
    createModel();
}

void SingleCubeWidget::createModel()
{
    olapmodel = new OlapModel(ax,d,this);
    getViewer()->setModel(olapmodel);

    connect(this,SIGNAL(sigUpdX(QStringList)),olapmodel,SLOT(setX(QStringList)));
    connect(this,SIGNAL(sigUpdY(QStringList)),olapmodel,SLOT(setY(QStringList)));
    connect(this,SIGNAL(sigSum(bool)),olapmodel,SLOT(setTypeSum(bool)));
    connect(this,SIGNAL(sigAvg(bool)),olapmodel,SLOT(setTypeAvg(bool)));
    connect(this,SIGNAL(sigMin(bool)),olapmodel,SLOT(setTypeMin(bool)));
    connect(this,SIGNAL(sigMax(bool)),olapmodel,SLOT(setTypeMax(bool)));

    connect(olapmodel,SIGNAL(sigRefresh()),this->getViewer(),SLOT(resizeToContents()));

    updQuery();
}

void SingleCubeWidget::updQuery()
{
    QString squery=query;
    squery.replace(":d1","'"+getBegDate().toString("yyyy-MM-dd")+"'");
    squery.replace(":d2","'"+getEndDate().toString("yyyy-MM-dd")+"'");
    olapmodel->setQuery(squery);
}
