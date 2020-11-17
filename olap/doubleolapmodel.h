#ifndef DOUBLEOLAPMODEL_H
#define DOUBLEOLAPMODEL_H
#include "olapmodel.h"

class DoubleOlapModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit DoubleOlapModel(QStringList axes, int dec, QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void setQuery(QString qu1, QString qu2);

signals:
    void sigRefresh();

public slots:
    void refresh();
    void setTypeSum(bool b);
    void setTypeAvg(bool b);
    void setTypeMax(bool b);
    void setTypeMin(bool b);
    void setX(QStringList x);
    void setY(QStringList y);

private:
    OlapModel *model1;
    OlapModel *model2;

};

#endif // DOUBLEOLAPMODEL_H
