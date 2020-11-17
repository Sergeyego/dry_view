#ifndef DOUBLECUBEWIDGET_H
#define DOUBLECUBEWIDGET_H
#include "cubewidget.h"
#include "doubleolapmodel.h"


class DoubleCubeWidget : public CubeWidget
{
    Q_OBJECT
public:
    explicit DoubleCubeWidget(QString head, QStringList axes, QString qu1, QString qu2, int dec, QWidget *parent = 0);
private:
    DoubleOlapModel *olapmodel;
    QString query1, query2;
protected slots:
    void updQuery();
};

#endif // DOUBLECUBEWIDGET_H
