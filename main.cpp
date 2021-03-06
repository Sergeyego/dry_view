#include "mainwindow.h"
#include <QApplication>
#include <QtSql>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":images/charm.png"));

    QString key="";
    if (argc>1){
        key=QString(argv[1]);
    }

    QSqlDatabase db=QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("neo_rtx");
    db.setHostName("192.168.1.10");
    db.setPort(5432);
    db.setUserName("user");
    db.setPassword("szsm");
    if (!db.open()){
        QMessageBox::critical(NULL,"Error",db.lastError().text(),QMessageBox::Ok);
        exit(0);
    }
    MainWindow w(key=="--ro");
    w.show();
    return a.exec();
}
