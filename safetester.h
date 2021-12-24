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

const double MIN_RAMP_TIME = 0.1;
const double MAX_RAMP_TIME = 999.9;

const double MIN_TIMER_TIME = 0.5;
const double MAX_TIMER_TIME = 999.9;


private:
    VoltFunction m_function;                            // Переменное или постоянное напряжение
    FrequncyACWVoltage m_frequency;                     // Частота сигнала при ACW
    QString m_testVoltageValue;                         // Значение испытательного напряжения
    QString m_hiCurrentValue;                         // Значение максимального тока при испытании
    QString m_lowCurrentValue;                         // Значение минимального тока при испытании
    QString m_rampTime;                                 // Время нарастания
    QString m_timerTime;                                // Время удержания


public:
    SafeTester();
    SafeTester(VoltFunction function, FrequncyACWVoltage frequency, QString voltage,
               QString hiCurrent, QString lowCurrent, QString rampTime, QString timerTime);
    void setVoltFunction(VoltFunction function);        // Устанавливает тип напряжения
    QString getVoltFunctionName();                          // Возращаем строку, т.к. используем только их
    /* Frequency of signal*/
    void setFrequencyACWVoltage(FrequncyACWVoltage frequency);
    QString getFrequencyACWVoltage();
    /* Voltage value*/
    void setVoltageValue(double value);
    QString getVoltageValue();
    /* Current hi & low*/
    void setHiCurrentValue(double value);
    QString getHiCurrentValue();
    void setLowCurrentValue(double value);
    QString getLowCurrentValue();

    void setRampTime(double value);
    QString getRampTime();

    void setTimerTime(double value);
    QString getTimerTime();


};

#endif // SAFETESTER_H
