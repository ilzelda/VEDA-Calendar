#ifndef WEEKBOX_H
#define WEEKBOX_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSimpleTextItem>
#include <QApplication>
#include <QMouseEvent>

#include "Schedule.h"

class WeekBox : public QGraphicsView
{
    Q_OBJECT
public:
    explicit WeekBox(QDate month, int n, QWidget *parent = nullptr);
    ~WeekBox();

    QList<Schedule> schedule_list;
    QGraphicsScene* scene;

    void drawSchedules();

    void showEvent(QShowEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QDate month;
    int nth_week;
    int min_day;
    int max_day;

signals:
    void newScheduleAdded(Schedule);
};



#endif // WEEKBOX_H
