#ifndef SINGLECUBEWIDGET_H
#define SINGLECUBEWIDGET_H
#include "cubewidget.h"


class SingleCubeWidget : public CubeWidget
{
    Q_OBJECT
public:
    explicit SingleCubeWidget(QString head, QStringList axes, QString qu, int dec, QWidget *parent = 0);
private:
    OlapModel *olapmodel;
    QString query;

protected slots:
    void updQuery();
};

#endif // SINGLECUBEWIDGET_H
