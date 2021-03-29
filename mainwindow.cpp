#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(bool ro, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadSettings();

    ui->verticalLayoutPlot->addWidget(Plot::instance());
    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()+1));
    ui->dateEditEnd->setDate(QDate::currentDate().addDays(1));

    QCalendarWidget *begCalendarWidget = new QCalendarWidget(this);
    begCalendarWidget->setFirstDayOfWeek(Qt::Monday);
    ui->dateTimeEditBeg->setCalendarWidget(begCalendarWidget);
    QCalendarWidget *endCalendarWidget = new QCalendarWidget(this);
    endCalendarWidget->setFirstDayOfWeek(Qt::Monday);
    ui->dateTimeEditEnd->setCalendarWidget(endCalendarWidget);
    ui->dateTimeEditBeg->setDateTime(QDateTime::currentDateTime().addDays(-3));
    ui->dateTimeEditEnd->setDateTime(QDateTime::currentDateTime().addDays(1));

    cfgPlot = new CfgPlot(Plot::instance(),this);
    ui->verticalLayoutCfg->addWidget(cfgPlot);

    plotData = new PlotData(this);
    ui->verticalLayoutChn->addWidget(plotData);

    modelDry = new ModelDry(this);

    ui->tableViewDry->setModel(modelDry);
    ui->tableViewDry->setColumnHidden(0,true);
    ui->tableViewDry->setColumnWidth(1,80);
    ui->tableViewDry->setColumnWidth(2,115);
    ui->tableViewDry->setColumnWidth(3,50);
    ui->tableViewDry->setColumnWidth(4,150);
    for (int i=5; i<ui->tableViewDry->model()->columnCount(); i++){
        ui->tableViewDry->setColumnHidden(i,true);
    }

    mapper = new DbMapper(ui->tableViewDry,this);
    mapper->addMapping(ui->lineEditT,5);
    mapper->addMapping(ui->lineEditW,6);
    mapper->addMapping(ui->lineEditZms,7);
    mapper->addMapping(ui->lineEditKvo,8);
    mapper->addMapping(ui->lineEditRab,9);
    mapper->addMapping(ui->lineEditEnerg,10);
    mapper->addMapping(ui->lineEditDBeg,2);
    mapper->addMapping(ui->lineEditDEnd,11);
    mapper->addMapping(ui->lineEditProg,13);
    mapper->setAddEnable(false);
    mapper->setDelEnable(false);
    mapper->addLock(ui->cmdUpdDry);
    mapper->addLock(ui->checkBoxMonly);
    mapper->addLock(ui->checkBoxOst);

    ui->horizontalLayoutMap->insertWidget(0,mapper);
    if (ro){
        mapper->hide();
    }

    modelOven = new ModelOven(this);
    modelOven->refresh();
    ui->tableViewOven->setModel(modelOven);
    ui->tableViewOven->setColumnHidden(0,true);
    ui->tableViewOven->verticalHeader()->hide();
    ui->tableViewOven->verticalHeader()->setDefaultSectionSize(ui->tableViewOven->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableViewOven->setColumnWidth(1,100);
    ui->tableViewOven->setColumnWidth(2,200);
    ui->tableViewOven->setColumnWidth(3,100);

    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(setCurrentProc(int)));
    connect(ui->tableViewOven->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(setCurrentOven(QModelIndex)));
    connect(ui->cmdUpdOvenData,SIGNAL(clicked(bool)),this,SLOT(refreshOvenData()));
    connect(ui->cmdUpdDry,SIGNAL(clicked(bool)),this,SLOT(refreshDry()));
    connect(ui->checkBoxMonly,SIGNAL(clicked(bool)),this,SLOT(refreshDry()));
    connect(ui->actionPrint,SIGNAL(triggered(bool)),this,SLOT(print()));
    connect(ui->actionExit,SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(ui->actionCfgOven,SIGNAL(triggered(bool)),this,SLOT(edtOven()));
    connect(ui->actionDryKvo,SIGNAL(triggered(bool)),this,SLOT(anDryKvo()));
    connect(ui->actionEnerg,SIGNAL(triggered(bool)),this,SLOT(anDryEnerg()));
    connect(ui->actionDryMas,SIGNAL(triggered(bool)),this,SLOT(anDryMas()));
    connect(ui->actionEnergKg,SIGNAL(triggered(bool)),this,SLOT(anDryEnergKg()));
    connect(ui->actionSushKvo,SIGNAL(triggered(bool)),this,SLOT(anSushKvo()));
    connect(ui->actionSushKwh,SIGNAL(triggered(bool)),this,SLOT(anSushEnerg()));
    connect(ui->checkBoxOst,SIGNAL(clicked(bool)),this,SLOT(setOst(bool)));
    connect(ui->actionGroups,SIGNAL(triggered(bool)),this,SLOT(cfgGroups()));
    connect(ui->tableViewDry->horizontalHeader(),SIGNAL(sectionClicked(int)),modelDry,SLOT(sort(int)));

    refreshDry();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::loadSettings()
{
    QSettings settings("szsm", "dry_view");
    this->restoreGeometry(settings.value("geometry").toByteArray());
    this->ui->splitter->restoreState(settings.value("splitter_width").toByteArray());
}

void MainWindow::saveSettings()
{
    QSettings settings("szsm", "dry_view");
    settings.setValue("geometry", this->saveGeometry());
    settings.setValue("splitter_width",ui->splitter->saveState());
}

QString MainWindow::secToStr(qint64 sec)
{
    if (sec==0) return QString();
    QTime t(0,0,0);
    int d = sec / 86400;
    QString suf = (d>0)? QString::number(d)+QString::fromUtf8(" д ") : QString();
    t=t.addSecs(sec);
    return suf+t.toString("hh:mm:ss");
}

QString MainWindow::getSensorInfo(int id_owen, QDate date)
{
    QString info;
    QSqlQuery query;
    query.prepare("select otc.nam, otcs.nam, otcs.num, otcs.dat_pov, otcs.dat_end from owens_trm_channel_sensor otcs "
                  "inner join "
                  "(select otcs2.id_channel, max(otcs2.dat) as dat from owens_trm_channel_sensor otcs2 "
                  "where otcs2.dat <= :dat "
                  "group by otcs2.id_channel) as z on z.dat=otcs.dat and z.id_channel=otcs.id_channel "
                  "inner join owens_trm_channel otc on otc.id=otcs.id_channel "
                  "inner join owens o on o.id = otc.id_owen "
                  "where otc.is_enabled=true and o.id = :id_ow and otcs.dat_end >= :dat2 ");
    query.bindValue(":dat",date);
    query.bindValue(":id_ow",id_owen);
    query.bindValue(":dat2",date);
    if (query.exec()){
        while (query.next()){
            info+="<br>";
            info+=query.value(0).toString()+tr(": датчик ");
            info+=query.value(1).toString()+tr(" №");
            info+=query.value(2).toString()+tr(" поверка до ");
            info+=query.value(4).toDate().toString("dd.MM.yyyy")+tr(";");
        }
    } else {
        QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Ok);
    }
    return info;
}

void MainWindow::setCurrentProc(int index)
{
    int id_owrab=ui->tableViewDry->model()->data(ui->tableViewDry->model()->index(index,0),Qt::EditRole).toInt();
    int id_own=ui->tableViewDry->model()->data(ui->tableViewDry->model()->index(index,1),Qt::EditRole).toInt();

    QDateTime begTime=ui->tableViewDry->model()->data(ui->tableViewDry->model()->index(index,2),Qt::EditRole).toDateTime();
    QDateTime endTime=ui->tableViewDry->model()->data(ui->tableViewDry->model()->index(index,11),Qt::EditRole).toDateTime();
    bool new_format=ui->tableViewDry->model()->data(ui->tableViewDry->model()->index(index,12),Qt::EditRole).toBool();

    QString own=ui->tableViewDry->model()->data(ui->tableViewDry->model()->index(index,1),Qt::DisplayRole).toString();
    QString mark=ui->tableViewDry->model()->data(ui->tableViewDry->model()->index(index,4),Qt::DisplayRole).toString();
    QString part=ui->tableViewDry->model()->data(ui->tableViewDry->model()->index(index,3),Qt::DisplayRole).toString();

    QString zms=ui->tableViewDry->model()->data(ui->tableViewDry->model()->index(index,7),Qt::DisplayRole).toString();
    QString kvo=ui->tableViewDry->model()->data(ui->tableViewDry->model()->index(index,8),Qt::DisplayRole).toString();
    QString wl=ui->tableViewDry->model()->data(ui->tableViewDry->model()->index(index,6),Qt::DisplayRole).toString();
    QString tl=ui->tableViewDry->model()->data(ui->tableViewDry->model()->index(index,5),Qt::DisplayRole).toString();

    qint64 tpi=begTime.secsTo(endTime);
    QString tp=secToStr(tpi);
    ui->lineEditS->setText(tp);

    Plot::instance()->setbaseTime(begTime);
    plotData->refresh(id_owrab, new_format,ui->checkBoxOst->isChecked());
    double sec=Plot::instance()->canvasMap(QwtPlot::xBottom).s2();

    QString vr=(ui->checkBoxOst->isChecked()) ? secToStr(sec) : tp;
    QString title="<FONT SIZE=2>"+own+tr(", ")+mark+tr(", п.")+part+tr(", замес ")+zms+tr(", ")+"<br>"+
            kvo+tr("кг, w0=")+wl+tr(", t0=")+tl+",<br>"+
            begTime.toString("dd.MM.yy hh:mm:ss")+" - "+endTime.toString("dd.MM.yy hh:mm:ss")+" ("+vr+")"+"</FONT>";
    title+="<FONT SIZE=1>"+getSensorInfo(id_own,begTime.date())+"</FONT>";
    Plot::instance()->setTitle(title);
}

void MainWindow::setCurrentOven(QModelIndex index)
{
    int id_oven=ui->tableViewOven->model()->data(ui->tableViewOven->model()->index(index.row(),0),Qt::EditRole).toInt();
    QString nam=ui->tableViewOven->model()->data(ui->tableViewOven->model()->index(index.row(),1),Qt::DisplayRole).toString();
    Plot::instance()->setbaseTime(ui->dateTimeEditBeg->dateTime());
    plotData->refresh(ui->dateTimeEditBeg->dateTime(),ui->dateTimeEditEnd->dateTime(), id_oven);
    Plot::instance()->setTitle("<FONT SIZE=2>"+nam+"<br>"+ui->dateTimeEditBeg->dateTime().toString("dd.MM.yy hh:mm:ss")+
                               " - "+ui->dateTimeEditEnd->dateTime().toString("dd.MM.yy hh:mm:ss")+"</FONT>"+
                               "<FONT SIZE=1>"+getSensorInfo(id_oven,ui->dateTimeEditBeg->dateTime().date())+"</FONT>");
}

void MainWindow::refreshOvenData()
{
    setCurrentOven(ui->tableViewOven->selectionModel()->currentIndex());
}

void MainWindow::refreshDry()
{
    bool b=ui->checkBoxMonly->isChecked();
    ui->dateEditBeg->setEnabled(!b);
    ui->dateEditEnd->setEnabled(!b);
    if (!modelDry->isEmpty()){
        Rels::instance()->refresh();
    }
    modelDry->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date(),b);
}

void MainWindow::print()
{
    QwtPlotRenderer renderer;
    renderer.setDiscardFlag( QwtPlotRenderer::DiscardBackground );
    renderer.setDiscardFlag( QwtPlotRenderer::DiscardCanvasBackground );
    renderer.setDiscardFlag( QwtPlotRenderer::DiscardCanvasFrame );
    renderer.setLayoutFlag( QwtPlotRenderer::FrameWithScales );
    QPrinter printer;
    printer.setColorMode(QPrinter::Color);
    QPrintDialog dialog(&printer,this);
    if (dialog.exec()){
        QPainter painter(&printer);
        QRect page = printer.pageRect();
        int w = page.width()-page.x();
        QRect rect(page.x(), page.y(), w*0.95, w*1.3);
        renderer.render(Plot::instance(),&painter,rect);
    }
}

void MainWindow::edtOven()
{
    CfgOwenDialog d;
    d.exec();
    modelOven->refresh();
    Rels::instance()->relOwen->refreshModel();
}

void MainWindow::anDryKvo()
{
    SingleCubeWidget *kvoCube = new SingleCubeWidget(8);
    kvoCube->setAttribute(Qt::WA_DeleteOnClose);
    kvoCube->show();
}

void MainWindow::anDryEnerg()
{
    SingleCubeWidget *energCube = new SingleCubeWidget(13);
    energCube->setAttribute(Qt::WA_DeleteOnClose);
    energCube->show();
}

void MainWindow::anDryMas()
{
    SingleCubeWidget *masCube = new SingleCubeWidget(11);
    masCube->setAttribute(Qt::WA_DeleteOnClose);
    masCube->show();
}

void MainWindow::anDryEnergKg()
{
    QStringList axes;
    axes.push_back(tr("Печь"));
    axes.push_back(tr("Марка"));
    axes.push_back(tr("Диаметр"));
    axes.push_back(tr("Группа"));
    axes.push_back(tr("Партия"));
    axes.push_back(tr("Год"));
    axes.push_back(tr("Месяц"));
    axes.push_back(tr("День"));
    QString query1("select o.num, e.marka, d.dim, t.nam, r.n_s, "
                   "substr(cast(r.dt_beg as char(32)),1,4) as yr, "
                   "substr(cast(r.dt_beg as char(32)),1,7) as mn, "
                   "substr(cast(r.dt_beg as char(32)),1,10) as dy, "
                   "r.energ "
                   "from owens_rab as r "
                   "inner join owens as o on r.id_owen=o.id "
                   "inner join dry_els as d on r.id_eldim=d.ide "
                   "inner join elrtr as e on d.id_el=e.id "
                   "inner join el_types as t on e.id_vid=t.id "
                   "where r.dt_beg between :d1 and :d2");
    QString query2("select o.num, e.marka, d.dim, t.nam, r.n_s, "
                   "substr(cast(r.dt_beg as char(32)),1,4) as yr, "
                   "substr(cast(r.dt_beg as char(32)),1,7) as mn, "
                   "substr(cast(r.dt_beg as char(32)),1,10) as dy, "
                   "r.kvo "
                   "from owens_rab as r "
                   "inner join owens as o on r.id_owen=o.id "
                   "inner join dry_els as d on r.id_eldim=d.ide "
                   "inner join elrtr as e on d.id_el=e.id "
                   "inner join el_types as t on e.id_vid=t.id "
                   "where r.dt_beg between :d1 and :d2");
    DoubleCubeWidget *kgCube = new DoubleCubeWidget(tr("Расход электрической энергии на прокалку, кВт*ч/кг"),axes,query1,query2,3);
    kgCube->setAttribute(Qt::WA_DeleteOnClose);
    kgCube->show();
}

void MainWindow::anSushKvo()
{
    SingleCubeWidget *masCube = new SingleCubeWidget(33);
    masCube->setAttribute(Qt::WA_DeleteOnClose);
    masCube->show();
}

void MainWindow::anSushEnerg()
{
    SingleCubeWidget *enCube = new SingleCubeWidget(34);
    enCube->setAttribute(Qt::WA_DeleteOnClose);
    enCube->show();
}

void MainWindow::setOst(bool /*b*/)
{
    setCurrentProc(ui->tableViewDry->currentIndex().row());
}

void MainWindow::cfgGroups()
{
    DialogGroups d;
    d.exec();
}
