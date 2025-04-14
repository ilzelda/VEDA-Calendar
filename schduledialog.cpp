#include "schduledialog.h"
#include "ui_schduledialog.h"

SchduleDialog::SchduleDialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SchduleDialog)
{
    ui->setupUi(this);
}

SchduleDialog::~SchduleDialog()
{
    delete ui;
}
