#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
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

    modelFilterDry = new ModelFilterDry(this);

    modelDry = new ModelDry(modelFilterDry);
    refreshDry();

    modelFilterDry->setSourceModel(modelDry);

    ui->tableViewDry->setModel(modelFilterDry);
    ui->tableViewDry->setColumnHidden(0,true);
    ui->tableViewDry->verticalHeader()->hide();
    ui->tableViewDry->verticalHeader()->setDefaultSectionSize(ui->tableViewDry->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableViewDry->setColumnWidth(1,80);
    ui->tableViewDry->setColumnWidth(2,115);
    ui->tableViewDry->setColumnWidth(3,50);
    ui->tableViewDry->setColumnWidth(4,150);

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(modelDry);
    mapper->addMapping(ui->lineEditT,5);
    mapper->addMapping(ui->lineEditW,6);
    mapper->addMapping(ui->lineEditZms,7);
    mapper->addMapping(ui->lineEditKvo,8);
    mapper->addMapping(ui->lineEditRab,9);
    mapper->addMapping(ui->lineEditEnerg,10);
    mapper->addMapping(ui->lineEditbeg,2);
    mapper->addMapping(ui->lineEditend,11);
    mapper->addMapping(ui->lineEditS,13);
    mapper->addMapping(ui->lineEditProg,14);

    modelOven = new ModelOven(this);
    modelOven->refresh();
    ui->tableViewOven->setModel(modelOven);
    ui->tableViewOven->setColumnHidden(0,true);
    ui->tableViewOven->verticalHeader()->hide();
    ui->tableViewOven->verticalHeader()->setDefaultSectionSize(ui->tableViewOven->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableViewOven->setColumnWidth(1,100);
    ui->tableViewOven->setColumnWidth(2,200);
    ui->tableViewOven->setColumnWidth(3,100);

    connect(ui->tableViewDry->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(setCurrentMapperIndex(QModelIndex)));
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

void MainWindow::setCurrentMapperIndex(QModelIndex src_index)
{
    mapper->setCurrentModelIndex(modelFilterDry->mapToSource(src_index));
    int id_owrab=ui->tableViewDry->model()->data(ui->tableViewDry->model()->index(src_index.row(),0),Qt::EditRole).toInt();
    QDateTime begTime=ui->tableViewDry->model()->data(ui->tableViewDry->model()->index(src_index.row(),2)).toDateTime();
    QDateTime endTime=modelDry->data(modelDry->index(modelFilterDry->mapToSource(src_index).row(),11)).toDateTime();
    bool new_format=modelDry->data(modelDry->index(modelFilterDry->mapToSource(src_index).row(),12),Qt::EditRole).toBool();
    QString own=ui->tableViewDry->model()->data(ui->tableViewDry->model()->index(src_index.row(),1)).toString();
    QString mark=ui->tableViewDry->model()->data(ui->tableViewDry->model()->index(src_index.row(),4)).toString();
    QString part=ui->tableViewDry->model()->data(ui->tableViewDry->model()->index(src_index.row(),3)).toString();
    QString zms=modelDry->data(modelDry->index(modelFilterDry->mapToSource(src_index).row(),7)).toString();
    QString kvo=modelDry->data(modelDry->index(modelFilterDry->mapToSource(src_index).row(),8)).toString();
    QString wl=modelDry->data(modelDry->index(modelFilterDry->mapToSource(src_index).row(),6)).toString();
    QString tl=modelDry->data(modelDry->index(modelFilterDry->mapToSource(src_index).row(),5)).toString();
    QString tp=modelDry->data(modelDry->index(modelFilterDry->mapToSource(src_index).row(),13)).toString();
    Plot::instance()->setbaseTime(begTime);
    plotData->refresh(id_owrab, new_format,ui->checkBoxOst->isChecked());
    QTime t(0,0,0);
    double sec=Plot::instance()->canvasMap(QwtPlot::xBottom).s2();
    int d = sec / 86400;
    QString suf = (d>0)? QString::number(d)+QString::fromUtf8(" д ") : QString();
    t=t.addSecs(sec);
    QString vr=(ui->checkBoxOst->isChecked()) ? suf+t.toString("hh:mm:ss") : tp;
    Plot::instance()->setTitle("<FONT SIZE=2>"+own+tr(", ")+mark+tr(", п.")+part+tr(", замес ")+zms+tr(", ")+"<br>"+
                                           kvo+tr("кг, w0=")+wl+tr(", t0=")+tl+",<br>"+
                               begTime.toString("dd.MM.yy hh:mm:ss")+" - "+endTime.toString("dd.MM.yy hh:mm:ss")+" ("+vr+")"+"</FONT>");
}

void MainWindow::setCurrentOven(QModelIndex index)
{
    int id_oven=ui->tableViewOven->model()->data(ui->tableViewOven->model()->index(index.row(),0),Qt::EditRole).toInt();
    QString nam=ui->tableViewOven->model()->data(ui->tableViewOven->model()->index(index.row(),1)).toString();
    Plot::instance()->setbaseTime(ui->dateTimeEditBeg->dateTime());
    plotData->refresh(ui->dateTimeEditBeg->dateTime(),ui->dateTimeEditEnd->dateTime(), id_oven);
    Plot::instance()->setTitle("<FONT SIZE=2>"+nam+"<br>"+ui->dateTimeEditBeg->dateTime().toString("dd.MM.yy hh:mm:ss")+
                               " - "+ui->dateTimeEditEnd->dateTime().toString("dd.MM.yy hh:mm:ss")+"</FONT>");
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
}

void MainWindow::anDryKvo()
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
    QString query("select o.num, e.marka, d.dim, t.nam, r.n_s, "
                  "substr(cast(r.dt_beg as char(32)),1,4) as yr, "
                  "substr(cast(r.dt_beg as char(32)),1,7) as mn, "
                  "substr(cast(r.dt_beg as char(32)),1,10) as dy, "
                  "1 "
                  "from owens_rab as r "
                  "inner join owens as o on r.id_owen=o.id "
                  "inner join dry_els as d on r.id_eldim=d.ide "
                  "inner join elrtr as e on d.id_el=e.id "
                  "inner join el_types as t on e.id_vid=t.id "
                  "where r.dt_beg between :d1 and :d2");
    SingleCubeWidget *kvoCube = new SingleCubeWidget(tr("Количество прокалок"),axes,query,0);
    kvoCube->show();
    //connect(kvoCube,SIGNAL(sigClose()),kvoCube,SLOT(deleteLater()));
}

void MainWindow::anDryEnerg()
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
    QString query("select o.num, e.marka, d.dim, t.nam, r.n_s, "
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
    SingleCubeWidget *energCube = new SingleCubeWidget(tr("Расход электрической энергии на прокалку, кВт*ч"),axes,query,3);
    energCube->show();
    //connect(energCube,SIGNAL(sigClose()),energCube,SLOT(deleteLater()));
}

void MainWindow::anDryMas()
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
    QString query("select o.num, e.marka, d.dim, t.nam, r.n_s, "
                  "substr(cast(r.dt_beg as char(32)),1,4) as yr, "
                  "substr(cast(r.dt_beg as char(32)),1,7) as mn, "
                  "substr(cast(r.dt_beg as char(32)),1,10) as dy, "
                  "r.kvo/1000.0 "
                  "from owens_rab as r "
                  "inner join owens as o on r.id_owen=o.id "
                  "inner join dry_els as d on r.id_eldim=d.ide "
                  "inner join elrtr as e on d.id_el=e.id "
                  "inner join el_types as t on e.id_vid=t.id "
                  "where r.dt_beg between :d1 and :d2");
    SingleCubeWidget *masCube = new SingleCubeWidget(tr("Прокалка электродов, тонн"),axes,query,3);
    masCube->show();
    //connect(masCube,SIGNAL(sigClose()),masCube,SLOT(deleteLater()));
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
    kgCube->show();
    //connect(kgCube,SIGNAL(sigClose()),kgCube,SLOT(deleteLater()));
}

void MainWindow::anSushKvo()
{
    QStringList axes;
    axes.push_back(tr("Оборудование"));
    axes.push_back(tr("Год"));
    axes.push_back(tr("Месяц"));
    axes.push_back(tr("День"));
    axes.push_back(tr("Час"));
    axes.push_back(tr("Минута"));
    QString query("select o.num, "
                  "substr(cast(cm.dat_time as char(32)),1,4) as yr, "
                  "substr(cast(cm.dat_time as char(32)),1,7) as mn, "
                  "substr(cast(cm.dat_time as char(32)),1,10) as dy, "
                  "substr(cast(cm.dat_time as char(32)),1,13) as ch, "
                  "substr(cast(cm.dat_time as char(32)),1,16) as mn, "
                  "kvs-(select kvs from owens_counts where dat_time=(select COALESCE (max(dat_time),cm.dat_time) from owens_counts where dat_time< cm.dat_time) and id_owen=cm.id_owen) as kvo "
                  "from owens_counts as cm "
                  "inner join owens as o on o.id=cm.id_owen "
                  "where cm.dat_time between :d1 and :d2 "
                  "order by dat_time");
    SingleCubeWidget *masCube = new SingleCubeWidget(tr("Сушка электродов, стопки"),axes,query,0);
    masCube->show();
    //connect(masCube,SIGNAL(sigClose()),masCube,SLOT(deleteLater()));
}

void MainWindow::anSushEnerg()
{
    QStringList axes;
    axes.push_back(tr("Оборудование"));
    axes.push_back(tr("Год"));
    axes.push_back(tr("Месяц"));
    axes.push_back(tr("День"));
    axes.push_back(tr("Час"));
    axes.push_back(tr("Минута"));
    QString query("select o.num, "
                  "substr(cast(cm.dat_time as char(32)),1,4) as yr, "
                  "substr(cast(cm.dat_time as char(32)),1,7) as mn, "
                  "substr(cast(cm.dat_time as char(32)),1,10) as dy, "
                  "substr(cast(cm.dat_time as char(32)),1,13) as ch, "
                  "substr(cast(cm.dat_time as char(32)),1,16) as mn, "
                  "kve-(select kve from owens_counts where dat_time=(select COALESCE (max(dat_time),cm.dat_time) from owens_counts where dat_time< cm.dat_time) and id_owen=cm.id_owen) as kvo "
                  "from owens_counts as cm "
                  "inner join owens as o on o.id=cm.id_owen "
                  "where cm.dat_time between :d1 and :d2 "
                  "order by dat_time");
    SingleCubeWidget *masCube = new SingleCubeWidget(tr("Расход энергии на сушку электродов, кВт*ч"),axes,query,0);
    masCube->show();
    //connect(masCube,SIGNAL(sigClose()),masCube,SLOT(deleteLater()));
}

void MainWindow::setOst(bool b)
{
    setCurrentMapperIndex(ui->tableViewDry->currentIndex());
}

void MainWindow::cfgGroups()
{
    DialogGroups d;
    d.exec();
}
