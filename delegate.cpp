#include "delegate.h"

ColorDelegate::ColorDelegate(QObject *parent):QItemDelegate(parent)
{

}

QWidget *ColorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QColorDialog *d = new QColorDialog(parent);
    return d;
}

void ColorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QColorDialog *d= static_cast<QColorDialog*>(editor);
    if (d){
        QColor color=Qt::black;
        QStringList col=(index.model()->data(index, Qt::EditRole).toString()).split(":");
        if (col.size()==3){
             color=QColor(col.at(0).toInt(),col.at(1).toInt(),col.at(2).toInt());
        }
        d->setCurrentColor(color);
    }
    return;
}

void ColorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QColorDialog *d= static_cast<QColorDialog*>(editor);
    if (d){
        QString val;
        QColor c= d->currentColor();
        val=QString::number(c.red())+":"+QString::number(c.green())+":"+QString::number(c.blue());
        model->setData(index,val);
    }
}

void ColorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QColor color=Qt::black;
    QStringList col=(index.model()->data(index, Qt::EditRole).toString()).split(":");
    if (col.size()==3){
         color=QColor(col.at(0).toInt(),col.at(1).toInt(),col.at(2).toInt());
    }
    painter->fillRect(option.rect, color);
    if (option.state & QStyle::State_Selected){
        QRect r=option.rect;
        int dx=r.width()*0.1;
        int dy=r.height()*0.1;
        painter->drawRect(QRect(r.left()+dx,r.top()+dy,r.width()-dx*2,r.height()-dy*2));
    }
}

bool ColorDelegate::eventFilter(QObject *object, QEvent *event)
{
    if (event->type()== QEvent::FocusOut)
        return false;
    return QItemDelegate::eventFilter(object,event);
}

