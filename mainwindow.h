#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <QMap>
#include <QList>
#include "Schedule.h"
#include "weekbox.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onShiftLeftClicked();
    void onShiftRightClicked();
    void addSchedule(Schedule newSchedule);

    //새로운 슬롯 추가 17:05 daywidget
    void dayWidgetClicked(const QDate &date);
    void dayListAddClicked(const QDate &selectedDate);

private:
    void updateCalendar();

    Ui::MainWindow *ui;
    QDate currentMonth;                // 현재 표시 중인 달 (매월 1일로 설정)
    QMap<QDate, QList<Schedule>> scheduleMap;  // 날짜별 일정 목록
    QList<WeekBox*> weekbox_list;

protected:
    void showEvent(QShowEvent*) override;
    void resizeEvent(QResizeEvent*) override;
};

#endif // MAINWINDOW_H
