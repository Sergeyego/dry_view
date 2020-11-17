#include "doubleolapmodel.h"

DoubleOlapModel::DoubleOlapModel(QStringList axes, int dec, QObject *parent):
    QAbstractTableModel(parent)
{
    model1 = new OlapModel(axes,dec,parent);
    model2 = new OlapModel(axes,dec,parent);
    connect(model2,SIGNAL(sigRefresh()),this,SIGNAL(sigRefresh()));
}

int DoubleOlapModel::rowCount(const QModelIndex &parent) const
{
    return model1->rowCount(parent);
}

int DoubleOlapModel::columnCount(const QModelIndex &parent) const
{
    return model1->columnCount(parent);
}

QVariant DoubleOlapModel::data(const QModelIndex &index, int role) const
{
    if (role==Qt::DisplayRole || role==Qt::EditRole){
        double i,n;
        i=model1->data(index,Qt::EditRole).toDouble();
        n=model2->data(index,Qt::EditRole).toDouble();
        if (i==0.0 && n==0.0) return QVariant();
        QString dat;
        //dat+=tr("и= ")+QLocale().toString(i,'f',3)+" \n";
        //dat+=tr("н= ")+QLocale().toString(n,'f',3)+" \n"+tr("%= ");
        if (n!=0.0) {
            dat+=QLocale().toString(i/n,'f',2);
        } else {
            dat+="NaN";
        }
        return dat;
    } else if (role==Qt::TextAlignmentRole ){
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    } else
        return QVariant();
}

QVariant DoubleOlapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return model1->headerData(section,orientation,role);
}

void DoubleOlapModel::setQuery(QString qu1, QString qu2)
{
    beginResetModel();
    model1->setQuery(qu1);
    model2->setQuery(qu2);
    endResetModel();
}

void DoubleOlapModel::refresh()
{
    beginResetModel();
    model1->refresh();
    model2->refresh();
    endResetModel();
}

void DoubleOlapModel::setTypeSum(bool b)
{
    beginResetModel();
    model1->setTypeSum(b);
    model2->setTypeSum(b);
    endResetModel();
}

void DoubleOlapModel::setTypeAvg(bool b)
{
    beginResetModel();
    model1->setTypeAvg(b);
    model2->setTypeAvg(b);
    endResetModel();
}

void DoubleOlapModel::setTypeMax(bool b)
{
    beginResetModel();
    model1->setTypeMax(b);
    model2->setTypeMax(b);
    endResetModel();
}

void DoubleOlapModel::setTypeMin(bool b)
{
    beginResetModel();
    model1->setTypeMin(b);
    model2->setTypeMin(b);
    endResetModel();
}

void DoubleOlapModel::setX(QStringList x)
{
    beginResetModel();
    model1->setX(x);
    model2->setX(x);
    endResetModel();
}

void DoubleOlapModel::setY(QStringList y)
{
    beginResetModel();
    model1->setY(y);
    model2->setY(y);
    endResetModel();
}
