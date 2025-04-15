#include "listdialog.h"
#include "./ui_listdialog.h"

ListDialog::ListDialog(QWidget *parent, const QDate &date)
    : QDialog(parent)
    , selectedDate(date)
    , ui(new Ui::ListDialog)
{
    ui->setupUi(this);

    connect(ui->addListButton, &QPushButton::clicked, this, &ListDialog::addListLine);
}

ListDialog::~ListDialog()
{
    delete ui;
}

void ListDialog::addListLine()
{
    emit callDayWidgetClicked(selectedDate);

    //QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    //QWidget* itemWidget = createTodoItemWidget();

    // item->setSizeHint(itemWidget->sizeHint());
    // ui->listWidget->addItem(item);
    // ui->listWidget->setItemWidget(item, itemWidget);

    qDebug() << "항목 추가됨";
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
    });

    return container;
}
