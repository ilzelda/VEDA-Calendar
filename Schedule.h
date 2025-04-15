#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QString>
#include <QDateTime>


struct Schedule {
    QString title;
    QString location;
    QDateTime start;
    QDateTime end;
};
#endif // SCHEDULE_H
