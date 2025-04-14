#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scheduledialog.h"

#include <QPushButton>
#include <QDebug>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentMonth(QDate::currentDate())
{
    ui->setupUi(this);
    // 현재 달을 1일로 설정
    currentMonth.setDate(currentMonth.year(), currentMonth.month(), 1);

    connect(ui->shift_left, &QPushButton::clicked, this, &MainWindow::onShiftLeftClicked);
    connect(ui->shift_right, &QPushButton::clicked, this, &MainWindow::onShiftRightClicked);

    updateCalendar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCalendar()
{
    ui->monthLabel->setText(currentMonth.toString("yyyy-MM"));

    // 기존에 생성된 날짜 버튼 제거
    QLayoutItem* item;
    while((item = ui->gridLayout->takeAt(0)) != nullptr) {
        if(item->widget())
            item->widget()->deleteLater();
        delete item;
    }

    int days = currentMonth.daysInMonth();
    int row = 0, column = 0;
    // 단순 7열 그리드로 날짜 버튼 생성
    for (int day = 1; day <= days; day++) {
        QPushButton *dayButton = new QPushButton(QString::number(day));
        QDate dayDate(currentMonth.year(), currentMonth.month(), day);
        dayButton->setProperty("date", dayDate);
        // 버튼 클릭 시 dayButtonClicked 슬롯 호출
        connect(dayButton, &QPushButton::clicked, this, &MainWindow::dayButtonClicked);

        ui->gridLayout->addWidget(dayButton, row, column);
        column++;
        if (column >= 7) {
            column = 0;
            row++;
        }
    }
}

void MainWindow::onShiftLeftClicked()
{
    currentMonth = currentMonth.addMonths(-1);
    currentMonth.setDate(currentMonth.year(), currentMonth.month(), 1);
    updateCalendar();
}

void MainWindow::onShiftRightClicked()
{
    currentMonth = currentMonth.addMonths(1);
    currentMonth.setDate(currentMonth.year(), currentMonth.month(), 1);
    updateCalendar();
}

void MainWindow::dayButtonClicked()
{
    // 클릭된 버튼에서 날짜 정보 읽어옴
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if(!button)
        return;
    QDate selectedDate = button->property("date").toDate();

    // 일정 추가 다이얼로그 실행 (모달)
    ScheduleDialog dlg(this, selectedDate);
    if (dlg.exec() == QDialog::Accepted) {
        Schedule newSchedule = dlg.getSchedule();
        addSchedule(newSchedule);
        qDebug() << "추가된 일정:" << newSchedule.date << newSchedule.title;
    }
}

void MainWindow::addSchedule(Schedule newSchedule)
{
    QList<Schedule> list = scheduleMap.value(newSchedule.date);
    list.append(newSchedule);
    // 시간순으로 정렬
    std::sort(list.begin(), list.end(), [](const Schedule &a, const Schedule &b){
        return a.time < b.time;
    });
    scheduleMap[newSchedule.date] = list;

    // 간단하게 해당 날짜 버튼 텍스트에 일정을 덧붙여 표시 (실제 구현에서는 schedule_bar 위젯 활용)
    for (int i = 0; i < ui->gridLayout->count(); i++) {
        QWidget *widget = ui->gridLayout->itemAt(i)->widget();
        if (widget && widget->property("date").toDate() == newSchedule.date) {
            QString text = QString::number(newSchedule.date.day());
            // 기존에 있던 일정 제목들을 덧붙임
            for (const Schedule &sch : scheduleMap[newSchedule.date]) {
                text += "\n" + sch.title;
            }
            // widget->setText(text);
        }
    }
}
