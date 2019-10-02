#include "cfgowendialog.h"
#include "ui_cfgowendialog.h"

CfgOwenDialog::CfgOwenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CfgOwenDialog)
{
    ui->setupUi(this);
    relGroups = new DbRelation(new DbRelationalModel("select id, nam from owens_groups order by id",this),0,1,this);
    modelOven = new DbTableModel("owens",this);
    modelOven->addColumn("id","id",true,true,TYPE_INT);
    modelOven->addColumn("num",tr("Название"),false,false,TYPE_STRING);
    modelOven->addColumn("tip",tr("Тип"),false,false,TYPE_STRING);
    modelOven->addColumn("host",tr("Хост"),false,false,TYPE_STRING);
    modelOven->addColumn("tcp_port",tr("Порт"),false,false,TYPE_INT,new QIntValidator(1,65535,this));
    modelOven->addColumn("addr",tr("Адрес"),false,false,TYPE_INT,new QIntValidator(0,65535,this));
    modelOven->addColumn("pwr",tr("Мощность"),false,false,TYPE_DOUBLE,new QDoubleValidator(0,999999999,3,this));
    modelOven->addColumn("pwr_cool",tr("Мощность вент"),false,false,TYPE_DOUBLE,new QDoubleValidator(0,999999999,3,this));
    modelOven->addColumn("id_group",tr("Группа"),false,false,TYPE_STRING,NULL,relGroups);
    modelOven->setSort("owens.num");
    modelOven->select();

    modelChannel = new DbTableModel("owens_trm_channel",this);
    modelChannel->addColumn("id_owen",tr("id_owen"),false,false,TYPE_INT);
    modelChannel->addColumn("id",tr("Id"),true,true,TYPE_INT,new QIntValidator(this));
    modelChannel->addColumn("number",tr("Номер"),false,false,TYPE_INT,new QIntValidator(this));
    modelChannel->addColumn("nam",tr("Название"),false,false,TYPE_STRING);
    modelChannel->addColumn("is_main",tr("Главный"),false,false,TYPE_BOOL);
    modelChannel->addColumn("color_ust",tr("Уставка"),false,false,TYPE_STRING);
    modelChannel->addColumn("color_val",tr("Значение"),false,false,TYPE_STRING);
    modelChannel->addColumn("pwr",tr("Мощность"),false,false,TYPE_DOUBLE,new QDoubleValidator(0,1000,1,this));
    modelChannel->addColumn("k_ust",tr("Коэф. уст."),false,false,TYPE_DOUBLE,new QDoubleValidator(0,10,3,this));
    modelChannel->addColumn("is_enabled",tr("Включен"),false,false,TYPE_BOOL);

    modelChannel->setSort("owens_trm_channel.number, owens_trm_channel.nam");

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
    push = new PushForm(ui->listViewOven,this);
    ui->verticalLayout->addWidget(push);
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
    //qDebug()<<id_oven;
}
