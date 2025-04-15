#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scheduledialog.h"
#include "daywidget.h"
#include "Schedule.h"
#include "listdialog.h"

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

// void MainWindow::updateCalendar()
// {
//     ui->monthLabel->setText(currentMonth.toString("yyyy-MM"));

//     // 기존에 생성된 날짜 버튼 제거
//     QLayoutItem* item;
//     while((item = ui->gridLayout->takeAt(0)) != nullptr) {
//         if(item->widget())
//             item->widget()->deleteLater();
//         delete item;
//     }

//     int days = currentMonth.daysInMonth();
//     int row = 0, column = 0;
//     // 단순 7열 그리드로 날짜 버튼 생성
//     for (int day = 1; day <= days; day++) {
//         QPushButton *dayButton = new QPushButton(QString::number(day));
//         QDate dayDate(currentMonth.year(), currentMonth.month(), day);
//         dayButton->setProperty("date", dayDate);
//         // 버튼 클릭 시 dayButtonClicked 슬롯 호출
//         connect(dayButton, &QPushButton::clicked, this, &MainWindow::dayButtonClicked);

//         ui->gridLayout->addWidget(dayButton, row, column);
//         column++;
//         if (column >= 7) {
//             column = 0;
//             row++;
//         }
//     }
// }

void MainWindow::updateCalendar()
{
    ui->monthLabel->setText(currentMonth.toString("yyyy-MM"));

    // 기존 버튼 제거
    QLayoutItem* item;
    while((item = ui->gridLayout->takeAt(0)) != nullptr) {
        if(item->widget())
            item->widget()->deleteLater();
        delete item;
    }

    int days = currentMonth.daysInMonth();
    int row = 0, column = 0;

    for (int day = 1; day <= days; day++) {
        QDate dayDate(currentMonth.year(), currentMonth.month(), day);
        DayWidget *dayWidget = new DayWidget(dayDate);

        connect(dayWidget, &DayWidget::clicked, this, &MainWindow::dayListAddClicked);

        ui->gridLayout->addWidget(dayWidget, row, column);
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


// void MainWindow::dayButtonClicked()
// {
//     QPushButton *button = qobject_cast<QPushButton*>(sender());
//     if (!button) return;

//     QDate selectedDate = button->property("date").toDate();
//     ScheduleDialog dlg(this, selectedDate);
//     if (dlg.exec() == QDialog::Accepted) {
//         Schedule newSchedule = dlg.getSchedule();
//         addSchedule(newSchedule);
//         qDebug() << "추가된 일정:" << newSchedule.date << newSchedule.title;
//     }
// }

void MainWindow::dayListAddClicked(const QDate &selectedDate)
{
    //ListDialog dlg(this);  // 부모 지정
    ListDialog dlg(this, selectedDate);
    dlg.setWindowTitle(selectedDate.toString("yyyy-MM-dd"));  // 선택된 날짜로 타이틀 설정 (선택사항)

    //dayWidgetClicked connect해주기 slot,signal
    //connect(dlg,&listdialog,)
    //connect(&dlg, &QPushButton::clicked, this, &MainWindow::dayWidgetClicked);
    //커스텀 시그널을 만들어서 emit을 통해 dayWidgetClicked 호출
    //listdialog에서 "할 일 추가" 버튼을 누르면 addListLine 실행 addListLine은
    // callDayWidgetClicked를 emit한다 그러면 아래의 connect문을 통해 dayWidgetClicked가 실행된다.
    connect(&dlg, &ListDialog::callDayWidgetClicked, this, &MainWindow::dayWidgetClicked);


    dlg.exec();
}


void MainWindow::dayWidgetClicked(const QDate &selectedDate)
{
    ScheduleDialog dlg(this, selectedDate);
    if (dlg.exec() == QDialog::Accepted) {
        Schedule newSchedule = dlg.getSchedule();
        addSchedule(newSchedule);
       qDebug() << "추가된 일정:" << newSchedule.start.date() << newSchedule.title;
    }
}


void MainWindow::addSchedule(Schedule newSchedule)
{
    QList<Schedule> list = scheduleMap.value(newSchedule.start.date());
    list.append(newSchedule);
    // 시간순으로 정렬
     std::sort(list.begin(), list.end(), [](const Schedule &a, const Schedule &b){
         return a.start.time() < b.start.time();
    });
    scheduleMap[newSchedule.start.date()] = list;

    // 간단하게 해당 날짜 버튼 텍스트에 일정을 덧붙여 표시 (실제 구현에서는 schedule_bar 위젯 활용)
    for (int i = 0; i < ui->gridLayout->count(); i++) {
        QWidget *widget = ui->gridLayout->itemAt(i)->widget();
        if (widget && widget->property("date").toDate() == newSchedule.start.date()) {
            QString text = QString::number(newSchedule.start.date().day());
            // 기존에 있던 일정 제목들을 덧붙임
            for (const Schedule &sch : scheduleMap[newSchedule.start.date()]) {
                text += "\n" + sch.title;
            }
            // widget->setText(text);
        }
    }
}


