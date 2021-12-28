#include "tableviewconfigmodel.h"
#include <QString>
#include <QSpinBox>

TableViewConfigModel::TableViewConfigModel(QObject *parent) : QAbstractTableModel(parent)
{
    values = new QList<SafeTester>();
}

int TableViewConfigModel::rowCount(const QModelIndex &) const
{
    return values->count();
}
int TableViewConfigModel::columnCount(const QModelIndex &) const
{
    return 9;
}
QVariant TableViewConfigModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case COLUMN_STEP:
            return QString("Шаг");
        case COLUMN_PLUS:
            return QString("№ Канала\nПлюс");
        case COLUMN_MINUS:
            return QString("№ Канала\nМинус");
        case COLUMN_FUNC:
            return QString("Тип\nСигнала");
        case COLUMN_VOLT:
            return QString("Напряжение\nПробоя");
        case COLUMN_HCUR:
            return QString("HI SET");
        case COLUMN_LCUR:
            return QString("LO SET");
        case COLUMN_RAMP:
            return QString("RAMP");
        case COLUMN_TIME:
            return QString("TIMER");
        }
    }
    return QVariant();
}

QVariant TableViewConfigModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case COLUMN_STEP:
            value = index.row() + 1;
            break;
        case COLUMN_PLUS:
            value =this->values->at(index.row()).getPlusChanel();
            break;
        case COLUMN_MINUS:
            value = this->values->at(index.row()).getMinusChanel();
            break;
        case COLUMN_FUNC:
            value = this->values->at(index.row()).getVoltFunctionName();
            break;
        case COLUMN_VOLT:
            value = this->values->at(index.row()).getVoltageValue();
            break;
        case COLUMN_HCUR:
            value = this->values->at(index.row()).getHiCurrentValue();
            break;
        case COLUMN_LCUR:
            value = this->values->at(index.row()).getLowCurrentValue();
            break;
        case COLUMN_RAMP:
            value = this->values->at(index.row()).getRampTime();
            break;
        case COLUMN_TIME:
            value = this->values->at(index.row()).getTimerTime();
            break;
        }
        break;
     case Qt::TextAlignmentRole:
        return int(Qt::AlignCenter);
        break;
    case Qt::UserRole:
        break;
//    case Qt::FontRole:
//       return int(Qt::AlignCenter);
//       break;
     default:
        break;
    }
    return value;;
}

Qt::ItemFlags TableViewConfigModel::flags(const QModelIndex &/*index*/) const
{
    return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}

bool TableViewConfigModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole)
    {
        if(!checkIndex(index))
            return false;
        switch (index.column())
        {
            case COLUMN_PLUS:
                values->operator[](index.row()).setPlusChanel(value.toInt());
                break;
            case COLUMN_MINUS:
                values->operator[](index.row()).setMinusChanel(value.toInt());
                break;
        }
    }
    return false;
}

void TableViewConfigModel::populate(QList<SafeTester> *newValues)
{
    int idx = this->values->count();
    this->beginInsertRows(QModelIndex(),0,idx);
        this->values = newValues;
    endInsertRows();
}

void TableViewConfigModel::append(SafeTester value)
{
    int newRow = this->values->count();
    beginInsertRows(QModelIndex(), newRow, newRow);
        values->append(value);
    endInsertRows();
}

void TableViewConfigModel::deleteRow(int idx)
{
    this->beginRemoveRows(QModelIndex(), idx,idx);
        (*this->values).removeAt(idx);
    this->endRemoveRows();
}

/*==================    Delegates   ======================*/

ChanelDelegate::ChanelDelegate(QObject *parent) : QStyledItemDelegate(parent)
{}

QWidget *ChanelDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &/* option */,
                                       const QModelIndex &/* index */) const
{
    QSpinBox *editor = new QSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(1);
    editor->setMaximum(100);
    return editor;
}
void ChanelDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::DisplayRole).toInt();
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->setValue(value);
}
void ChanelDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();
    model->setData(index, value, Qt::EditRole);
}
void ChanelDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
