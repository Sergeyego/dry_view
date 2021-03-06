#ifndef CFGOWENDIALOG_H
#define CFGOWENDIALOG_H

#include <QDialog>
#include "db/dbtablemodel.h"
#include "db/dbmapper.h"
#include "delegate.h"

namespace Ui {
class CfgOwenDialog;
}

class CfgOwenDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CfgOwenDialog(QWidget *parent = 0);
    ~CfgOwenDialog();
    
private:
    Ui::CfgOwenDialog *ui;
    DbRelation *relGroups;
    DbTableModel *modelOven;
    DbTableModel *modelChannel;
    DbTableModel *modelSensor;
    DbMapper *push;
private slots:
    void setCurrentOven(QModelIndex index);
    void setCurrentOvenSensor(QModelIndex index);
};

#endif // CFGOWENDIALOG_H
