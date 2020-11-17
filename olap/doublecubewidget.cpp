#include "doublecubewidget.h"

DoubleCubeWidget::DoubleCubeWidget(QString head, QStringList axes, QString qu1, QString qu2, int dec, QWidget *parent) : CubeWidget(head,axes,qu1,dec,parent), query1(qu1), query2(qu2)
{
    olapmodel = new DoubleOlapModel(axes,dec,this);
    getViewer()->setModel(olapmodel);
    updQuery();

    connect(this,SIGNAL(sigUpdX(QStringList)),olapmodel,SLOT(setX(QStringList)));
    connect(this,SIGNAL(sigUpdY(QStringList)),olapmodel,SLOT(setY(QStringList)));
    connect(this,SIGNAL(sigSum(bool)),olapmodel,SLOT(setTypeSum(bool)));
    connect(this,SIGNAL(sigAvg(bool)),olapmodel,SLOT(setTypeAvg(bool)));
    connect(this,SIGNAL(sigMin(bool)),olapmodel,SLOT(setTypeMin(bool)));
    connect(this,SIGNAL(sigMax(bool)),olapmodel,SLOT(setTypeMax(bool)));

    connect(olapmodel,SIGNAL(sigRefresh()),this->getViewer(),SLOT(resizeToContents()));
}

void DoubleCubeWidget::updQuery()
{
    QString squery1=query1;
    QString squery2=query2;
    squery1.replace(":d1","'"+this->getBegDate().toString("yyyy-MM-dd")+"'");
    squery1.replace(":d2","'"+this->getEndDate().toString("yyyy-MM-dd")+"'");
    squery2.replace(":d1","'"+this->getBegDate().toString("yyyy-MM-dd")+"'");
    squery2.replace(":d2","'"+this->getEndDate().toString("yyyy-MM-dd")+"'");
    olapmodel->setQuery(squery1,squery2);
}
