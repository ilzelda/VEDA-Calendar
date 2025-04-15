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

    QMap<QDate, QList<Schedule>> scheduleMap;  // 날짜별 일정 목록
    Schedule showScheduleDialogForUpdate(QWidget *parent, Schedule _sch);
    void deleteSchedule(Schedule oldSch);
    void updateScheduleInMap(Schedule oldSch, Schedule newSch, QDate d);

    void deleteSchedule(Schedule newSchedule);
    void addSchedule(Schedule newSchedule);

private slots:
    void onShiftLeftClicked();
    void onShiftRightClicked();


    //새로운 슬롯 추가 17:05 daywidget
    void dayListAddClicked(const QDate &selectedDate);
    void showScheduleDialogForAdd(const QDate &date);
    //void showScheduleDialogForUpdate(const QDate &date);
    //void showScheduleDialogForUpdate(QWidget *parent, Schedule _sch);
private:
    void updateCalendar();

    Ui::MainWindow *ui;
    QDate currentMonth;                // 현재 표시 중인 달 (매월 1일로 설정)
    QList<WeekBox*> weekbox_list;

protected:
    void showEvent(QShowEvent*) override;
    void resizeEvent(QResizeEvent*) override;
};

#endif // MAINWINDOW_H
