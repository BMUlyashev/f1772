#include "channelnamemodel.h"
#include <QFile>
#include <QMessageBox>
#include <QComboBox>
ChannelNameModel::ChannelNameModel(QObject *parent) : QAbstractTableModel(parent)
{
    values = new QList<QPair<int, QString>>();
//    m_listOfNames = new QStringList;
    /* load from file list of permitted names */
    QFile fileNames("./PermittedNames.name");
    if (fileNames.open(QFile::ReadOnly))
    {
        QTextStream textStream(&fileNames);
        textStream.setCodec("UTF-8");
        while (true) {
            QString line = textStream.readLine();
            if (line.isNull())
                break;
            else
                m_listOfNames.append(line);
            qDebug() << m_listOfNames.last();
        }
        fileNames.close();

//        qDebug() << m_listOfNames->last();
    }   else
    {
        qDebug() << "Нету файла списка имен цепей";
        QMessageBox::critical(nullptr, "Ошибка", "Отсутствует файл <PermittedNames.name>\n"
                                                 "Наименования цепей не будут загружены.");
    }

}
ChannelNameModel::~ChannelNameModel()
{
//    delete m_listOfNames;
}
int ChannelNameModel::rowCount(const QModelIndex &/*parent*/) const
{
    return values->count();
}
int ChannelNameModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}
QStringList ChannelNameModel::getStringNames()
{
    return m_listOfNames;
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
void ChannelNameModel::setChannelName(int index, QString name)
{
    values->replace(index,QPair<int, QString>(values->at(index).first, name));
}
/*---------     ComboDeleagte       ---------------*/
NameDelegate::NameDelegate(QStringList list, QObject *parent) : QStyledItemDelegate(parent)
    {
        m_listOfNames = list;
    }
void NameDelegate::setupListNames(QStringList list)
{
    m_listOfNames = list;
}
QWidget *NameDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/,
        const QModelIndex &/*index*/) const
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems(m_listOfNames);
        return editor;
    }
void NameDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QString text = index.model()->data(index, Qt::DisplayRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        int tindex = comboBox->findText(text);
        comboBox->setCurrentIndex(tindex);
    }
void NameDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString text = comboBox->currentText();
        model->setData(index, text, Qt::EditRole);
    }
void NameDelegate::updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
    {
        editor->setGeometry(option.rect);
    }
