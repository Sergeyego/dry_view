#include "widgets.h"

ProcViewer::ProcViewer(QWidget *parent) :
    QTableView(parent)
{
}

void ProcViewer::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Delete){
        int row=this->currentIndex().row();
        if (event->modifiers()==Qt::ControlModifier && row>=0){
            QString nam;
            for (int i=0; i<this->model()->columnCount(); i++)
                nam+=this->model()->data(this->model()->index(row,i)).toString()+" ";
            if (!nam.isEmpty()) nam.truncate(nam.size()-1);
            int n = QMessageBox::question(this,tr("Подтвердите удаление"),tr("Удалить ")+nam+"?",QMessageBox::Yes,QMessageBox::No);
            if (n==QMessageBox::Yes){
                this->model()->removeRows(row,1);
                if (row< this->model()->rowCount() && row>=0) this->selectRow(row);
            }
        }
    } else {
        return QTableView::keyPressEvent(event);
    }
}
