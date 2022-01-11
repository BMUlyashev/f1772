#include "tableviewconfigmodel.h"
#include <QString>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>

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
    return 10;
}
QVariant TableViewConfigModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case COLUMN_STEP:
            return QString("Шаг №");
        case COLUMN_PLUS:
            return QString("№ Канала\nПлюс");
        case COLUMN_MINUS:
            return QString("№ Канала\nМинус");
        case COLUMN_FUNC:
            return QString("Тип\nСигнала");
        case COLUMN_FREQ:
            return QString("Частота,Гц");
        case COLUMN_VOLT:
            return QString("Напряжение\nПробоя,кВ");
        case COLUMN_HCUR:
            return QString("HiSET,mA");
        case COLUMN_LCUR:
            return QString("LoSET,mA");
        case COLUMN_RAMP:
            return QString("Время\nНарастания,с");
        case COLUMN_TIME:
            return QString("Время\nВыдержки,с");
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
        case COLUMN_FREQ:
            value = this->values->at(index.row()).getFrequencyACWVoltage();
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
            case COLUMN_FUNC:
                if (value.toString() == "ACW")
                    values->operator[](index.row()).setVoltFunction(SafeTester::ACW);
                else if (value.toString() == "DCW")
                    values->operator[](index.row()).setVoltFunction(SafeTester::DCW);
                break;
            case COLUMN_FREQ:
              if (value.toString() == "50")
                  values->operator[](index.row()).setFrequencyACWVoltage(SafeTester::FREQ_50HZ);
              else if (value.toString() == "60")
                 values->operator[](index.row()).setFrequencyACWVoltage(SafeTester::FREQ_60HZ);
             break;
        case COLUMN_VOLT:
            values->operator[](index.row()).setVoltageValue(value.toDouble());
            break;
        case COLUMN_HCUR:
            values->operator[](index.row()).setHiCurrentValue(value.toDouble());
            break;
        case COLUMN_LCUR:
            values->operator[](index.row()).setLowCurrentValue(value.toDouble());
            break;
        case COLUMN_RAMP:
            values->operator[](index.row()).setRampTime(value.toDouble());
            break;
        case COLUMN_TIME:
            values->operator[](index.row()).setTimerTime(value.toDouble());
            break;
        }
        return true;
    }
    return false;
}
void TableViewConfigModel::clear()
{
    this->beginResetModel();
    (*this->values).clear();
    this->endResetModel();
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

/*---------     ComboDeleagte       ---------------*/
FunctionDelegate::FunctionDelegate(QObject *parent) : QStyledItemDelegate(parent){}
QWidget *FunctionDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/,
        const QModelIndex &/*index*/) const
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItem("ACW");
        editor->addItem("DCW");
        return editor;
    }
void FunctionDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QString text = index.model()->data(index, Qt::DisplayRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        int tindex = comboBox->findText(text);
        comboBox->setCurrentIndex(tindex);
    }
void FunctionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString text = comboBox->currentText();
        model->setData(index, text, Qt::EditRole);
    }
void FunctionDelegate::updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
    {
        editor->setGeometry(option.rect);
    }
/*---------     FrequencyDeleagte       ---------------*/
FrequencyDelegate::FrequencyDelegate(QObject *parent) : QStyledItemDelegate(parent){}
QWidget *FrequencyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/,
        const QModelIndex &/*index*/) const
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItem("50");
        editor->addItem("60");
        return editor;
    }
void FrequencyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QString text = index.model()->data(index, Qt::DisplayRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        int tindex = comboBox->findText(text);
        comboBox->setCurrentIndex(tindex);
    }
void FrequencyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString text = comboBox->currentText();
        model->setData(index, text, Qt::EditRole);
    }
void FrequencyDelegate::updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
    {
        editor->setGeometry(option.rect);
    }

/*---------     readOnly Delegate delegate       ---------------*/
ReadOnlyDelegate::ReadOnlyDelegate(QObject *parent) : QStyledItemDelegate(parent){}
QWidget *ReadOnlyDelegate::createEditor(QWidget */*parent*/, const QStyleOptionViewItem &/*option*/,
         const QModelIndex &/*index*/) const
{
    return NULL;
}
/*---------     lineEdit delegate       ---------------*/
ValueDelegate::ValueDelegate(QObject *parent) : QStyledItemDelegate(parent){}

QWidget *ValueDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/,
                                     const QModelIndex &/*index*/) const
{
//    QString text = index.model()->data(index, Qt::DisplayRole).toString();
    QLineEdit *editor = new QLineEdit(parent);
//    QDoubleValidator* validator =  new QDoubleValidator( 0.000, 100.0, 3 );
//    validator->setNotation(QDoubleValidator::StandardNotation);
//    editor->setValidator(validator);
    // QRegExp
    //editor->setText(text);
    return  editor;
}

void ValueDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::DisplayRole).toString();
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(text);
}
void ValueDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const
    {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString text = lineEdit->text();
        text = text.replace(',', '.');
        model->setData(index, text, Qt::EditRole);
    }
void ValueDelegate::updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
    {
        editor->setGeometry(option.rect);
    }
