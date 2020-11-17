#include "dialoggroups.h"
#include "ui_dialoggroups.h"

DialogGroups::DialogGroups(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGroups)
{
    ui->setupUi(this);
    modelGroup = new DbTableModel("owens_groups",this);
    modelGroup->addColumn("id",tr("id"));
    modelGroup->addColumn("nam",tr("Название"));
    modelGroup->setSort("owens_groups.id");
    modelGroup->select();
    ui->tableView->setModel(modelGroup);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnWidth(1,200);
}

DialogGroups::~DialogGroups()
{
    delete ui;
}
