#ifndef REPORTCLASS_H
#define REPORTCLASS_H

#include <QObject>

class ReportClass : public QObject
{
    Q_OBJECT
public:
    explicit ReportClass(QObject *parent = nullptr);

signals:

};

#endif // REPORTCLASS_H
