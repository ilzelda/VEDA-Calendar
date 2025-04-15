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

    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    QWidget* itemWidget = createTodoItemWidget(item);

    item->setSizeHint(itemWidget->sizeHint());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, itemWidget);

    qDebug() << "항목 추가됨";
}

QWidget* ListDialog::createTodoItemWidget(QListWidgetItem* item)
{
    QWidget* container = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(container);

    QLineEdit* lineEdit = new QLineEdit(container);
    QPushButton* deleteButton = new QPushButton("삭제", container);
    QPushButton* editButton = new QPushButton("수정", container);

    layout->addWidget(lineEdit);
    layout->addWidget(deleteButton);
    layout->addWidget(editButton);
    container->setLayout(layout);

    // 삭제 버튼 누르면 해당 아이템 삭제
    connect(deleteButton, &QPushButton::clicked, this, [=]() {
        int row = ui->listWidget->row(item);
        delete ui->listWidget->takeItem(row);  // 삭제
    });

    //editButton 기능 구현 필요. 11:23

    return container;
}
