#ifndef TABLEVIEWCONFIGMODEL_H
#define TABLEVIEWCONFIGMODEL_H

#include <QAbstractTableModel>
#include "safetester.h"
#include <QString>
#include <QApplication>

#include <QStyledItemDelegate>

class TableViewConfigModel : public QAbstractTableModel
{
    Q_OBJECT
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
   bool isEditable = false;
public:
    explicit TableViewConfigModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    void clear();
    void populate(QList<SafeTester> *newValues);
    void append(SafeTester value);
    void deleteRow(int idx);
signals:
    void modelChanged();
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

class ReadOnlyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ReadOnlyDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
};

class ValueDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ValueDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
};

class ProgressBarDelegate : public QStyledItemDelegate{
public:
    ProgressBarDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // TABLEVIEWCONFIGMODEL_H
