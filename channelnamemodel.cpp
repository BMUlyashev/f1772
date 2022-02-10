#include "channelnamemodel.h"

ChannelNameModel::ChannelNameModel(QObject *parent) : QAbstractTableModel(parent)
{
    values = new QList<QPair<int, QString>>();
}
int ChannelNameModel::rowCount(const QModelIndex &/*parent*/) const
{
    return values->count();
}
int ChannelNameModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

QVariant ChannelNameModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section) {
            case COL_CHANNEL_NUM:
                return "Канал\n№" ;
            case COL_CHANNEL_NAME:
                return "Имя цепи";
        }
    }
    return QVariant();
}

Qt::ItemFlags ChannelNameModel::flags(const QModelIndex &/*index*/) const
{
    return Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled;
}

QVariant ChannelNameModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    switch (role) {
        case Qt::DisplayRole:
        switch (index.column()) {
            case COL_CHANNEL_NUM:
                value = values->at(index.row()).first;
                break;
            case COL_CHANNEL_NAME:
                value = values->at(index.row()).second;
                break;
        }
        break;
    case Qt::TextAlignmentRole:
        return int(Qt::AlignCenter);
        break;
    }
    return value;
}

bool ChannelNameModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        if (!checkIndex(index))
            return false;
        switch (index.column()) {
            case COL_CHANNEL_NUM:
                values->operator[](index.row()).first = value.toInt();
                break;
            case COL_CHANNEL_NAME:
                values->operator[](index.row()).second = value.toString();
                break;
        }
        return true;
    }
    return false;
}

void ChannelNameModel::append(int channelNumber, QString channelName)
{
    int newRow = this->values->count();
    beginInsertRows(QModelIndex(), newRow, newRow);
        values->append(QPair<int,QString>(channelNumber, channelName));
    endInsertRows();
}
void ChannelNameModel::clear()
{
    this->beginResetModel();
    (*this->values).clear();
    this->endResetModel();
}

void ChannelNameModel::updateChannelName()
{
    clear();

}
