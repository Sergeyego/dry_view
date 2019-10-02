#ifndef PLOTDATA_H
#define PLOTDATA_H

#include <QWidget>
#include "srcchannel.h"
#include <QMessageBox>

class PlotData : public QWidget
{
    Q_OBJECT
public:
    explicit PlotData(QWidget *parent = 0);
    void clearData();

private:
    QVector <SrcChannel*> srcData;
    
signals:
    
public slots:
    void refresh(int id_owrab, bool new_format, bool ost=false);
    void refresh(QDateTime dBeg, QDateTime dEnd, int id_oven);
    
};

#endif // PLOTDATA_H
