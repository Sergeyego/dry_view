#ifndef SRCCHANNEL_H
#define SRCCHANNEL_H

#include <QWidget>
#include "srcplot.h"
#include <QSqlError>
#include <QGroupBox>
#include <QMessageBox>

class SrcChannel : public QWidget
{
    Q_OBJECT
public:
    SrcChannel(int id_ch, int id_owr, bool o, QWidget *parent = 0);
    SrcChannel(QString name, QColor cv, QColor cu, QWidget *parent = 0);
    SrcChannel(int id_ch, QDateTime dBeg, QDateTime dEnd, QWidget *parent = 0);
    ~SrcChannel();
    void addData(double ust, double val, double t);
    void plotData(double xmax);
    void clear();
private:
    SrcPlot *srcUst;
    SrcPlot *srcVal;
    SrcPlot *srcPwr;
    bool ost;
    bool createChannel(int id_channel);
    QGroupBox *groupBoxCh;
    void updDataPlot(int id_channel, int id_owrab);
    void updDataPlot(int id_channel, QDateTime dBeg, QDateTime dEnd);
    void mkWidget(QString name, QColor cv, QColor cu);
    
signals:
    
public slots:
    
};

#endif // SRCCHANNEL_H
