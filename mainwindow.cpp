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

    // qDebug() << "row :" << row;
    // weekbox들을 gridLayout의 부모 위젯에 올리고, pushbutton들 위에 보이게 처리
    // gridLayout의 부모 위젯을 가져옵니다. (예: ui->calendarWidget 등)
    QWidget *container = ui->gridLayout->parentWidget();
    // qDebug() << "container :" << "h:" << container->height() << "w:"<< container->width();

    // 기존 weekbox 목록이 있다면 필요에 따라 삭제 후 새로 생성합니다.
    qDeleteAll(weekbox_list);
    weekbox_list.clear();

    // // row의 수만큼 weekbox 생성
    // int n_row = row+1;
    // for(int r = 0; r <= n_row; r++)
    // {
    //     WeekBox* weekbox = new WeekBox(r, container);
    //     // weekbox의 위치와 크기를 원하는 대로 지정합니다.
    //     // gridLayout에서 r번째 행의 영역에 맞게 위치시키기 위한 계산
    //     int yPos = r * (container->height() / n_row);
    //     weekbox->setGeometry(0, yPos, container->width(), container->height() / n_row);

    //     // pushbutton들 위로 올리기 위해 raise() 호출
    //     weekbox->raise();

    //     weekbox->scene->setSceneRect(0, 0, weekbox->width(), weekbox->height());
    //     // weekbox->show();

    //     weekbox_list.append(weekbox);
    // }
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

        qDebug() << "addSchedule(newSchedule)";

        ListDialog* _listdialog = qobject_cast<ListDialog*>(sender());
        _listdialog->createTodoItemWidget(newSchedule.title, newSchedule.location, newSchedule.start, newSchedule.end);

        qDebug() << "추가된 일정:" << newSchedule.start.date() << newSchedule.title;

    }
}


void MainWindow::addSchedule(Schedule newSchedule)
{
    qDebug() << "inside addSchedule()";

    // scheduleMap 수정
    for (QDate date = newSchedule.start.date(); date <= newSchedule.end.date(); date = date.addDays(1)) {
        QList<Schedule> list = scheduleMap.value(date);
        list.append(newSchedule);
        // // 시간순으로 정렬
        // std::sort(list.begin(), list.end(), [](const Schedule &a, const Schedule &b){
        //     return a.time < b.time;
        // });
        scheduleMap[date] = list;
    }

    qDebug() << "before modify weebox";

    // weekBox들 수정
    int wbl_idx_from = int((newSchedule.start.date().day()-1) / 7);
    int wbl_idx_to = int((newSchedule.end.date().day()-1) / 7);
    for(int idx=wbl_idx_from; idx<=wbl_idx_to; idx++)
    {
        weekbox_list[idx]->schedule_list.append(newSchedule);
        qDebug() << "update " << idx << "th weekbox";
        weekbox_list[idx]->show();
    }
}


void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    updateCalendar();
    for(int idx=0; idx<weekbox_list.size(); idx++)
    {
        weekbox_list[idx]->show();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateCalendar();
    for(int idx=0; idx<weekbox_list.size(); idx++)
    {
        weekbox_list[idx]->show();
    }
}
