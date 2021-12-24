#include "safetester.h"

SafeTester::SafeTester(){
    m_function = VoltFunction::ACW;
    m_frequency = FrequncyACWVoltage::FREQ_50HZ;
    m_testVoltageValue = "0.100";
    m_hiCurrentValue = "1.000";
    m_lowCurrentValue = "0.000";
    m_rampTime = "5.000";
    m_timerTime = "10.000";
}
SafeTester::SafeTester(VoltFunction function, FrequncyACWVoltage frequency, QString voltage,
                       QString hiCurrent, QString lowCurrent, QString rampTime, QString timerTime):
    m_function{function},
    m_frequency{frequency},
    m_testVoltageValue{voltage},
    m_hiCurrentValue{hiCurrent},
    m_lowCurrentValue{lowCurrent},
    m_rampTime{rampTime},
    m_timerTime{timerTime}
{

}

void SafeTester::setVoltFunction(VoltFunction function){m_function = function;}
QString SafeTester::getVoltFunctionName()
{
    if(m_function == SafeTester::ACW)
        return "ACW";
    if(m_function == SafeTester::DCW)
        return "DCW";
    return "ACW";
}

void SafeTester::setFrequencyACWVoltage(FrequncyACWVoltage frequency){m_frequency = frequency;}
QString SafeTester::getFrequencyACWVoltage()
{
    if(m_frequency == FrequncyACWVoltage::FREQ_50HZ)
        return "50";
    if(m_frequency == FrequncyACWVoltage::FREQ_60HZ)
        return "60";
    return "50";
}

void SafeTester::setVoltageValue(double value)
{
    if(value >= MIN_VOLTAGE_VALUE && value <= MAX_VOLTAGE_VALUE)
    {
        m_testVoltageValue = QString().number(value, 'f', 3);
    }else{
        m_testVoltageValue = "0.050";
    }
}
QString SafeTester::getVoltageValue(){return m_testVoltageValue;}


void SafeTester::setHiCurrentValue(double value)
{
    if (value >= MIN_HI_CURRENT_VALUE && value <= MAX_HI_CURRENT_VALUE)
        m_hiCurrentValue = QString().number(value, 'f', 3);
    else
        m_hiCurrentValue = "0.001";
}
QString SafeTester::getHiCurrentValue(){return m_hiCurrentValue;}

void SafeTester::setLowCurrentValue(double value)
{
    if (value >= MIN_LOW_CURRENT_VALUE && value <= MAX_LOW_CURRENT_VALUE)
        m_lowCurrentValue = QString().number(value, 'f', 3);
    else
        m_lowCurrentValue = "0.000";
}
QString SafeTester::getLowCurrentValue(){return m_lowCurrentValue;}

void SafeTester::setRampTime(double value)
{
    if (value >= MIN_RAMP_TIME && value <= MAX_RAMP_TIME)
        m_rampTime = QString().number(value, 'f', 3);
    else
        m_rampTime = "0.1";
}
QString SafeTester::getRampTime(){return m_rampTime;}

void SafeTester::setTimerTime(double value)
{
    if (value >= MIN_TIMER_TIME && value <= MAX_TIMER_TIME)
        m_timerTime = QString().number(value, 'f', 3);
    else
        m_timerTime = "0.5";
}
QString SafeTester::getTimerTime(){return m_timerTime;}
