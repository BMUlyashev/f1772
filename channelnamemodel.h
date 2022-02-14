#ifndef CHANNELNAMEMODEL_H
#define CHANNELNAMEMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QStyledItemDelegate>
#include <QDebug>

//QStringList m_global;
class ChannelNameModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum ChannelColumnName {
        COL_CHANNEL_NUM,
        COL_CHANNEL_NAME
    };
    explicit ChannelNameModel(QObject *parent = nullptr);
    ~ChannelNameModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    void append(int channelNumber, QString channelName);
    void clear();
    QStringList getStringNames();
    void setChannelName(int index, QString name);
public slots:
    void updateChannelName();
private:
    QList<QPair<int, QString>> *values;
    QStringList m_listOfNames;

};

/* Delegate for edit channels names*/
class NameDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    NameDelegate(QStringList list, QObject *parent = nullptr);
//    ~NameDelegate();
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
    void setupListNames(QStringList list);

private:
    QStringList m_listOfNames;
};


#endif // CHANNELNAMEMODEL_H
