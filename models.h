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

class ModelOven : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelOven(QObject *parent = 0);
    
signals:
    
public slots:
    void refresh();
    
};

class ModelDry : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelDry(QObject *parent = 0);
    QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;
    bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex());
signals:

public slots:
    void refresh(QDate dBeg, QDate dEnd, bool l_month);
private:
    QDate begDate;
    QDate endDate;
    bool month;

};

class ModelFilterDry : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ModelFilterDry(QObject *parent=0);
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;
};

class ModelDryData : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelDryData(QObject *parent = 0);

signals:

public slots:
    void refresh(int id_dry);

};

#endif // MODELS_H
