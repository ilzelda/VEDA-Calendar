#include "scheduledialog.h"
#include "ui_scheduledialog.h"
// #include <QDateTime>
// #include <QDate>
ScheduleDialog::ScheduleDialog(QWidget *parent, const QDate &date) :
    QDialog(parent),
    ui(new Ui::ScheduleDialog)
{
    ui->setupUi(this);

    QDateTime startDateTime;
    QDateTime endDateTime;
    startDateTime.setDate(date);
    endDateTime.setDate(date);
    endDateTime = endDateTime.addDays(1);
    ui->dateTimeEditStart->setDateTime(startDateTime);
    ui->dateTimeEditEnd->setDateTime(endDateTime);
    // schedule.start =
    // schedule.end = QDateTime::currentDateTime().addDays(1);
}

ScheduleDialog::ScheduleDialog(QWidget *parent, Schedule _sch) :
    QDialog(parent),
    ui(new Ui::ScheduleDialog)
{
    ui->setupUi(this);

    ui->lineEditTitle->setText(_sch.title);
    ui->lineEditLocation->setText(_sch.location);
    ui->dateTimeEditStart->setDateTime(_sch.start);
    ui->dateTimeEditEnd->setDateTime(_sch.end);

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

    schedule.start = ui->dateTimeEditStart->dateTime();
    schedule.end = ui->dateTimeEditEnd->dateTime();

    // 입력 후 다이얼로그 종료 (Accepted)
    accept();
}
