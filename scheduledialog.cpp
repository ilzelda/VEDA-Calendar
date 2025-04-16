#include "scheduledialog.h"
#include "ui_scheduledialog.h"
#include <QMessageBox>
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
        QDateTime check_start = ui->dateTimeEditStart->dateTime();
        QDateTime check_end = ui->dateTimeEditEnd->dateTime();

        if (check_start > check_end) {
            QMessageBox::warning(this, tr("시간 오류"), tr("종료 시간이 시작 시간보다 빠릅니다.\n"));
            return;
        }

        //동일 시간 확인하기
        if (check_start == check_end) {
            if (QMessageBox::question(this,
                                      tr("확인 필요"),
                                      tr("시작과 종료 시간이 같습니다. 저장하시겠습니까?"),
                                      QMessageBox::Yes | QMessageBox::No)
                == QMessageBox::No) {
                return;
            }
        }


    schedule.title = ui->lineEditTitle->text();
    schedule.location = ui->lineEditLocation->text();

    schedule.start = ui->dateTimeEditStart->dateTime();
    schedule.end = ui->dateTimeEditEnd->dateTime();

    // 입력 후 다이얼로그 종료 (Accepted)
    accept();
}
