#ifndef MODELS_H
#define MODELS_H

#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>
#include <QDateTime>
#include <QSqlQuery>
#include <QSortFilterProxyModel>
#include <QDebug>
#include "db/dbtablemodel.h"
#include "rels.h"

class ModelOven : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelOven(QObject *parent = 0);
    
signals:
    
public slots:
    void refresh();
    
};

class ModelDry : public DbTableModel
{
    Q_OBJECT
public:
    explicit ModelDry(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;

public slots:
    void refresh(QDate dBeg, QDate dEnd, bool l_month);
    void sort(int section);
};

#endif // MODELS_H
