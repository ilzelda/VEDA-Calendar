#include "daywidget.h"
#include "ui_daywidget.h"
#include <QDate>
#include <QVBoxLayout>
#include <QResizeEvent>

DayWidget::DayWidget(const QDate &date, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DayWidget)
    , date(date)
{
    ui->setupUi(this);

    ui->pushButton->setText(date.toString("dd"));
    ui->pushButton->setStyleSheet("text-align: top center;");

    resize(60,90);

    //최소 크기 없애기
    this->setMinimumSize(60, 90);
    ui->pushButton->setMinimumSize(60, 90);
    //레이아웃 추가
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(ui->pushButton);
    layout->setContentsMargins(0, 0, 0, 0); // 테두리 여백 제거 (선택 사항)
    layout->setSpacing(0); // 위젯 사이 여백 제거 (선택 사항)
    this->setLayout(layout);


    connect(ui->pushButton, &QPushButton::clicked, this, &DayWidget::handleButtonClick);

}

DayWidget::~DayWidget()
{
    delete ui;
}

void DayWidget::handleButtonClick()
{
    emit clicked(date);
}

void DayWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    int side = qMin(width(), height());
    ui->pushButton->resize(side, side);
}


