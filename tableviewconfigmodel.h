#ifndef TABLEVIEWCONFIGMODEL_H
#define TABLEVIEWCONFIGMODEL_H

#include <QAbstractTableModel>
#include "safetester.h"
#include <QString>

#include <QStyledItemDelegate>

class TableViewConfigModel : public QAbstractTableModel
{
public:
   enum TableColumnName {
        COLUMN_STEP,
        COLUMN_PLUS,
        COLUMN_MINUS,
        COLUMN_FUNC,
        COLUMN_FREQ,
        COLUMN_VOLT,
        COLUMN_HCUR,
        COLUMN_LCUR,
        COLUMN_RAMP,
        COLUMN_TIME,
    };
public:
    explicit TableViewConfigModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);


    void populate(QList<SafeTester> *newValues);
    void append(SafeTester value);
    void deleteRow(int idx);
private:
    QList<SafeTester> *values;
};

class ChanelDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ChanelDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
};

class FunctionDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    FunctionDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
};

class FrequencyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    FrequencyDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
};

#endif // TABLEVIEWCONFIGMODEL_H
