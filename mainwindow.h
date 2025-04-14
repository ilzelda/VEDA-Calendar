#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <QMap>
#include <QList>

namespace Ui {
class MainWindow;
}

// 일정 정보를 저장할 구조체
struct Schedule {
    QString title;
    QString location;
    QTime time;
    QDate date;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onShiftLeftClicked();
    void onShiftRightClicked();
    void dayButtonClicked();
    void addSchedule(Schedule newSchedule);

private:
    void updateCalendar();

    Ui::MainWindow *ui;
    QDate currentMonth;                // 현재 표시 중인 달 (매월 1일로 설정)
    QMap<QDate, QList<Schedule>> scheduleMap;  // 날짜별 일정 목록
};

#endif // MAINWINDOW_H
