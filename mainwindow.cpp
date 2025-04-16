#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scheduledialog.h"
#include "daywidget.h"
#include "Schedule.h"
#include "listdialog.h"

#include <QPushButton>
#include <QDebug>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentMonth(QDate::currentDate())
{
    ui->setupUi(this);
    // 현재 달을 1일로 설정
    currentMonth.setDate(currentMonth.year(), currentMonth.month(), 1);

    QIcon leftArrow(":/icon/icon/left_icon.png");
    QIcon rightArrow(":/icon/icon/right_icon.png");
    ui->shift_left->setIcon(leftArrow);
    ui->shift_right->setIcon(rightArrow);
    ui->shift_left->setIconSize(QSize(32, 32));
    ui->shift_right->setIconSize(QSize(32, 32));
    ui->shift_left->setFixedSize(40, 40);
    ui->shift_right->setFixedSize(40, 40);
    if (leftArrow.isNull()) {
        qDebug() << "왼쪽 아이콘 로딩 실패!";
    }
    if (rightArrow.isNull()) {
        qDebug() << "오른쪽 아이콘 로딩 실패!";
    }
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
    QWidget *container = ui->gridLayout->parentWidget(); // centralWidget
    qDebug() << "container :" << "h:" << container->height() << "w:"<< container->width();

    // 기존 weekbox 목록이 있다면 필요에 따라 삭제 후 새로 생성합니다.
    qDeleteAll(weekbox_list);
    weekbox_list.clear();

    qDebug() << "container width:" << container->width() << "container height:" <<container->height();

    // row의 수만큼 weekbox 생성
    int n_row = row+1;
    for(int r = 0; r <= n_row; r++)
    {
        WeekBox* weekbox = new WeekBox(r, container);
        weekbox->raise();

        int yPos = r * (container->height() / n_row);

        weekbox->setGeometry(0, yPos, container->width(), container->height() / n_row);

        weekbox->scene->setSceneRect(0, 0, weekbox->width(), weekbox->height());

        QDate _date = currentMonth.addDays(r*7);
        for(int i=0; i<7; i++ )
        {
            for(Schedule _sch : scheduleMap[_date])
            {
                bool exists = false;
                // weekbox->schedule_list에 이미 같은 스케줄이 존재하는지 검사
                for (const Schedule &existing : weekbox->schedule_list) {
                    if (existing.title == _sch.title &&
                        existing.location == _sch.location &&
                        existing.start == _sch.start &&
                        existing.end == _sch.end)
                    {
                        exists = true;
                        break;
                    }
                }
                // 동일한 스케줄이 없을 경우 추가
                if (!exists) {
                    weekbox->schedule_list.append(_sch);
                }
            }
            _date = _date.addDays(1);
        }
        qDebug() << r <<"th weekbox has "<<weekbox->schedule_list.size()<<"schedule";
        weekbox->show();
        weekbox->drawSchedules();

        weekbox_list.append(weekbox);
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
    connect(&dlg, &ListDialog::addListButtonClicked, this, &MainWindow::showScheduleDialogForAdd);

    dlg.exec();
}

void MainWindow::showScheduleDialogForAdd(const QDate &selectedDate)
{
    ScheduleDialog dlg(this, selectedDate);
    if (dlg.exec() == QDialog::Accepted) {
        Schedule newSchedule = dlg.getSchedule();
        addSchedule(newSchedule);

        ListDialog* _listdialog = qobject_cast<ListDialog*>(sender());
        _listdialog->createTodoItemWidget(newSchedule);
        qDebug() << "추가된 일정:" << newSchedule.start.date() << newSchedule.title;
    }
}



//우리가 원하는 값들
Schedule MainWindow::showScheduleDialogForUpdate(QWidget *parent, Schedule oldSch)
{
    ScheduleDialog dlg(this, oldSch);
    if(dlg.exec() == QDialog::Accepted){
        Schedule newSch = dlg.getSchedule();


        //map 및 weekbox 수정
        // 기존 일정 날짜와 새 일정 날짜 구하기
        QDate oldStart = oldSch.start.date();
        QDate oldEnd   = oldSch.end.date();
        QDate newStart = newSch.start.date();
        QDate newEnd   = newSch.end.date();
        // 교집합 구하기
        QDate intersectStart = qMax(oldStart, newStart);
        QDate intersectEnd   = qMin(oldEnd, newEnd);
        bool hasIntersection = (intersectStart <= intersectEnd);
        // 1. 삭제할 기간: 기존 일정에 포함되어 있으나 새 일정에는 없는 날짜들
        if (hasIntersection) {
            // [oldStart, intersectStart - 1] 구간 삭제
            for (QDate d = oldStart; d < intersectStart; d = d.addDays(1)) {
                deleteSchedule(oldSch);
            }
            // [intersectEnd + 1, oldEnd] 구간 삭제
            for (QDate d = intersectEnd.addDays(1); d <= oldEnd; d = d.addDays(1)) {
                deleteSchedule(oldSch);
            }
        } else {
            // 교집합이 없으면 기존 전체 기간 삭제
            for (QDate d = oldStart; d <= oldEnd; d = d.addDays(1)) {
                deleteSchedule(oldSch);
            }
        }
        // 2. 수정(업데이트)할 기간: 두 일정의 공통 구간
        if (hasIntersection) {
            for (QDate d = intersectStart; d <= intersectEnd; d = d.addDays(1)) {
                updateScheduleInMap(oldSch, newSch, d);   // 기존 Schedule 정보를 새 정보로 갱신
            }
        }
        // 3. 추가할 기간: 새 일정에 포함되나 기존에는 없었던 날짜들
        if (hasIntersection) {
            // [newStart, intersectStart - 1] 구간 추가
            for (QDate d = newStart; d < intersectStart; d = d.addDays(1)) {
                addSchedule(newSch);
            }
            // [intersectEnd + 1, newEnd] 구간 추가
            for (QDate d = intersectEnd.addDays(1); d <= newEnd; d = d.addDays(1)) {
                addSchedule(newSch);
            }
        } else {
            // 교집합이 없으면 새 일정 전체 기간 추가
            for (QDate d = newStart; d <= newEnd; d = d.addDays(1)) {
                addSchedule(newSch);
            }
        }

        return newSch;
    }
    Schedule failSch;
    return failSch;
}

void MainWindow::addSchedule(Schedule newSchedule)
{
    // scheduleMap 수정
    for (QDate date = newSchedule.start.date(); date <= newSchedule.end.date(); date = date.addDays(1))
    {
        QList<Schedule> list = scheduleMap.value(date);
        list.append(newSchedule);
        scheduleMap[date] = list;
    }

    qDebug() << "before modify weekbox";

    // weekBox들 수정
    int wbl_idx_from = int((newSchedule.start.date().day()-1) / 7);
    int wbl_idx_to = int((newSchedule.end.date().day()-1) / 7);
    for(int idx=wbl_idx_from; idx<=wbl_idx_to; idx++)
    {
        weekbox_list[idx]->schedule_list.append(newSchedule);
        qDebug() << "[addSchedule] update " << idx << "th weekbox";
        weekbox_list[idx]->drawSchedules();
    }
}

void MainWindow::deleteSchedule(Schedule target_sch)
{
    // map에서 스케줄 삭제 (selectedDate는 이 다이얼로그의 날짜)
    // find info of target schedule and remove schedule in current list
    for(QDate _d=target_sch.start.date(); _d<=target_sch.end.date(); _d=_d.addDays(1))
    {
        QList<Schedule>& scheduleList = scheduleMap[_d];
        for (int i = 0; i < scheduleList.size(); ++i) {
            const Schedule &sch = scheduleList[i];
            if (sch.title == target_sch.title &&
                sch.location == target_sch.location &&
                sch.start == target_sch.start &&
                sch.end == target_sch.end)
            {
                scheduleList.removeAt(i);
                break;
            }
        }
    }

    // weekBox들 수정
    int wbl_idx_from = int((target_sch.start.date().day()-1) / 7);
    int wbl_idx_to = int((target_sch.end.date().day()-1) / 7);
    for(int idx=wbl_idx_from; idx<=wbl_idx_to; idx++)
    {
        // weekbox_list[idx]에서 삭제
        // weekbox_list의 해당 weekbox에서 target 스케줄을 삭제
        QList<Schedule>& weekSchedules = weekbox_list[idx]->schedule_list;
        for (int i = 0; i < weekSchedules.size(); ++i)
        {
            const Schedule &sch = weekSchedules[i];
            if (sch.title == target_sch.title &&
                sch.location == target_sch.location &&
                sch.start == target_sch.start &&
                sch.end == target_sch.end)
            {
                weekSchedules.removeAt(i);
                break;
            }
        }
        weekbox_list[idx]->drawSchedules();
    }
}


void MainWindow::updateScheduleInMap(Schedule oldSch, Schedule newSch, QDate d)
{
    QList<Schedule>& sch_list = scheduleMap[d];
    for (int i = 0; i < sch_list.size(); ++i) {
        Schedule &sch = sch_list[i];
        if (sch.title == oldSch.title &&
            sch.location == oldSch.location &&
            sch.start == oldSch.start &&
            sch.end == oldSch.end)
        {
            sch.title = newSch.title;
            sch.location = newSch.location;
            sch.start = newSch.start;
            sch.end = newSch.end;
            break;
        }
    }

    // weekBox들 수정
    int wbl_idx_from = int((newSch.start.date().day()-1) / 7);
    int wbl_idx_to = int((newSch.end.date().day()-1) / 7);
    for(int idx=wbl_idx_from; idx<=wbl_idx_to; idx++)
    {
        // weekbox_list[idx]에서 삭제
        // weekbox_list의 해당 weekbox에서 target 스케줄을 삭제
        QList<Schedule>& weekSchedules = weekbox_list[idx]->schedule_list;
        for (int i = 0; i < weekSchedules.size(); ++i)
        {
            Schedule &sch = weekSchedules[i];
            if (sch.title == oldSch.title &&
                sch.location == oldSch.location &&
                sch.start == oldSch.start &&
                sch.end == oldSch.end)
            {
                sch.title = oldSch.title ;
                sch.location = oldSch.location ;
                sch.start = oldSch.start ;
                sch.end = oldSch.end;
                break;
            }
        }
        weekbox_list[idx]->drawSchedules();
    }
}


void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    updateCalendar();
    for(int idx=0; idx<weekbox_list.size(); idx++)
    {
        weekbox_list[idx]->showEvent(event);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateCalendar();
    for(int idx=0; idx<weekbox_list.size(); idx++)
    {
        weekbox_list[idx]->resizeEvent(event);
    }
}
