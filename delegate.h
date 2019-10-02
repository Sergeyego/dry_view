#ifndef DELEGATE_H
#define DELEGATE_H

#include <QtGui>
#include "widgets.h"
#include <QItemDelegate>
#include <QColorDialog>

class ColorDelegate : public QItemDelegate {
    Q_OBJECT
public:
    ColorDelegate(QObject *parent = 0);
     QWidget *createEditor(QWidget *parent,const QStyleOptionViewItem &option,const QModelIndex &index) const;
     void setEditorData(QWidget *editor,const QModelIndex &index) const;
     void setModelData(QWidget *editor,QAbstractItemModel *model,const QModelIndex &index) const;
     void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
     bool eventFilter(QObject *object, QEvent *event);
 };

#endif // DELEGATE_H
