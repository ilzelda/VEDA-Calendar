#include "scheduledialog.h"
#include "ui_scheduledialog.h"
// #include <QDateTime>
// #include <QDate>
ScheduleDialog::ScheduleDialog(QWidget *parent, const QDate &date) :
    QDialog(parent),
    ui(new Ui::ScheduleDialog)
{
    ui->setupUi(this);

    QTime defaultTime = QTime::currentTime();
    schedule.start = QDateTime(date, defaultTime); //

    //schedule.date = date;
    //schedule.time = QTime::currentTime();  // 기본 현재 시간 설정
}

ScheduleDialog::~ScheduleDialog()
{
    delete ui;
}

Schedule ScheduleDialog::getSchedule() const
{
    return schedule;
}

void ScheduleDialog::on_input_button_clicked()
{
    schedule.title = ui->lineEditTitle->text();
    schedule.location = ui->lineEditLocation->text();

    QTime inputTime = ui->dateTimeEdit->time();
    // schedule.time = ui->timeEdit->time();
    schedule.start.setTime(inputTime);
    // 입력 후 다이얼로그 종료 (Accepted)
    accept();
}
