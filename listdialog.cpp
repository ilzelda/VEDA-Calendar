#include "listdialog.h"
#include "./ui_listdialog.h"

ListDialog::ListDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ListDialog)
{
    ui->setupUi(this);

    connect(ui->addListButton, &QPushButton::clicked, this, &ListDialog::addListLine);
    connect(ui->saveButton, &QPushButton::clicked, this, &ListDialog::savefile);
    connect(ui->loadButton, &QPushButton::clicked, this, &ListDialog::loadfile);
}

ListDialog::~ListDialog()
{
    delete ui;
}

void ListDialog::savefile()
{
    qDebug() << "savefile()\n";
}

void ListDialog::loadfile()
{
    qDebug() << "loadfile()\n";
}

void ListDialog::addListLine()
{
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

    //QCheckBox* checkbox = new QCheckBox(container);
    QLineEdit* lineEdit = new QLineEdit(container);
    QPushButton* deleteButton = new QPushButton("삭제", container);

    //layout->addWidget(checkbox);
    layout->addWidget(lineEdit);
    layout->addWidget(deleteButton);
    container->setLayout(layout);

    // 삭제 버튼 누르면 해당 아이템 삭제
    connect(deleteButton, &QPushButton::clicked, this, [=]() {
        int row = ui->listWidget->row(item);
        delete ui->listWidget->takeItem(row);  // 삭제
    });

    return container;
}
