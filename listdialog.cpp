#include "Schedule.h"
#include "listdialog.h"
#include "mainwindow.h"

#include "./ui_listdialog.h"

ListDialog::ListDialog(QWidget *parent, const QDate &date)
    : QDialog(parent)
    , selectedDate(date)
    , sch_map(qobject_cast<MainWindow*>(parent)->scheduleMap)
    , ui(new Ui::ListDialog)
{
    ui->setupUi(this);
    // 이전에 저장해놓았던것들 불러오기
    QList<Schedule>& _sch_list = sch_map[date];

    for(int idx = 0; idx<_sch_list.size(); idx++)
    {
        Schedule& _sch = _sch_list[idx];
        createTodoItemWidget(_sch.title, _sch.location, _sch.start, _sch.end);
    }

    connect(ui->addListButton, &QPushButton::clicked, this, &ListDialog::addListLine);
}

ListDialog::~ListDialog()
{
    delete ui;
}

void ListDialog::addListLine()
{
    emit callDayWidgetClicked(selectedDate);
}

QWidget* ListDialog::createTodoItemWidget(
                                          const QString &title,
                                          const QString &location,
                                          const QDateTime &start,
                                          const QDateTime &end)
{
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    QWidget* container = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(container);

    QLabel* titleLabel = new QLabel(QString("제목: %1").arg(title), container);
    QLabel* locationLabel = new QLabel(QString("장소: %1").arg(location), container);
    QLabel* timeLabel = new QLabel(
        QString("시간: %1 ~ %2")
            .arg(start.toString("yyyy-MM-dd hh:mm"))
            .arg(end.toString("yyyy-MM-dd hh:mm")),
        container
        );


    QHBoxLayout* buttonLayout = new QHBoxLayout();
    //QLineEdit* lineEdit = new QLineEdit(container);
    QPushButton* deleteButton = new QPushButton("삭제", container);
    QPushButton* editButton = new QPushButton("수정", container);

    //layout->addWidget(lineEdit);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(editButton);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(locationLabel);
    mainLayout->addWidget(timeLabel);
    mainLayout->addLayout(buttonLayout);

    container->setLayout(mainLayout);
    //container->setLayout(layout);

    // 삭제 버튼 누르면 해당 아이템 삭제
    connect(deleteButton, &QPushButton::clicked, this, [=]() {
        int row = ui->listWidget->row(item);
        delete ui->listWidget->takeItem(row);  // 삭제

        // 2. map에서 스케줄 삭제 (selectedDate는 이 다이얼로그의 날짜)
        // find info of target schedule and remove schedule in current list
        QString target_title = title;
        QList<QDate> target_dates;

        QList<Schedule>& scheduleList = sch_map[selectedDate];
        for (int i = 0; i < scheduleList.size(); ++i) {
            const Schedule &sch = scheduleList[i];
            if (sch.title == title &&
                sch.location == location &&
                sch.start == start &&
                sch.end == end)
            {
                for(QDate _d=sch.start.date(); _d<=sch.end.date(); _d = _d.addDays(1))
                {
                    target_dates.append(_d);
                }
                scheduleList.removeAt(i);
                break;
            }
        }

        // remove extra schedule
        for(QDate _d : target_dates)
        {
            QList<Schedule>& scheduleList = sch_map[_d];
            for (int i = 0; i < scheduleList.size(); ++i) {
                const Schedule &sch = scheduleList[i];
                if (sch.title == title &&
                    sch.location == location &&
                    sch.start == start &&
                    sch.end == end)
                {
                    scheduleList.removeAt(i);
                    break;
                }
            }
        }
    });

    item->setSizeHint(container->sizeHint());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, container);

    return container;
}
