#include "cubewidget.h"
#include "ui_cubewidget.h"

CubeWidget::CubeWidget(QString head, QStringList axes, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CubeWidget)
{
    ui->setupUi(this);
    QCalendarWidget *begCalendarWidget = new QCalendarWidget();
    begCalendarWidget->setFirstDayOfWeek(Qt::Monday);
    ui->dateEditBeg->setCalendarWidget(begCalendarWidget);
    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()+1));
    QCalendarWidget *endCalendarWidget = new QCalendarWidget();
    endCalendarWidget->setFirstDayOfWeek(Qt::Monday);
    ui->dateEditEnd->setCalendarWidget(endCalendarWidget);
    ui->dateEditEnd->setDate(QDate::currentDate().addDays(1));

    axisX = new AxisWidget(axes);
    axisY = new AxisWidget(axes);
    ui->groupBoxX->layout()->addWidget(axisX);
    ui->groupBoxY->layout()->addWidget(axisY);

    this->setWindowTitle(head);

    connect(ui->cmdUpd,SIGNAL(clicked()),this,SLOT(updQuery()));
    connect(axisX,SIGNAL(sigUpd(QStringList)),this,SIGNAL(sigUpdX(QStringList)));
    connect(axisY,SIGNAL(sigUpd(QStringList)),this,SIGNAL(sigUpdY(QStringList)));

    connect(ui->radioButtonSum,SIGNAL(clicked(bool)),this,SIGNAL(sigSum(bool)));
    connect(ui->radioButtonAvg,SIGNAL(clicked(bool)),this,SIGNAL(sigAvg(bool)));
    connect(ui->radioButtonMin,SIGNAL(clicked(bool)),this,SIGNAL(sigMin(bool)));
    connect(ui->radioButtonMax,SIGNAL(clicked(bool)),this,SIGNAL(sigMax(bool)));
    connect(ui->cmdSave,SIGNAL(clicked()),this,SLOT(saveXls()));
}

CubeWidget::~CubeWidget()
{
    qDebug()<<"close";
    delete ui;
}

TableView *CubeWidget::getView()
{
    return ui->tableView;
}

QDate CubeWidget::getBegDate()
{
    return ui->dateEditBeg->date();
}

QDate CubeWidget::getEndDate()
{
    return ui->dateEditEnd->date();
}

void CubeWidget::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
    emit sigClose();
}


void CubeWidget::updQuery()
{
    return;
}

void CubeWidget::saveXls()
{
    ui->tableView->save(this->windowTitle()+tr(" с ")+ui->dateEditBeg->date().toString("dd.MM.yy")+tr(" по ")+ui->dateEditEnd->date().toString("dd.MM.yy"));
}
