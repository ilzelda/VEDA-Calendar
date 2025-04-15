#include "Schedule.h"
#include "listdialog.h"
#include "mainwindow.h"

#include "./ui_listdialog.h"

ListDialog::ListDialog(QWidget *parent, const QDate &date)
    : QDialog(parent)
    , selectedDate(date)
    , main_window(qobject_cast<MainWindow*>(parent))
    , sch_map(qobject_cast<MainWindow*>(parent)->scheduleMap)
    , ui(new Ui::ListDialog)
{
    ui->setupUi(this);
    // 이전에 저장해놓았던것들 불러오기
    QList<Schedule>& _sch_list = sch_map[date];

    for(int idx = 0; idx<_sch_list.size(); idx++)
    {
        Schedule& _sch = _sch_list[idx];
        createTodoItemWidget(_sch/*_sch.title, _sch.location, _sch.start, _sch.end*/);
    }

    connect(ui->addListButton, &QPushButton::clicked, this, &ListDialog::emitSignalAddListButtonClicked);
}

ListDialog::~ListDialog()
{
    delete ui;
}

void ListDialog::emitSignalAddListButtonClicked()
{
    emit addListButtonClicked(selectedDate);
}

QWidget* ListDialog::createTodoItemWidget(Schedule _sch)
{
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    QWidget* container = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(container);

    QLabel* titleLabel = new QLabel(QString("제목: %1").arg(_sch.title), container);
    QLabel* locationLabel = new QLabel(QString("장소: %1").arg(_sch.location), container);
    QLabel* timeLabel = new QLabel(
        QString("시간: %1 ~ %2")
            .arg(_sch.start.toString("yyyy-MM-dd hh:mm"))
            .arg(_sch.end.toString("yyyy-MM-dd hh:mm")),
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
        QList<Schedule>& scheduleList = sch_map[selectedDate];
        for (int i = 0; i < scheduleList.size(); ++i) {
            const Schedule &sch = scheduleList[i];
            if (sch.title == _sch.title &&
                sch.location == _sch.location &&
                sch.start == _sch.start &&
                sch.end == _sch.end)
            {
                scheduleList.removeAt(i);
                break;
            }
        }
    });

    connect(editButton, &QPushButton::clicked, this, [=](){
       //emitSignalAddListButtonClicked()
        //(우리가 원하는 값들 <-_sch) >> showScheduleDialogForUpdated인자값
        QString currentTitle = titleLabel->text().mid(4);
        QString currentLocation = locationLabel->text().mid(4);
        QDateTime currentStart = _sch.start;
        QDateTime currentEnd = _sch.end;
        qDebug() << "new Title : " << currentTitle;
        // Schedule 객체 생성
        Schedule currentSchedule;
        currentSchedule.title = currentTitle;
        currentSchedule.location = currentLocation;
        currentSchedule.start = currentStart;
        currentSchedule.end = currentEnd;

        Schedule newSch = qobject_cast<MainWindow*>(main_window)->showScheduleDialogForUpdate(this, currentSchedule);
        if(newSch.title.size() > 0)
        {
            // 수정된 Schedule 값을 반영해서 라벨 텍스트 업데이트
            titleLabel->setText(QString("제목: %1").arg(newSch.title));
            locationLabel->setText(QString("장소: %1").arg(newSch.location));
            timeLabel->setText(QString("시간: %1 ~ %2")
                                   .arg(newSch.start.toString("yyyy-MM-dd hh:mm"))
                                   .arg(newSch.end.toString("yyyy-MM-dd hh:mm")));
        }
    });

    item->setSizeHint(container->sizeHint());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, container);

    return container;
}
