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
    explicit WeekBox(int n, QWidget *parent = nullptr);
    ~WeekBox();

    QList<Schedule> schedule_list;
    QGraphicsScene* scene;

    void drawSchedules();

protected:
    void showEvent(QShowEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    int nth_week;
    int min_day;
    int max_day;
};



#endif // WEEKBOX_H
