#ifndef DIALOGGROUPS_H
#define DIALOGGROUPS_H

#include <QDialog>
#include "db/dbtablemodel.h"

namespace Ui {
class DialogGroups;
}

class DialogGroups : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGroups(QWidget *parent = 0);
    ~DialogGroups();

private:
    Ui::DialogGroups *ui;
    DbTableModel *modelGroup;
};

#endif // DIALOGGROUPS_H
