#ifndef TABLEVIEWCONFIGMODEL_H
#define TABLEVIEWCONFIGMODEL_H

#include <QAbstractTableModel>
#include "safetester.h"
#include <QString>
class TableViewConfigModel : public QAbstractTableModel
{
    enum TableColumnName {
        COLUMN_STEP,
        COLUMN_PLUS,
        COLUMN_MINUS,
        COLUMN_FUNC,
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

    void populate(QList<SafeTester> *newValues);
private:
    QList<SafeTester> *values;
};

#endif // TABLEVIEWCONFIGMODEL_H
