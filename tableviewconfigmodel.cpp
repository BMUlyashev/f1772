#include "tableviewconfigmodel.h"
#include <QString>

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
    return 11;
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
     default:
        break;
    }
    return value;;
}

void TableViewConfigModel::populate(QList<SafeTester> *newValues)
{
    int idx = this->values->count();
    this->beginInsertRows(QModelIndex(),0,idx);
        this->values = newValues;
    endInsertRows();
}
