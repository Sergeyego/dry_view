#include "models.h"

ModelOven::ModelOven(QObject *parent) :
    QSqlQueryModel(parent)
{
}

void ModelOven::refresh()
{
    setQuery("select o.id, o.num, g.nam, o.pwr||'+'||o.pwr_cool||' kW' from owens as o "
             "inner join owens_groups as g on o.id_group=g.id "
             "order by o.num");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Ok);
    } else {
        setHeaderData(1, Qt::Horizontal,tr("Печь"));
        setHeaderData(2, Qt::Horizontal,tr("Группа"));
        setHeaderData(3, Qt::Horizontal,tr("Мощность"));
    }
}


ModelDry::ModelDry(QObject *parent) :
    QSqlQueryModel(parent)
{

}

QVariant ModelDry::data(const QModelIndex &item, int role) const
{
    QVariant value= QSqlQueryModel::data(item,role);
    if (role==Qt::DisplayRole || role==Qt::EditRole){
        if (value.isNull()) {
            value=QString("");
        } else if ((item.column()==2 || item.column()==11) && role==Qt::EditRole){
            value=value.toDateTime().toString("dd.MM.yy hh:mm");
        } else if (item.column()==10){
            return QLocale().toString(value.toDouble(),'f',3);
        }
    } else if (role==Qt::BackgroundRole){
        if (!data(index(item.row(),11)).toString().isEmpty())
            return QVariant(QColor(170,255,170));
    }
    return value;
}

bool ModelDry::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row>=rowCount() || row<0) return false;
    int id=data(index(row,0),Qt::EditRole).toInt();
    bool ok=false;
    QSqlQuery query;
    query.prepare("delete from owens_rab where id= ?");
    query.addBindValue(id);
    ok=query.exec();
    if (!ok){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Ok);
    } else {
        refresh(begDate,endDate,month);
    }
    return ok;
}

void ModelDry::refresh(QDate dBeg, QDate dEnd, bool l_month)
{
    QSqlQuery query;
    begDate=dBeg;
    endDate=dEnd;
    month=l_month;
    QDate date2 =QDate::currentDate().addDays(-30);
    QString suff;
    suff = l_month ? ("where r.dt_beg >= '"+date2.toString("yyyy-MM-dd")+"' ") :
                     ("where r.dt_beg between '"+dBeg.toString("yyyy-MM-dd")+"' and '"+dEnd.toString("yyyy-MM-dd")+"' ");

    query.prepare("select r.id, o.num, r.dt_beg, r.n_s, e.fnam, r.t0, r.w0, r.zms, r.kvo, "
             "rb.snam, r.energ, r.dt_end, r.new_format, r.dt_end-r.dt_beg, d.nam "
             "from owens_rab as r "
             "inner join owens as o on o.id=r.id_owen "
             "inner join dry_els as e on r.id_eldim=e.ide "
             "inner join rab_rab as rb on rb.id=r.id_rab "
             "inner join dry_reg as d on d.id=r.id_prog "
             +suff+
             "order by o.num, dt_beg");
    if (query.exec()){
        setQuery(query);
        setHeaderData(1, Qt::Horizontal,tr("Печь"));
        setHeaderData(2, Qt::Horizontal,tr("Дата-время"));
        setHeaderData(3, Qt::Horizontal,tr("Парт."));
        setHeaderData(4, Qt::Horizontal,tr("Марка-диам."));
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Ok);
    }
}


ModelFilterDry::ModelFilterDry(QObject *parent):QSortFilterProxyModel(parent)
{
}

bool ModelFilterDry::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    return source_column<5;
}


ModelDryData::ModelDryData(QObject *parent) : QSqlQueryModel(parent)
{
}

void ModelDryData::refresh(int id_dry)
{

}
