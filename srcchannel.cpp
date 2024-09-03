#include "srcchannel.h"

SrcChannel::SrcChannel(int id_ch, int id_owr, bool o, QWidget *parent) :
    QWidget(parent), ost(o)
{
    this->setLayout(new QVBoxLayout(this));
    if (createChannel(id_ch))
        updDataPlot(id_ch,id_owr);
}

SrcChannel::SrcChannel(QString name, QColor cv, QColor cu, QWidget *parent) :
    QWidget(parent)
{
    ost=false;
    this->setLayout(new QVBoxLayout(this));
    mkWidget(name,cv,cu);
}

SrcChannel::SrcChannel(int id_ch, QDateTime dBeg, QDateTime dEnd, QWidget *parent) :
    QWidget(parent)
{
    ost=false;
    this->setLayout(new QVBoxLayout(this));
    if (createChannel(id_ch))
        updDataPlot(id_ch,dBeg,dEnd);
}

SrcChannel::~SrcChannel()
{
}

void SrcChannel::addData(double ust, double val, double t)
{
    if (srcUst) srcUst->addData(t,ust);
    if (srcVal) srcVal->addData(t,val);
}

void SrcChannel::plotData(double xmax)
{
    if (srcUst) srcUst->plotData();
    if (srcVal) srcVal->plotData();
    if (srcPwr) srcPwr->plotData();
    Plot::instance()->setXmax(xmax);
}

void SrcChannel::clear()
{
    if (srcUst) srcUst->clear();
    if (srcVal) srcVal->clear();
    if (srcPwr) srcPwr->clear();
    Plot::instance()->setXmax(0);
}

void SrcChannel::mkWidget(QString name, QColor cv, QColor cu)
{
    groupBoxCh = new QGroupBox(name,this);
    this->layout()->addWidget(groupBoxCh);
    srcVal = new SrcPlot(tr("Температура"),cv,true,this);
    srcUst = new SrcPlot(tr("Уставка"),cu,true,this);
    srcPwr = new SrcPlot(tr("Мощность"),Qt::black,false,this);
    groupBoxCh->setLayout(new QVBoxLayout);
    groupBoxCh->layout()->addWidget(srcVal);
    groupBoxCh->layout()->addWidget(srcUst);
    groupBoxCh->layout()->addWidget(srcPwr);
}

bool SrcChannel::createChannel(int id_channel)
{
    QSqlQuery query;
    bool ok=false;
    query.prepare("select c.nam, NULL, c.color_ust, c.color_val, c.is_main from owens_trm_channel as c where c.id= :id");
    query.bindValue(":id",id_channel);
    QStringList c_ust, c_val;
    QString nam;
    bool is_main=false;
    if (query.exec()){
        while (query.next()){
            nam=query.value(1).toString()+" "+query.value(0).toString();
            c_ust=query.value(2).toString().split(":");
            c_val=query.value(3).toString().split(":");
            is_main=query.value(4).toBool();

        }
    } else {
        QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Ok);
    }
    if (c_ust.size()==3 && c_val.size()==3) {
        QColor cu(c_ust.at(0).toInt(),c_ust.at(1).toInt(),c_ust.at(2).toInt());
        QColor cv(c_val.at(0).toInt(),c_val.at(1).toInt(),c_val.at(2).toInt());
        if (is_main) nam+=tr(" <главный>");
        mkWidget(nam,cv,cu);
        ok=true;
    }
    return ok;
}

void SrcChannel::updDataPlot(int id_channel, int id_owrab)
{
    QSqlQuery dataquery;
    dataquery.prepare("select ust, val, dat_time, pwr from owens_data_new where id_ow_rab= :idowr and id_channel= :idch order by dat_time");
    dataquery.bindValue(":idowr", id_owrab);
    dataquery.bindValue(":idch",id_channel);
    if (dataquery.exec() && srcUst && srcVal && srcPwr){
        int interval=Plot::instance()->getInterval();
        double x=0.0;
        srcUst->clear();
        srcVal->clear();
        srcPwr->clear();
        QDateTime tm=Plot::instance()->getBaseTime();
        int delta;

        if (dataquery.size() && (!ost)){
            while (dataquery.next()){
                delta=tm.secsTo(dataquery.value(2).toDateTime());
                tm=dataquery.value(2).toDateTime();
                x+= delta/interval;
                srcUst->addData(x,dataquery.value(0).toDouble());
                srcVal->addData(x,dataquery.value(1).toDouble());
                srcPwr->addData(x,dataquery.value(3).toDouble()*100.0);
            }
            plotData(x);
        } else {
            QSqlQuery query;
            QString qu("select dt_beg from owens_rab where id= :id_owr "
                    "union "
                    "select max(n.dat_time) from owens_data_new as n "
                    "where n.id_channel= :id_ch and n.dat_time>=(select dt_beg from owens_rab where id= :id_owr ) "
                    "and n.dat_time<COALESCE((select min(dt_beg) from owens_rab "
                    "where id_owen=(select id_owen from owens_rab where id= :id_owr ) and dt_beg>(select dt_beg from owens_rab where id= :id_owr )), "
                    "(select max(dat_time) from owens_data_new where id_channel= :id_ch ))");
            qu=qu.replace(QString(":id_owr"),QString::number(id_owrab));
            qu=qu.replace(QString(":id_ch"),QString::number(id_channel));
            query.prepare(qu);
            //qDebug()<<query.executedQuery();
            if (query.exec()){
                QDateTime d1,d2;
                if (query.size()==2){
                    query.next();
                    d1=query.value(0).toDateTime();
                    query.next();
                    d2=query.value(0).toDateTime();
                    updDataPlot(id_channel,d1,d2);
                }
            } else {
                QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Ok);
            }
        }
    } else {
        QMessageBox::critical(this,"Error",dataquery.lastError().text(),QMessageBox::Ok);
    }
}

void SrcChannel::updDataPlot(int id_channel, QDateTime dBeg, QDateTime dEnd)
{
    QSqlQuery dataquery;
    dataquery.prepare("select ust, val, dat_time, pwr from owens_data_new where dat_time between :d1 and :d2 and id_channel = :idch order by dat_time");
    dataquery.bindValue(":d1", dBeg.toString("yyyy-MM-dd hh:mm:ss"));
    dataquery.bindValue(":d2", dEnd.toString("yyyy-MM-dd hh:mm:ss"));
    dataquery.bindValue(":idch", id_channel);
    if (dataquery.exec() && srcUst && srcVal && srcPwr){
        int interval=Plot::instance()->getInterval();
        double x=0.0;
        srcUst->clear();
        srcVal->clear();
        QDateTime tm=Plot::instance()->getBaseTime();
        int delta;
        while (dataquery.next()){
            delta=tm.secsTo(dataquery.value(2).toDateTime());
            tm=dataquery.value(2).toDateTime();
            x+= delta/interval;
            srcUst->addData(x,dataquery.value(0).toDouble());
            srcVal->addData(x,dataquery.value(1).toDouble());
            srcPwr->addData(x,dataquery.value(3).toDouble()*100.0);
        }
        plotData(x);
    } else {
        QMessageBox::critical(this,"Error",dataquery.lastError().text(),QMessageBox::Ok);
    }
}
