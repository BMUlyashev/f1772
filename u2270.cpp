#include "u2270.h"

U2270::U2270()
{
    m_plusChanel = 0;
    m_minusChanel = 0;
}

void U2270::setPlusChanel(int chanel)
{
    if(chanel >= 0)
        m_plusChanel = chanel;
}
QString U2270::getPlusChanel(){return QString().number(m_plusChanel);}

void U2270::setMinusChanel(int chanel)
{
    if(chanel >= 0)
        m_minusChanel = chanel;
}
QString U2270::getMinusChanel(){return QString().number(m_minusChanel);}
