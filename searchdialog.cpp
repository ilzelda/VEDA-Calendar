#include "searchdialog.h"
#include "ui_searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SearchDialog)
{
    ui->setupUi(this);

    connect(ui->pushButtonSubmit, &QPushButton::clicked, this, &SearchDialog::onSubmit);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::onSubmit()
{
    target_title = ui->lineEditTitle->text();
    done(QDialog::Accepted);
}
