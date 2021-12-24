#include "safetester.h"

SafeTester::SafeTester(){}
SafeTester::SafeTester(VoltFunction function, FrequncyACWVoltage frequency, QString voltage):
    m_function{function}, m_frequency{frequency}, m_testVoltageValue{voltage}
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
