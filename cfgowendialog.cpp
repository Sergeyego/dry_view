#include "cfgowendialog.h"
#include "ui_cfgowendialog.h"

CfgOwenDialog::CfgOwenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CfgOwenDialog)
{
    ui->setupUi(this);

    modelSensor = new DbTableModel("owens_trm_channel_sensor",this);
    modelSensor->addColumn("id_channel",tr("id_channel"));
    modelSensor->addColumn("dat",tr("Дата установки"));
    modelSensor->addColumn("nam",tr("Марка датчика"));
    modelSensor->addColumn("dat_pov",tr("Поверка"));
    modelSensor->addColumn("dat_end",tr("Оконч.поверки"));
    modelSensor->addColumn("num",tr("Зав.номер"));
    modelSensor->setSort("dat");

    relGroups = new DbRelation(new DbRelationalModel("select id, nam from owens_groups order by id",this),0,1,this);
    modelOven = new DbTableModel("owens",this);
    modelOven->addColumn("id","id");
    modelOven->addColumn("num",tr("Название"));
    modelOven->addColumn("tip",tr("Тип"));
    modelOven->addColumn("host",tr("Хост"));
    modelOven->addColumn("tcp_port",tr("Порт"));
    modelOven->addColumn("addr",tr("Адрес"));
    modelOven->addColumn("pwr",tr("Мощность"));
    modelOven->addColumn("pwr_cool",tr("Мощность вент"));
    modelOven->addColumn("id_group",tr("Группа"),relGroups);
    modelOven->setSort("owens.num");
    modelOven->select();

    modelChannel = new DbTableModel("owens_trm_channel",this);
    modelChannel->addColumn("id_owen",tr("id_owen"));
    modelChannel->addColumn("id",tr("Id"));
    modelChannel->addColumn("number",tr("Номер"));
    modelChannel->addColumn("nam",tr("Название"));
    modelChannel->addColumn("is_main",tr("Главный"));
    modelChannel->addColumn("color_ust",tr("Уставка"));
    modelChannel->addColumn("color_val",tr("Значение"));
    modelChannel->addColumn("pwr",tr("Мощность"));
    modelChannel->addColumn("k_ust",tr("Коэф. уст."));
    modelChannel->addColumn("is_enabled",tr("Включен"));

    modelChannel->setSort("owens_trm_channel.number, owens_trm_channel.nam");
    modelChannel->setColumnFlags(1,Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    ui->tableViewSensor->setModel(modelSensor);
    ui->tableViewSensor->setColumnHidden(0,true);
    ui->tableViewSensor->setColumnWidth(1,110);
    ui->tableViewSensor->setColumnWidth(2,140);
    ui->tableViewSensor->setColumnWidth(3,110);
    ui->tableViewSensor->setColumnWidth(4,110);
    ui->tableViewSensor->setColumnWidth(5,140);

    ui->tableViewCh->setModel(modelChannel);
    ui->tableViewCh->setColumnHidden(0,true);
    ui->tableViewCh->setColumnWidth(1,50);
    ui->tableViewCh->setColumnWidth(2,50);
    ui->tableViewCh->setColumnWidth(3,120);
    ui->tableViewCh->setColumnWidth(4,80);
    ui->tableViewCh->setColumnWidth(5,80);
    ui->tableViewCh->setColumnWidth(6,80);
    ui->tableViewCh->setColumnWidth(7,80);
    ui->tableViewCh->setColumnWidth(8,80);
    ui->tableViewCh->setColumnWidth(9,80);

    ui->tableViewCh->setItemDelegateForColumn(5,new ColorDelegate(this));
    ui->tableViewCh->setItemDelegateForColumn(6,new ColorDelegate(this));

    ui->listViewOven->setModel(modelOven);
    ui->listViewOven->setModelColumn(1);
    push = new DbMapper(ui->listViewOven,this);
    ui->horizontalLayoutMap->insertWidget(0,push);
    push->addMapping(ui->lineEditId,0);
    push->addMapping(ui->lineEditNum,1);
    push->addMapping(ui->lineEditTyp,2);
    push->addMapping(ui->lineEditHost,3);
    push->addMapping(ui->lineEditPort,4);
    push->addMapping(ui->lineEditAddr,5);
    push->addMapping(ui->lineEditPn,6);
    push->addMapping(ui->lineEditPv,7);
    push->addMapping(ui->comboBoxGroup,8);
    push->addLock(ui->listViewOven);
    push->addLock(ui->tableViewCh);

    connect(ui->listViewOven->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(setCurrentOven(QModelIndex)));
    connect(ui->tableViewCh->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(setCurrentOvenSensor(QModelIndex)));
}

CfgOwenDialog::~CfgOwenDialog()
{
    delete ui;
}

void CfgOwenDialog::setCurrentOven(QModelIndex index)
{
    int id_oven=ui->listViewOven->model()->data(ui->listViewOven->model()->index(index.row(),0),Qt::EditRole).toInt();
    modelChannel->setFilter("owens_trm_channel.id_owen = "+QString::number(id_oven));
    modelChannel->setDefaultValue(0,id_oven);
    modelChannel->select();
    if (modelChannel->rowCount()){
        ui->tableViewCh->selectRow(0);
    }
}

void CfgOwenDialog::setCurrentOvenSensor(QModelIndex index)
{
    int id_channel=ui->tableViewCh->model()->data(ui->tableViewCh->model()->index(index.row(),1),Qt::EditRole).toInt();
    modelSensor->setFilter("owens_trm_channel_sensor.id_channel = "+QString::number(id_channel));
    modelSensor->setDefaultValue(0,id_channel);
    modelSensor->select();
}
