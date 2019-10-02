#include "cbrelationdelegate.h"
#include <QLineEdit>

CbRelationDelegate::CbRelationDelegate(QObject *parent)
               : QItemDelegate(parent)
{

}

QWidget *CbRelationDelegate::createEditor (QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{

    const DbTableModel *sqlModel = qobject_cast<const DbTableModel *>(index.model());
    if (!sqlModel) return QItemDelegate::createEditor(parent, option, index);
    QWidget *editor=NULL;
    if (sqlModel->relation(index.column())){
        QAbstractItemModel *childModel=sqlModel->relation(index.column())->model();
        if (!childModel){
            editor=QItemDelegate::createEditor(parent, option, index);
        } else {
            editor=new QComboBox(parent);
        }
    } else {
        switch (sqlModel->columnType(index.column())){
            case TYPE_BOOL:
            {
                editor=NULL;
                break;
            }
            case TYPE_INTBOOL:
            {
                editor=NULL;
                break;
            }
            case TYPE_STRING:
            {
                editor=new QLineEdit(parent);
                break;
            }
            case TYPE_INT:
            {
                editor=new QLineEdit(parent);
                break;
            }
            case TYPE_DOUBLE:
            {
                editor=new QLineEdit(parent);
                break;
            }
            case TYPE_DATE:
            {
                QDateEdit *dateEdit = new QDateEdit(parent);
                dateEdit->setCalendarPopup(true);
                QCalendarWidget * pCW = new QCalendarWidget(parent);
                pCW->setFirstDayOfWeek( Qt::Monday );
                dateEdit->setCalendarWidget( pCW );
                editor=dateEdit;
                break;
            }
            default:
            {
                editor=QItemDelegate::createEditor(parent, option, index);
                break;
            }
        }
    }
    if (editor) editor->installEventFilter(const_cast<CbRelationDelegate *>(this));
    return editor;
}

void CbRelationDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
{
    const DbTableModel *sqlModel = qobject_cast<const DbTableModel *>(index.model());
    if (sqlModel && sqlModel->relation(index.column())){
        QComboBox *combo = qobject_cast<QComboBox *>(editor);
        if (combo) {
            QAbstractItemModel *childModel=sqlModel->relation(index.column())->model();
            if (childModel){
                combo->setModel(childModel);
                combo->setModelColumn(sqlModel->relation(index.column())->columnDisplay());
                combo->setEditable(true);
                CustomCompletter *c = new CustomCompletter(combo);
                c->setModel(childModel);
                c->setCompletionColumn(sqlModel->relation(index.column())->columnDisplay());
                combo->setCompleter(c);
                int pos=combo->findText(sqlModel->data(index).toString());
                combo->setCurrentIndex(pos);
                return;
            }
        }
    }
    if (sqlModel && sqlModel->validator(index.column())){
        QLineEdit *line = qobject_cast<QLineEdit *>(editor);
        if (line) {
            line->setValidator(sqlModel->validator(index.column()));
            if (sqlModel->columnType(index.column())==TYPE_DOUBLE){
                int dec=3;
                if (sqlModel->validator(index.column())){
                    QDoubleValidator *doublevalidator = qobject_cast<QDoubleValidator*>(sqlModel->validator(index.column()));
                    if (doublevalidator) dec=doublevalidator->decimals();
                }
                line->setText(QString::number(sqlModel->data(index,Qt::EditRole).toDouble(),'f',dec));
                return;
            }
        }
    }
    return QItemDelegate::setEditorData(editor, index);
}

void CbRelationDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
    if (!index.isValid())
        return;
    DbTableModel *sqlModel = qobject_cast<DbTableModel *>(model);
    if (sqlModel && sqlModel->relation(index.column())){
        QAbstractItemModel *childModel = sqlModel->relation(index.column())->model();
        QComboBox *combo = qobject_cast<QComboBox *>(editor);
        if (!childModel || !combo) {
            return QItemDelegate::setModelData(editor, model, index);
        }
        QVariant val;
        int pos=combo->findText(combo->currentText(),Qt::MatchFixedString);
        if (pos!=-1) {
            combo->setCurrentIndex(pos);
            int currentRow = combo->currentIndex();
            int childEditIndex = sqlModel->relation(index.column())->columnKey();
            val=childModel->data(childModel->index(currentRow, childEditIndex), Qt::EditRole);
        } else val=QVariant();
        sqlModel->setData(index,val,Qt::EditRole);
    } else {
        return QItemDelegate::setModelData(editor, model, index);
    }
}

void CbRelationDelegate::updateEditorGeometry(
            QWidget *editor,
            const QStyleOptionViewItem &option,
            const QModelIndex& /* index */) const
{
    editor->setGeometry(option.rect);
}

bool CbRelationDelegate::eventFilter(QObject *object, QEvent *event)
{
    if (event->type()== QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        QLineEdit *line = qobject_cast<QLineEdit *>(object);
        if (line && line->validator()) {
            const QDoubleValidator *val = qobject_cast< const QDoubleValidator *>(line->validator());
            if (val){
                if (keyEvent->text()==",") {
                    line->insert(".");
                    return false;
                }
            }
        }

        if(keyEvent->key()==Qt::Key_Tab || keyEvent->key()==Qt::Key_Down || keyEvent->key()==Qt::Key_Up){
            //qDebug()<<"Key_Tab";
            QWidget *editor = qobject_cast<QWidget*>(object);
            emit commitData(editor);
            emit closeEditor(editor);
            return false;
        }
    }
    return QItemDelegate::eventFilter(object,event);
}

CustomCompletter::CustomCompletter(QObject *parent):QCompleter(parent)
{
    setCompletionMode(QCompleter::PopupCompletion);
    setCaseSensitivity(Qt::CaseInsensitive);
}

bool CustomCompletter::eventFilter(QObject *o, QEvent *e)
{
    if (e->type()==QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
        if (keyEvent->key()==Qt::Key_Tab) {
            this->popup()->close();
            return false;
        }
    }
    return QCompleter::eventFilter(o,e);
}
