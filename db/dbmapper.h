#ifndef DBMAPPER_H
#define DBMAPPER_H

#include <QWidget>
#include "dbtablemodel.h"
#include <QDataWidgetMapper>
#include "dbdelegate.h"
#include <QTableView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QApplication>

class DbMapper : public QWidget
{
    Q_OBJECT
    
public:
    explicit DbMapper(QAbstractItemView *v, QWidget *parent = 0);
    ~DbMapper();
    void addLock(QWidget *widget);
    void addUnLock(QWidget *widget);
    void addEmptyLock(QWidget *widget);
    bool isLock();
    void addMapping(QWidget *widget, int section);
    int currentIndex();
    void setDefaultFocus(int n);
    void setItemDelegate(QAbstractItemDelegate *delegate);
    
private:
    QVector <QWidget*> lock1;
    QVector <QWidget*> lock2;
    QVector <QWidget*> lockEmpty;
    QDataWidgetMapper *mapper;
    QAbstractItemView *viewer;
    QPushButton *cmdNew;
    QPushButton *cmdWrite;
    QPushButton *cmdEdt;
    QPushButton *cmdEsc;
    QPushButton *cmdDel;
    QHBoxLayout *mainLayout;
    bool isEdt;
    int defaultFocus;

public slots:
    void refresh();
    void slotNew();
    void slotEdt();
    void slotDel();
    void slotWrite();
    void slotEsc();
    void first();
    void last();
    void setCurrentViewRow(int row);
    void setAddEnable(bool b);
    void setDelEnable(bool b);

private slots:
    void checkEmpty();
    void lock(bool val);

signals:
    void currentIndexChanged(int index);
    void lockChanged(bool l);
};

#endif // DBMAPPER_H
