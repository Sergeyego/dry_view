#include "cubewidget.h"
#include "ui_cubewidget.h"

CubeWidget::CubeWidget(QString head, QStringList axes, QString qu, int dec, QWidget *parent) :
    QWidget(parent)
{
    he=head;
    ax=axes;
    query=qu;
    d=dec;
    inital();
}

CubeWidget::CubeWidget(int id_cube, QWidget *parent) :
    QWidget(parent)
{
    QString cols;
    QSqlQuery qu;
    qu.prepare("select nam, columns, query, dc from olaps where id= :id");
    qu.bindValue(":id",id_cube);
    if (qu.exec()){
        while (qu.next()){
            he=qu.value(0).toString();
            cols=qu.value(1).toString();
            query=qu.value(2).toString();
            d=qu.value(3).toInt();
        }

        cols=cols.replace('{',"");
        int pos=0;
        QRegExp ex1("^\"([^\"].*[^\\\\])[\"][,}]");
        ex1.setMinimal(true);
        QRegExp ex2("^([^\"].*)[,}]");
        ex2.setMinimal(true);
        while (ex1.indexIn(cols)!=-1 || ex2.indexIn(cols)!=-1){
            if (ex1.indexIn(cols)!=-1){
                ax << ex1.cap(1);
                pos=ex1.indexIn(cols)+ex1.cap(1).size()+2;
            } else if (ex2.indexIn(cols)!=-1){
                ax << ex2.cap(1);
                pos=ex2.indexIn(cols)+ex2.cap(1).size();
            } else {
                pos=-1;
            }
            cols=cols.mid(pos+1);
        }
    } else {
        QMessageBox::critical(this,"Error",qu.lastError().text(),QMessageBox::Ok);
    }
    inital();
}

CubeWidget::~CubeWidget()
{
    delete ui;
}

QTableView *CubeWidget::getViewer()
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

void CubeWidget::inital()
{
    ui = new Ui::CubeWidget;
    ui->setupUi(this);
    ui->cmdUpd->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload)));
    ui->cmdSave->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton)));

    QCalendarWidget *begCalendarWidget = new QCalendarWidget(this);
    begCalendarWidget->setFirstDayOfWeek(Qt::Monday);
    ui->dateEditBeg->setCalendarWidget(begCalendarWidget);
    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()+1));
    QCalendarWidget *endCalendarWidget = new QCalendarWidget(this);
    endCalendarWidget->setFirstDayOfWeek(Qt::Monday);
    ui->dateEditEnd->setCalendarWidget(endCalendarWidget);
    ui->dateEditEnd->setDate(QDate::currentDate());

    axisX = new AxisWidget(ax, this);
    axisY = new AxisWidget(ax, this);
    ui->groupBoxX->layout()->addWidget(axisX);
    ui->groupBoxY->layout()->addWidget(axisY);

    this->setWindowTitle(he);
    connect(ui->cmdUpd,SIGNAL(clicked()),this,SLOT(updQuery()));
    connect(ui->cmdSave,SIGNAL(clicked()),this,SLOT(saveXls()));

    connect(axisX,SIGNAL(sigUpd(QStringList)),this,SIGNAL(sigUpdX(QStringList)));
    connect(axisY,SIGNAL(sigUpd(QStringList)),this,SIGNAL(sigUpdY(QStringList)));

    connect(ui->radioButtonSum,SIGNAL(clicked(bool)),this,SIGNAL(sigSum(bool)));
    connect(ui->radioButtonAvg,SIGNAL(clicked(bool)),this,SIGNAL(sigAvg(bool)));
    connect(ui->radioButtonMin,SIGNAL(clicked(bool)),this,SIGNAL(sigMin(bool)));
    connect(ui->radioButtonMax,SIGNAL(clicked(bool)),this,SIGNAL(sigMax(bool)));
}

void CubeWidget::updQuery()
{
    return;
}

void CubeWidget::saveXls()
{
    ui->tableView->save(this->windowTitle()+tr(" с ")+ui->dateEditBeg->date().toString("dd.MM.yy")+tr(" по ")+ui->dateEditEnd->date().toString("dd.MM.yy"));
}
