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


ModelDry::ModelDry(QObject *parent) : DbTableModel("owens_rab", parent)
{
    addColumn("id","id");
    addColumn("id_owen",QString::fromUtf8("Печь"),Rels::instance()->relOwen);
    addColumn("dt_beg",QString::fromUtf8("Дата-время"));
    addColumn("n_s",QString::fromUtf8("Парт."));
    addColumn("id_eldim",QString::fromUtf8("Марка-диам."),Rels::instance()->relElDim);
    addColumn("t0",QString::fromUtf8("Нач.темп."));
    addColumn("w0",QString::fromUtf8("Нач.влаж."));
    addColumn("zms",QString::fromUtf8("Замес"));
    addColumn("kvo",QString::fromUtf8("К-во"));
    addColumn("id_rab",QString::fromUtf8("Работник"),Rels::instance()->relRab);
    addColumn("energ",QString::fromUtf8("Энерг."));
    addColumn("dt_end",QString::fromUtf8("Конец"));
    addColumn("new_format",QString::fromUtf8("Нов.формат"));
    addColumn("id_prog",QString::fromUtf8("Прог."),Rels::instance()->relProg);

    setSuffix("inner join owens as o on o.id = owens_rab.id_owen "
              "inner join dry_els as d on d.ide = owens_rab.id_eldim");

    setSort("owens_rab.dt_beg");

}

QVariant ModelDry::data(const QModelIndex &index, int role) const
{
    if (role==Qt::BackgroundRole){
        if (!this->data(this->index(index.row(),11),Qt::EditRole).isNull())
            return QVariant(QColor(170,255,170));
    }
    return DbTableModel::data(index,role);
}


void ModelDry::refresh(QDate dBeg, QDate dEnd, bool l_month)
{
    QDate date2 =QDate::currentDate().addDays(-30);
    QString flt;
    flt = l_month ? ("dt_beg >= '"+date2.toString("yyyy-MM-dd")+"' ") :
                    ("dt_beg between '"+dBeg.toString("yyyy-MM-dd")+"' and '"+dEnd.toString("yyyy-MM-dd")+"' ");
    setFilter(flt);
    select();

}

void ModelDry::sort(int section)
{
    QString s="owens_rab.dt_beg";
    if (section==1){
        s="o.num, owens_rab.dt_beg";
    } else if (section==3){
        s="owens_rab.n_s, owens_rab.dt_beg";
    } else if (section==4){
        s="d.fnam, owens_rab.dt_beg";
    }
    if (DbTableModel::sort==s){
        int pos=s.indexOf(",");
        if (pos!=-1){
            s=s.insert(pos," desc");
        } else {
            s+=" desc";
        }
    }
    setSort(s);
    select();
}

