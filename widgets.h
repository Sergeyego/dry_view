#ifndef WIDGETS_H
#define WIDGETS_H

#include <QTableView>
#include <QKeyEvent>
#include <QDebug>
#include <QMessageBox>

class ProcViewer : public QTableView
{
    Q_OBJECT
public:
    explicit ProcViewer(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *event);
    
public slots:
    
};

#endif // WIDGETS_H
