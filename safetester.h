#ifndef SAFETESTER_H
#define SAFETESTER_H

#include <QString>

class SafeTester
{
public:
enum VoltFunction {
    ACW = 0,
    DCW
};
enum FrequncyACWVoltage{
    FREQ_50HZ,
    FREQ_60HZ
};

private:
const double MAX_VOLTAGE_VALUE = 5.000;
const double MIN_VOLTAGE_VALUE = 0.050;

const double MAX_HI_CURRENT_VALUE = 42.0;
const double MIN_HI_CURRENT_VALUE = 0.001;

const double MAX_LOW_CURRENT_VALUE = 41.9;
const double MIN_LOW_CURRENT_VALUE = 0.0;

private:
    VoltFunction m_function;                            // Переменное или постоянное напряжение
    FrequncyACWVoltage m_frequency;                     // Частота сигнала при ACW
    QString m_testVoltageValue;                         // Значение испытательного напряжения
    QString m_hiCurrentValue;                         // Значение максимального тока при испытании
    QString m_lowCurrentValue;                         // Значение минимального тока при испытании



public:
    SafeTester();
    SafeTester(VoltFunction function, FrequncyACWVoltage frequency, QString voltage);
    void setVoltFunction(VoltFunction function);        // Устанавливает тип напряжения
    QString getVoltFunctionName();                          // Возращаем строку, т.к. используем только их

    void setFrequencyACWVoltage(FrequncyACWVoltage frequency);
    QString getFrequencyACWVoltage();

    void setVoltageValue(double value);
    QString getVoltageValue();

    void setHiCurrentValue(double value);
    QString getHiCurrentValue();
};

#endif // SAFETESTER_H
