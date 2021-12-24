#ifndef U2270_H
#define U2270_H

#include <QString>

class U2270
{
private:
    int m_plusChanel;
    int m_minusChanel;
public:
    U2270();

    void setPlusChanel(int chanel);
    QString getPlusChanel();

    void setMinusChanel(int chanel);
    QString getMinusChanel();
};

#endif // U2270_H
