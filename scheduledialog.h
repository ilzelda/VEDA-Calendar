#ifndef SCHEDULEDIALOG_H
#define SCHEDULEDIALOG_H

#include <QDialog>
#include <QDate>
#include "Schedule.h"

namespace Ui {
class ScheduleDialog;
}

class ScheduleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ScheduleDialog(QWidget *parent = nullptr, const QDate &date = QDate::currentDate());
    ScheduleDialog(QWidget *parent, Schedule _sch);
    ~ScheduleDialog();

    Schedule getSchedule() const;

private slots:
    // input_button 클릭 시 호출 (Qt의 auto-connect 사용)
    void on_input_button_clicked();

private:
    Ui::ScheduleDialog *ui;
    Schedule schedule;
};

#endif // SCHEDULEDIALOG_H
