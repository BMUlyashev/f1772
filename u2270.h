#ifndef U2270_H
#define U2270_H

#include <QString>

class U2270
{
private:
    int m_plusChanel;
    int m_minusChanel;

    const QString U2270_SERIAL_RESET = "CLR";
    const QString U2270_SERIAL_LOAD = "LOAD";


public:
    U2270();

    void setPlusChanel(int chanel);
    QString getPlusChanel();

    void setMinusChanel(int chanel);
    QString getMinusChanel();
    /* return "Interface" string to reset U2270*/
    QString reset();
};

#endif // U2270_H
