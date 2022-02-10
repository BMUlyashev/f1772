#ifndef CHANNELNAMEMODEL_H
#define CHANNELNAMEMODEL_H

#include <QAbstractTableModel>
#include <QObject>

class ChannelNameModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum ChannelColumnName {
        COL_CHANNEL_NUM,
        COL_CHANNEL_NAME
    };
    explicit ChannelNameModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    void append(int channelNumber, QString channelName);
    void clear();
public slots:
    void updateChannelName();
private:
    QList<QPair<int, QString>> *values;
};

#endif // CHANNELNAMEMODEL_H
